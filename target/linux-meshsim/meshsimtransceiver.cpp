#include "MeshSimTransceiver"
#include <IPhyObserver>
#include <phy/Frame>
#include <trace/Trace>
#include "MeshSimFactory"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
using boost::asio::ip::tcp;
#include <QtGui/QApplication>
#include <QtGui/QWidget>


/*** Mesh Simulator options. *****************************************************************************************/

extern const char * __MESH_SERVER;		// URL where the simulated NRF Transceiver connects to (IPv4).
extern uint16_t 	__MESH_PORT;		// The TCP port to connect to.
extern const char * __MESH_NAME;		// Pointer to the name of the node.
extern bool 		__XF_PAUSED;		// External boolean to stop XF execution.


/*** Mesh Simulator communication definitions. ***********************************************************************/

namespace MeshSimulator
{
	typedef enum
	{
		InvalidCI               = 0 ,	// Invalid Channel Identifier.
		FrameDataCI             = 1 ,	// Frame Data Channel Identifier, used for raw frames (simulation PDUs).
		SimulationIndicationCI  = 2 ,	// Simulation Indication Channel Identifier, used for ACK/NACK.
		SimulationControlCI     = 3 ,	// Simulation Control Channel Identifier, used to control the node.
		TraceCI                 = 4 ,	// Trace Channel Identifier, sends messages to the MeshSimulator (Not impl.)
		DebugCommandCI          = 5 ,	// Debug Command Channel Identifier, used to send commands to the code.
		RpcCI                   = 6 ,	// Remote Procedure Call Channel Identifier, not implemented on MeshSimulator.
		NodePrivMessagesCI      = 7		// Private Node Messages Channel Identifier, not implemented.
	} ChannelIdentifier;

	typedef enum
	{
		InvalidSIT              = 0 ,	// Invalid Simulation Indication Type.
		AckSIT                  = 1 ,	// Acknowledge from a node that received the frame.
		NackSIT                 = 2		// Not Acknowledged.
	} SimulationIndicationType;

	typedef enum
	{
		InvalidSCC              = 0 ,	// Invalid Simulation Control Command.
		SetMacAddressSCC        = 1 ,	// Changes the MAC address of the node. MUST BE THE FIRST MESSAGE!
		EnableNodeSCC           = 2 ,	// Enables the node (visual feedback) on the MeshSimulator.
		DisableNodeSCC          = 3 ,	// Disables the node including visual feedback on the MeshSimulator.
		ChangeColorSCC          = 4 ,	// Changes the color of the node inside the MeshSimulator UI.
		ChangeNameSCC           = 5		// Changes the name of the node inside the MeshSimulator UI.
	} SimulationControlCommand;

	typedef enum
	{
		InvalidDC               = 0 ,	// Invalid Debug Command.
		RunContDC               = 1 ,	// Starts/Continues execution.
		PauseDC                 = 2 ,	// Stops execution.
		StepDC                  = 3 ,	// Single step execution.
		ResetDC                 = 4 ,	// Resets the code.
		QuitDC                  = 5 ,	// Quits the application connected to the MeshSimulator.
		IdentifyDC              = 6		// Identifies the instance controlling the node.
	} DebugCommand;

	enum
	{
		MessageHeaderSize		=  5 ,	// The size of a message header (1 byte for channel, 4 bytes size).
		MessageChannelIdOffset	=  0 ,	// The first byte is always the channel ID.
		MessageLenghtOffset		=  1 ,	// The second up to the fifth are the size in big endian.
		MessageLengthSize		=  4 ,	// The length of the message is coded with four bytes.
		MessageCommandOffset	=  5 ,	// The position of the command.
		MessageCommandSize		=  1	// The size of a standard command.
	};
}


/*** NRF Wireless PHY (simulation) implementation. *******************************************************************/

class MeshSimTransceiver::Private
{
public:
	enum
	{
		MinimalFrameSize 		= 10 ,				// The minimal size (complete frame) for the PHY.
		MaximalFramePayloadSize	= 32 ,				// The maximal PDU payload size.
		FrameHeaderSize			=  7 ,				// The number of bytes used for the frame header.
		FramePreambleOffset		=  0 ,
		FrameAddressOffset		=  1 ,
		FrameSizeOffset			=  2 ,
		FrameTrailerSize		=  2 ,				// The number of bytes used for trailer (CRC).
		FrameDecorationSize		= FrameHeaderSize + // The number of bytes for the PHY frame without payload.
								  FrameTrailerSize
	};

	Private() : observer( NULL ) , socket( io_service ) , channel( 0 ) , busy( false ) , _mode( Active ) ,
			    _autoAck( false )
	{}

	// This method is called by the boost socket if data is available or the connection dropped.
	static void onRead( const boost::system::error_code & error, size_t bytes_transferred )
	{
		// Is it a valid TCP message or was there an error while trying to receive?
		if ( !error )
		{
			uint8_t * buffer = _pImpl->rxBuffer;
			size_t bytesLeft = bytes_transferred;

			// As long as we have data...
			while( bytesLeft > MeshSimulator::MessageHeaderSize )
			{
				// Get channel and frame size.
				MeshSimulator::ChannelIdentifier channelIdentifier =
						(MeshSimulator::ChannelIdentifier)buffer[MeshSimulator::MessageChannelIdOffset];
				uint32_t frameSize;
				for ( int i = 0 ; i < MeshSimulator::MessageLengthSize ; i++ )
					*( (uint8_t *)&frameSize + i ) = buffer[i + MeshSimulator::MessageLenghtOffset];
				frameSize = ntohl( frameSize );

				// We need complete messages.
				if ( bytesLeft >= frameSize )
				{
					// On which channel did the message arrived?
					switch( channelIdentifier )
					{
						// It is on the frame channel, so it certainly must be a data frame.
						case MeshSimulator::FrameDataCI:
							// Check validity.
							if ( frameSize >= MinimalFrameSize &&
								 frameSize == static_cast<unsigned int>( buffer[10] + FrameDecorationSize ) &&
								  buffer[10] <= MaximalFramePayloadSize &&
								  _pImpl->observer )
							{
								// Copy data to a frame and return that to the observer.
								Frame data( _pImpl->filterAddress , static_cast<size_t>( buffer[10] ) );
								memcpy( data.payloadBytes() ,
										 buffer + MeshSimulator::MessageHeaderSize + FrameHeaderSize , buffer[10] );
								_pImpl->observer->onReceive( data );
							}
							break;

						// It is on the indication channel, so it may be an ack or nack.
						case MeshSimulator::SimulationIndicationCI:
							_pImpl->busyMutex.lock();
							_pImpl->busy = false;
							_pImpl->busyMutex.unlock();
							if ( frameSize == 1 && _pImpl->observer )
							{
								// Give status further to the observer.
								if ( _pImpl->_autoAck )
								{
									if ( buffer[MeshSimulator::MessageCommandOffset] == MeshSimulator::AckSIT )
										_pImpl->observer->onSendStatus( IPhyObserver::Delivered );
									else if ( buffer[MeshSimulator::MessageCommandOffset] == MeshSimulator::NackSIT )
										_pImpl->observer->onSendStatus( IPhyObserver::NotDelivered );
								}
								else
								{
									_pImpl->observer->onSendStatus( IPhyObserver::Sent );
								}
							}
							break;

						// It is on the Debug channel, so thread the debug messages.
						case MeshSimulator::DebugCommandCI:
							if ( frameSize == 1 )
							{
								switch( buffer[MeshSimulator::MessageCommandOffset] )
								{
									// Continue execution.
									case MeshSimulator::RunContDC:
										{
											__XF_PAUSED = false;
											IAir3TFactory & factory = MeshSimFactory::factory();
											( (MeshSimFactory &)factory ).boardWidget()->update();
										}
										break;

									// Pause execution.
									case MeshSimulator::PauseDC:
										{
											__XF_PAUSED = true;
											IAir3TFactory & factory = MeshSimFactory::factory();
											( (MeshSimFactory &)factory ).boardWidget()->update();
										}
										break;

									// Quit application.
									case MeshSimulator::QuitDC:
										_exit( 0 );

									// Identify.
									case MeshSimulator::IdentifyDC:
										{
											Trace::out( "It is me..." );
											IAir3TFactory & factory = MeshSimFactory::factory();
											QApplication::alert( ( (MeshSimFactory &)factory ).boardWidget() );
										}
										break;

									// We ignore StepDC and ResetDC commands.
									default:
										Trace::out( "Unknown Debug Command 0x%02X Received!" , buffer[5] );
										break;
								}
							}
							break;

						// We ignore the SimulationIndicationCI and NodePrivMessagesCI channels and the others are
						// only used in direction to the MeshSimulator and ignored too.
						default:
							Trace::out( "Channel identifier 0x%02X not supported!" , channelIdentifier );
							break;
					}
				}

				// Move to next message.
				bytesLeft -= ( frameSize + MeshSimulator::MessageHeaderSize );
				buffer += ( frameSize + MeshSimulator::MessageHeaderSize );
			}

			// Schedule already next reception if there was no error.
			_pImpl->socket.async_receive( boost::asio::buffer( _pImpl->rxBuffer , sizeof( _pImpl->rxBuffer ) ) ,
										  Private::onRead );
		}
		else
		{
			Trace::out( "Error while receiving, connection to MeshSimulator lost? " );
		}
	}

	IPhyObserver * observer;				// The PHY observer.
	boost::thread thread;					// Thread used for reception and event handling.
	boost::asio::io_service io_service;		// The IO service that powers the TCP socket.
	tcp::socket socket;						// The TCP socket.
	uint8_t rxBuffer[64];					// Reception buffer.
	FilterAddress filterAddress;			// Reception channel address.
	RadioChannel channel;					// The actual used radio channel.
	bool busy;								// Is the PHY busy sending?
	boost::mutex busyMutex;					// Mutex protecting the busy boolean?
	Mode _mode;								// The mode the PHY actually is.
	bool _autoAck;							// Are the frames acknowledged by the phy?
};

MeshSimTransceiver::Private * MeshSimTransceiver::_pImpl = NULL;


MeshSimTransceiver::MeshSimTransceiver()
{}

MeshSimTransceiver::~MeshSimTransceiver()
{}

bool MeshSimTransceiver::initialize()
{
	// If there is no instance, create one.
	if ( !_pImpl ) _pImpl = new Private();
	assert( _pImpl );

	// Initialize only once!
	if ( _pImpl->socket.is_open() ) return true;

	// Prepare set name setup command.
	uint32_t size = strlen( __MESH_NAME ) + 1;
	uint8_t * setNameCommand = new uint8_t[size +
											MeshSimulator::MessageHeaderSize +
											MeshSimulator::MessageCommandSize];

	try
	{
		// Try to connect to the MeshSimulator.
		_pImpl->socket.connect( tcp::endpoint( boost::asio::ip::address::from_string( __MESH_SERVER ) ,
							    			   __MESH_PORT ) );

		// The very fist command in Name ID mode is to transmit the node's name.
		setNameCommand[MeshSimulator::MessageChannelIdOffset] = MeshSimulator::SimulationControlCI;
		setNameCommand[MeshSimulator::MessageCommandOffset] = MeshSimulator::ChangeNameSCC;
		size = htonl( size );
		memcpy( setNameCommand + MeshSimulator::MessageLenghtOffset , &size ,
				MeshSimulator::MessageLengthSize );
		size = htonl( size );
		memcpy( setNameCommand + MeshSimulator::MessageHeaderSize + MeshSimulator::MessageCommandSize ,
				__MESH_NAME , size );

		// Send the command (This is always the first thing to do to work with MeshSimulator).
		if ( _pImpl->socket.send( boost::asio::buffer( setNameCommand ,
								  size + MeshSimulator::MessageHeaderSize + MeshSimulator::MessageCommandSize ) ) ==
								  size + MeshSimulator::MessageHeaderSize + MeshSimulator::MessageCommandSize )
		{
			// Set the node to active!
			setMode( Active );

			// Create and start thread that handles the async operations.
			_pImpl->thread = boost::thread( boost::bind( &boost::asio::io_service::run , &_pImpl->io_service ) );

			// Start asynchronous reception.
			_pImpl->socket.async_receive( boost::asio::buffer( _pImpl->rxBuffer , sizeof( _pImpl->rxBuffer ) ) ,
										  Private::onRead );

			// Ok, all went fine :)
			delete[] setNameCommand;
			usleep( 10000 );
			return true;
		}
	}
	catch ( ... ) {}

	// Something did not worked as expected...
	Trace::out( "Could not initialize Simulated NRF Transceiver, is MeshSimulator running and listening to port %i ?" ,
			    __MESH_PORT );
	delete[] setNameCommand;
	return false;
}

MeshSimTransceiver::Mode MeshSimTransceiver::mode() const
{
	if ( _pImpl && _pImpl->socket.is_open() )
	{
		return _pImpl->_mode;
	}
	else
	{
		return Undefined;
	}
}
bool MeshSimTransceiver::setMode( Mode mode )
{
	// Send message to activate the node inside MeshSimulator.
	if ( _pImpl && _pImpl->socket.is_open() )
	{
		static uint8_t message[6] = { MeshSimulator::SimulationControlCI , 0 , 0 , 0 , 1 ,
									  MeshSimulator::DisableNodeSCC };

		_pImpl->_mode = mode;
		if ( mode == Active ) message[5] = MeshSimulator::EnableNodeSCC;
		else message[5] = MeshSimulator::DisableNodeSCC;

		try
		{
			if ( _pImpl->socket.send( boost::asio::buffer( message , 6 ) ) == 6 )
			{
				return true;
			}
		}
		catch ( ... ) {}
	}

	Trace::out( "Could not change transceiver mode! Check connection to MeshSimulator." );
	return false;
}

IPhyTransceiver::RadioChannel MeshSimTransceiver::radioChannel() const
{
	if ( _pImpl )
		return _pImpl->channel;
	else
		return static_cast<RadioChannel>( -1 );
}

bool MeshSimTransceiver::setRadioChannel( RadioChannel channel )
{
	if ( _pImpl )
	{
		_pImpl->channel = channel;
		return true;
	}
	else
		return false;
}

IPhyTransceiver::FilterAddress MeshSimTransceiver::receptionFilterAddress() const
{
	if ( _pImpl )
		return _pImpl->filterAddress;
	else
		return FilterAddress();
}

bool MeshSimTransceiver::setReceptionFilterAddress( const IPhyTransceiver::FilterAddress & address )
{
	if ( _pImpl && _pImpl->socket.is_open() )
	{
		uint8_t setMacAddressCommand[12] = { MeshSimulator::SimulationControlCI , 0 , 0 , 0 , 5 ,
										     MeshSimulator::SetMacAddressSCC , 0 , 0 , 0 , 0 };

		_pImpl->filterAddress = address;
		address.copyTo( setMacAddressCommand + 6 );

		try
		{
			// Send the command (This is always the first thing to do to work with MeshSimulator).
			if ( _pImpl->socket.send( boost::asio::buffer( setMacAddressCommand , 10 ) ) == 10 )
				return true;
		} catch ( ... ) {}
	}

	return false;
}

bool MeshSimTransceiver::autoAck() const
{
	if ( _pImpl )
		return _pImpl->_autoAck;
	else
		return false;
}

bool MeshSimTransceiver::setAutoAck( bool autoAck )
{
	if ( _pImpl )
	{
		_pImpl->_autoAck = autoAck;
		return true;
	}
	else
	{
		return false;
	}
}

bool MeshSimTransceiver::setObserver( IPhyObserver * observer )
{
	// If there was an set an observer already, fail. Otherwise set the observer and return true.
	if ( _pImpl )
	{
		_pImpl->observer = observer;
		return observer;
	}
	else
	{
		Trace::out( "There is already an observer registered, ignoring the new one!" );
		return false;
	}
}

bool MeshSimTransceiver::send( const Frame & frame )
{
	// If the frame is to big, fail.
	if ( !_pImpl || frame.payloadSize() == 0 || frame.payloadSize() > 32 ) return false;

	// Is the transceiver busy?
	_pImpl->busyMutex.lock();
	bool busy = _pImpl->busy;
	if ( busy )
	{
		_pImpl->busyMutex.unlock();
		return false;
	}

	// Construct message to send...
	uint32_t frameSize = frame.payloadSize() + 9;
	std::vector<uint8_t> actualFrame( frameSize + 5 , 0 );
	actualFrame[MeshSimulator::MessageChannelIdOffset] = MeshSimulator::FrameDataCI;
	frameSize = htonl( frameSize );
	for ( int i = 0 ; i < 4 ; i++ ) actualFrame[i + 1] = *( (uint8_t *)&frameSize + i );

	// Preamble:
	if ( frame.address()[0] & 0x80 )
		actualFrame[MeshSimulator::MessageHeaderSize] = 0xAA;
	else
		actualFrame[MeshSimulator::MessageHeaderSize] = 0x55;

	// Address:
	frame.address().copyTo( actualFrame.data() + 6 );

	// Packet control field:
	actualFrame[10] = frame.payloadSize();
	actualFrame[11] = 0;

	// Payload:
	memcpy( actualFrame.data() + 12 , frame.payloadBytes() , frame.payloadSize() );

	// Checksum:
	uint16_t cs = qChecksum( reinterpret_cast<char *>( actualFrame.data() + 6 ) , frame.payloadSize() + 6 );
	for ( int i = 0 ; i < 2 ; i++ ) actualFrame[i + frame.payloadSize() + 12] = *( (uint8_t *)&cs + i );

	// Send message to MeshSimulator.
	try
	{
		if ( _pImpl->socket.send( boost::asio::buffer( actualFrame ) ) == actualFrame.size() )
		{
			_pImpl->busy = true;
			_pImpl->busyMutex.unlock();
			return true;
		}
	} catch ( ... ) {}

	Trace::out( "Could not send frame! Check connection to MeshSimulator." );

	_pImpl->busyMutex.unlock();
	if ( _pImpl->observer ) _pImpl->observer->onSendStatus( IPhyObserver::NotDelivered );
	return false;
}
