#pragma once
/**
 * @file DataLink.hpp
 * @date fall 2012 - 18.01.2013
 * @author Christian Muller & Romain Maffina
 */

#include <xf/xf.h>
#include <xf/xfevent.h>
#include <xf/xfreactive.h>
#include <xf/xfstaticevent.h>
#include <trace/trace.h>

#include <interfaces/IAir3TFactory>
#include <interfaces/IPhyObserver>
#include <interfaces/IDataLinkObserver>
#include <interfaces/IPhyTransceiver>

#include <desenet/datalink/Node>
#include <desenet/datalink/DataPdu>
#include <desenet/datalink/AdvPdu>
#include <desenet/datalink/Common>

using namespace std;
using namespace datalink;

/**
 * @brief Represent the DataLink layer in the desenet network stack.
 */
class DataLink : public XFReactive, public IPhyObserver {
private:
	class StartAdvertiseRequestEvent : 		public XFStaticEvent { public: static const int Id = 10; StartAdvertiseRequestEvent() : 	XFStaticEvent( Id ) {} };
	class StopAdvertiseRequestEvent	:		public XFStaticEvent { public: static const int Id = 11; StopAdvertiseRequestEvent() : 		XFStaticEvent( Id ) {} };
	class ConnectionIndicationEvent : 		public XFStaticEvent { public: static const int Id = 12; ConnectionIndicationEvent() : 		XFStaticEvent( Id ) {} };

	class ConnectionRequestEvent : 			public XFStaticEvent { public: static const int Id = 20; ConnectionRequestEvent() : 		XFStaticEvent( Id ) {} };
	class ConnectionConfirmationEvent : 	public XFStaticEvent { public: static const int Id = 21; ConnectionConfirmationEvent() : 	XFStaticEvent( Id ) {} };
	class DataIndicationEvent : 			public XFStaticEvent { public: static const int Id = 22; DataIndicationEvent() : 			XFStaticEvent( Id ) {} };

	class FrameDeliveredEvent : 			public XFStaticEvent { public: static const int Id = 23; FrameDeliveredEvent() : 			XFStaticEvent( Id ) {} };
	class FrameNotDeliveredEvent : 			public XFStaticEvent { public: static const int Id = 24; FrameNotDeliveredEvent() : 		XFStaticEvent( Id ) {} };

	class EstablishConnectionTimeout		{ public: static const int Id = 30; };
	class AdvertiseIntervalTimeout			{ public: static const int Id = 31; };
	class ConnectionPduMaxTimeout			{ public: static const int Id = 32; };
	class ConnectionIntervalTimeout			{ public: static const int Id = 33; };
	class WaitNullTransition				{ public: static const int Id = 40; };

	queue<Frame> rxQueue, txQueue;
	IPhyTransceiver *_transceiver;
	IDataLinkObserver *_observer;
	Node *_nodePeer, *_nodeLocal;
	uint8_t *_advDesc;
	uint8_t advRadioChannel;
	Frame::FrameAddress advAccessAddress;

	static const uint8_t DISCONNECT_REQUEST = 0xFF;


public:

	DataLink() : 	_state( Initialize ),
					_advertiseSubstate( AdvertiseInitialize ) ,
					_establishConnectionSubstate( EstablishConnectionInitialize ) ,
					_connectedSubstate( ConnectedInitialize ),
					advRadioChannel( 1 ),
					advAccessAddress(Frame::FrameAddress::fromHexString("8E89BED6") ){
	}

	/**
	 * @brief Initialize the datalink
	 * @param transceiver the underlying tranceiver for sending data
	 */
	bool initialize( IPhyTransceiver & transceiver , Node::NodeId DataLinkId ) {
		_transceiver = &transceiver;
		_transceiver->setObserver(this);

		_nodeLocal = new Node(Node::NodeId::fromHexString("000000000002"));
		startBehavior();
		return true;
	}

	/**
	 * @brief Allow to set observer of the DL (that implement IDataLinkObserver) to then call his callbacks.
	 * @param observer a pointer to the observer
	 */
	bool setObserver(IDataLinkObserver * observer) {
		// If there was an set an observer already, fail. Otherwise set the observer and return true.
		if (_observer == NULL) {
			_observer = observer;
			return true;
		} else {
			Trace::out(
					"There is already an observer registered, ignoring the new one!");
			return false;
		}
	}


	/**
	 * @brief Start advertising
	 * @param pointer to the description to send
	 *
	 * - Send own description to other nodes in the network
	 * - Discover other nodes in the network
	 */
	void startAdvertiseRequest(uint8_t *desc) {
		_advDesc = desc;
		static StartAdvertiseRequestEvent event;
		pushEvent(&event);
	}

	/**
	 * @brief Stop advertising.
	 */
	void stopAdvertiseRequest() {
		static StopAdvertiseRequestEvent event;
		pushEvent(&event);
	}

	/**
	 * @brief Try to establish a connection with a specific node
	 * @param nodeId the node to connect with
	 * @param connectionInterval Time (of sleep) between active communication phases
	 */
	void connectRequest(Node::NodeId nodeId, int connectionInterval /*, void *serviceReference*/) {
		static ConnectionRequestEvent event;
		pushEvent(&event);

		_nodePeer = new Node(nodeId);
	}

	/**
	 * @brief Disconnect from current connected node (if any)
	 */
	void disconnectRequest() {
		//TODO
	}

	/**
	 * @brief Sent a data using the DL
	 * @param dataPdu the data to send
	 */
	void dataRequest(DataPdu &dataPdu) {
		Frame *frame;

		int i;
		// for each data byte
		for (i = 0; i < dataPdu.getPayloadSize(); i++) {
			// if we have 32 bytes, send

			if (i % 32) {
				frame = new Frame(_nodePeer->getAddress(),
								dataPdu.getPayloadBytes(),
								dataPdu.getPayloadSize() + 6);
				txQueue.push(*frame);
			}
		}

		// if we have data left (not full payload frame)
		if (dataPdu.getPayloadSize() % 32 != 0) {
			// send left data
			frame = new Frame(_nodePeer->getAddress(),
					dataPdu.getPayloadBytes(), dataPdu.getPayloadSize() + 6);
			txQueue.push(*frame);
		}
	}

private:
	enum { Initialize , Idle , Advertise , EstablishConnection , AcceptConnection , Connected } _state , _oldState;
	enum { AdvertiseInitialize , SendAdvertise , WaitForAdvertiseData , ProcessAdvertiseData } _advertiseSubstate , _oldAdvertiseSubstate;
	enum { EstablishConnectionInitialize, WaitForConnectionData , ProcessConnectionData } _establishConnectionSubstate , _oldEstablishConnectionSubstate;
	enum { ConnectedInitialize , SendQueuedDataPdu , WaitDataPdu , ProcessDataPdu , CloseConnection , WaitForNextConnectionEvent , EnableTransceiver } _connectedSubstate , _oldConnectedSubstate;

	/**
	 * @enum Represents the roles that a node can take
	 */
	enum PeerRole {
		MASTER, /**< Master role : the node has  initiated the connection */
		SLAVE  /**< Slave role : the node has accepted the connection */
	};

	PeerRole role;

	static const int ESTABLISH_CONNECTION_TIMEOUT = 1000; 	/**< Max time to wait for the connection to establish */
	static const int ADVERTISE_INTERVAL_TIMEOUT = 500; 		/**< Interval of advertising */
	static const int CONNECTION_PDU_MAX_TIMEOUT = 100;		/**< Max time to wait for data to arrive */
	static const int CONNECTION_INTERVAL_TIMEOUT = 1000;	/**< Time of sleep between to active communication phase */

	/**
	 * @brief This function is called by the XF when an event must be handled.
	 */
	EventStatus processEvent() {

		IXFEvent *e = getCurrentEvent();
		_oldState = _state;
		_oldAdvertiseSubstate = _advertiseSubstate;
		_oldEstablishConnectionSubstate = _establishConnectionSubstate;
		_oldConnectedSubstate = _connectedSubstate;


		switch( _state ) {
		case Initialize:
			if ( e->getEventType() == XFEvent::Initial )
				_state = Idle;
			break;

		case Idle:
			if ( e->getEventType() == XFEvent::Event && e->getId() == StartAdvertiseRequestEvent::Id ){
				_state = Advertise;
				_advertiseSubstate = WaitForAdvertiseData;
			}

			break;

		case Advertise:
			SubMachineAdvertise(e);

			if ( e->getEventType() == XFEvent::Event && e->getId() == ConnectionRequestEvent::Id )
				_state = EstablishConnection;
			else if ( e->getEventType() == XFEvent::Event && e->getId() == ConnectionIndicationEvent::Id )
				_state = AcceptConnection;
			else if ( e->getEventType() == XFEvent::Event && e->getId() == StopAdvertiseRequestEvent::Id )
				_state = Idle;
			break;

		case EstablishConnection:
			SubMachineEstablishConnection(e);

			// if we request the connection, we are master
			role = MASTER;

			if ( e->getEventType() == XFEvent::Timeout && e->getId() == EstablishConnectionTimeout::Id ) // timeout
				_state = Advertise;
			else if ( e->getEventType() == XFEvent::Event && e->getId() == ConnectionConfirmationEvent::Id )
				_state = Connected;
			break;

		case AcceptConnection:
			//TODO Send accept connection data
			// create AdvPdu with type Connect
			// create frame with AdvPdu
			// send frame with transceiver
			// get ack from phy layer about

			// if we didn't requested the connection, we are slave
			role = SLAVE;

			_state = Connected;
			break;

		case Connected:
			// call the "Connected" submachine and test if it returns a disconnection
			switch( SubMachineConnected(e) ) {
			case LINK_LOSS: // in case of link loss, restart connection
				_state = Advertise;
				break;

			case DISCONNECT: // if peer (or local) asked for disconnection, go idle
				_state = Idle;
				break;

			default:
				break;
			}
			break;
		}


		/* MEALY
		 * Some actions requires to be done only on state transition,
		 * and not when staying in same state.
		 */

		/* Action on entry */
		if( _state != _oldState ) {
			switch( _state ) {
			case Initialize:
				Trace::out("->[Initialize global state machine]");
				break;

			case Idle:
				Trace::out("->[Idle]");
				break;

			case Advertise:
				Trace::out("->[Advertise]");
				break;

			case EstablishConnection:
				Trace::out("->[EstablishConnection]");
				getThread()->scheduleTimeout(EstablishConnectionTimeout::Id, ESTABLISH_CONNECTION_TIMEOUT, this);
				break;

			case AcceptConnection:
				Trace::out("->[AcceptConnection]");
				pushEvent( new XFNullTransition( WaitNullTransition::Id ) );
				break;

			case Connected:
				Trace::out("->[Connected]");
				break;

			default:
				Trace::out("->[undefined state error]");
				break;
			}
		}

		if( _advertiseSubstate != _oldAdvertiseSubstate ) {
			switch( _advertiseSubstate ) {
			case AdvertiseInitialize:
				Trace::out("->[Initialize advertisement phase state machine]");
				break;

			case WaitForAdvertiseData:
				Trace::out("\t- WaitForAdvertiseData");
				getThread()->scheduleTimeout(AdvertiseIntervalTimeout::Id, ADVERTISE_INTERVAL_TIMEOUT, this);
				break;

			case SendAdvertise:
				Trace::out("\t- sending my advertise");
				pushEvent( new XFNullTransition( WaitNullTransition::Id ) );
				break;

			case ProcessAdvertiseData:
				Trace::out("\t- processing advertise received");
				pushEvent( new XFNullTransition( WaitNullTransition::Id ) );
				break;

			default:
				Trace::out("\t- undefined state error");
				break;
			}
		}

		if( _establishConnectionSubstate != _oldEstablishConnectionSubstate ) {
			switch( _establishConnectionSubstate ) {
			case EstablishConnectionInitialize:
				Trace::out("->[Initialize connection establishment state machine]");
				break;

			case WaitForConnectionData:
				Trace::out("\t- wait for connection data");
				break;

			case ProcessConnectionData:
				Trace::out("\t- process connection data");
				pushEvent( new XFNullTransition( WaitNullTransition::Id ) );
				break;

			default:
				Trace::out("\t- undefined state error");
				break;
			}
		}

		if( _connectedSubstate != _oldConnectedSubstate ) {
			switch( _connectedSubstate ) {
			case ConnectedInitialize:
				Trace::out("\t- initialize data transfer (alias 'connected') state machine");
				break;

			case SendQueuedDataPdu:
				Trace::out("\t- send queued data pdu");
				if(!txQueue.empty()) {
					_transceiver->send(txQueue.front());
					txQueue.pop();
				}
				break;

			case WaitDataPdu:
				Trace::out("\t- wait data pdu");
				getThread()->scheduleTimeout(ConnectionPduMaxTimeout::Id, CONNECTION_PDU_MAX_TIMEOUT, this);
				break;

			case ProcessDataPdu:
				Trace::out("\t- process data pdu");
				pushEvent( new XFNullTransition( WaitNullTransition::Id ) );
				break;

			case CloseConnection:
				Trace::out("\t- close connection");
				break;

			case WaitForNextConnectionEvent:
				Trace::out("\t- wait for connection");
				getThread()->scheduleTimeout(ConnectionIntervalTimeout::Id, CONNECTION_INTERVAL_TIMEOUT, this);
				break;

			case EnableTransceiver:
				Trace::out("\t- enable transceiver");
				break;

			default:
				Trace::out("\t- undefined state error");
				break;
			}
		}

		/* Actions on exit */
		if( _oldState != _state) {
			switch( _oldState ) {
			case EstablishConnection:
				getThread()->unscheduleTimeout(EstablishConnectionTimeout::Id, this);
				break;

			default:
				break;
			}
		}

		if( _oldAdvertiseSubstate != _advertiseSubstate ) {
			switch( _oldAdvertiseSubstate ) {
			case WaitForAdvertiseData:
				getThread()->unscheduleTimeout(AdvertiseIntervalTimeout::Id, this);
				break;

			default:
				break;
			}
		}

		if( _oldConnectedSubstate != _connectedSubstate) {
			switch( _oldConnectedSubstate ) {
			case WaitDataPdu:
				getThread()->unscheduleTimeout(ConnectionPduMaxTimeout::Id, this);
				break;

			case WaitForNextConnectionEvent:
				getThread()->unscheduleTimeout(ConnectionIntervalTimeout::Id, this);
				break;

			default:
				break;
			}
		}
		return EventStatus::Consumed; //TODO : not necessary consumed...
	}

	/*
	 * @brief This submachine contains the state machine for the main AVERTISE state.
	 * @return the cause of disconnection
	 */
	void SubMachineAdvertise(IXFEvent *e) {

		AdvPdu *advPdu;
		Frame *frame;
		switch(_advertiseSubstate) {
		case AdvertiseInitialize:
			_advertiseSubstate = SendAdvertise;
			break;

		case SendAdvertise:
			// Activate transceiver
			Trace::out("\t-Activate transceiver");
			_transceiver->setMode(IPhyTransceiver::Active);

			Trace::out("\t-Send advertise");
			advPdu = new AdvPdu(_nodeLocal->id(), (uint8_t*) _advDesc);
			frame = new Frame(advAccessAddress, advPdu, advPdu->size());
			//frame = new Frame(advAccessAddress, "0000002AAAA", 11); // this is for tests
			_transceiver->send(*frame);
			delete frame;

			_advertiseSubstate = WaitForAdvertiseData;
			break;

		case WaitForAdvertiseData:

			if ( e->getEventType() == XFEvent::Timeout && e->getId() == AdvertiseIntervalTimeout::Id ){
				Trace::out("\t-Timeout");
				_advertiseSubstate = SendAdvertise;
			} else if ( e->getEventType() == XFEvent::Event && e->getId() == DataIndicationEvent::Id ) {
				Trace::out("\t-Advertise received");
				_advertiseSubstate = ProcessAdvertiseData;
			}
			break;

		case ProcessAdvertiseData:
			Trace::out("\t-Process advertise");
			advPdu = (AdvPdu*)rxQueue.front().payloadBytes();
			rxQueue.pop();

			// test if frame is an advertising frame
			_advertiseSubstate = WaitForAdvertiseData;
			break;

		}

	}

/*
 * We don't use this since both advertising substate machines have been merged
 */
//	void ReceiveAdvertise(IXFEvent *e) {
//		case WaitForAdvertiseData:
//
//			_receiveAdvertiseSubstate = ProcessAdvertiseData;
//			break;
//
//		case ProcessAdvertiseData:
//
//			currentState = WaitForAdvertiseData;
//			return;
//	}

	/*
	 * @brief This submachine contains the state machine for the main ESTABLISH_CONNECTION state.
	 * @return the cause of disconnection
	 */
	void SubMachineEstablishConnection(IXFEvent *e) {

		switch(_establishConnectionSubstate) {
		case EstablishConnectionInitialize:
			_establishConnectionSubstate = WaitForConnectionData;
			break;

		case WaitForConnectionData:
			Trace::out("\t-Wait for connection data");
			if ( e->getEventType() == XFEvent::Event && e->getId() == DataIndicationEvent::Id )
				// check if frame is connection data, if not : break
				Trace::out("\t-Connection data received");
				_establishConnectionSubstate = ProcessConnectionData;
			break;

		case ProcessConnectionData:
			// process connection data
			Trace::out("\t-Process connections data");
			_establishConnectionSubstate = WaitForConnectionData;
			break;
		}
	}

	/*
	 * @brief This submachine contains the state machine for the main CONNECTED state.
	 * @return the cause of disconnection
	 */
	DisconnectIndicationCause SubMachineConnected(IXFEvent *e) {

		DataPdu *dataPdu;
		DisconnectIndicationCause cause;

		switch(_connectedSubstate) {
		case ConnectedInitialize:
			// first state is not same for master and slave
			switch(role){
			case MASTER:
				_connectedSubstate = SendQueuedDataPdu;
				break;

			case SLAVE:
				_connectedSubstate = WaitDataPdu;
				break;
			}

		case SendQueuedDataPdu:
			// send all data in queue
			if(!txQueue.empty()) {
				Trace::out("->[Send data pdu]");
				dataPdu = (DataPdu *) txQueue.front().payloadBytes();
				_transceiver->send(txQueue.front());
				// get data off the queue
				txQueue.pop();
			}

			if ( e->getEventType() == XFEvent::Event && e->getId() == FrameNotDeliveredEvent::Id ) {
				Trace::out("->[Frame not delivered]");
				break;
			} else if ( e->getEventType() == XFEvent::Event && e->getId() == FrameDeliveredEvent::Id ) {

				if (*dataPdu->getPayloadBytes() == DISCONNECT_REQUEST) { // if it was a disconnection request, disconnect
					_connectedSubstate = CloseConnection;
					cause = DISCONNECT;
				} else if (!dataPdu->isMoreData()) { // No more datas (MMF==0 && SMF==0)
					_connectedSubstate = WaitForNextConnectionEvent;
				} else {
					_connectedSubstate = WaitDataPdu;
				}
			}
			break;


		case WaitDataPdu:
			if ( e->getEventType() == XFEvent::Timeout && e->getId() == ConnectionPduMaxTimeout::Id ) {
				_connectedSubstate = CloseConnection;
				cause = LINK_LOSS;
			} else if ( e->getEventType() == XFEvent::Event && e->getId() == DataIndicationEvent::Id ) {
				_connectedSubstate = ProcessDataPdu;
			}
			break;


		case ProcessDataPdu:
			Frame *frame;
			// If there is a frame in the queue
			if(rxQueue.empty())
				break;

			// Get the frame
			*frame = rxQueue.front();
			rxQueue.pop();

			// Extract the data pdu
			DataPdu *dataPdu;
			*dataPdu = *((DataPdu*)(frame->payloadBytes()));

			// Check the data pdu informations
			if (*dataPdu->getPayloadBytes() == DISCONNECT_REQUEST) { // disconnection
				_connectedSubstate = CloseConnection;
				cause = DISCONNECT;
			} else if (!dataPdu->isMoreData()) { // No more datas (MMF==0 && SMF==0)
				_connectedSubstate = WaitForNextConnectionEvent;
			} else {
				_connectedSubstate = SendQueuedDataPdu;
			}
			break;

		case CloseConnection:
			Trace::out("\t-Disconnection");
			// send disconnection indication to upper layer
			_observer->onDisconnectIndication(DISCONNECT);
			return cause;
			break;

		case WaitForNextConnectionEvent:
			// Disable transceiver
			_transceiver->setMode(IPhyTransceiver::Inactive);
			Trace::out("\t-Disable transceiver");

			// Set timer until next transceiver activation
			if ( e->getEventType() == XFEvent::Timeout && e->getId() == ConnectionIntervalTimeout::Id )
				_connectedSubstate = EnableTransceiver;
			break;

		case EnableTransceiver:
			// Enable transceiver
			Trace::out("\t-Enable transceiver");
			_transceiver->setMode(IPhyTransceiver::Active);
			break;
		}
		return NO_DISCONNECT;
	}


	/* DataLink primitives for PHY layer */

	/**
	 * @brief Add data received from PHY layer and raise data indication event
	 * @param pdu the frame received
	 */
	void onReceive( const Frame &pdu ) {
		rxQueue.push(pdu);
		pushEvent( new XFStaticEvent( DataIndicationEvent::Id ) );
	}

	/**
	 * @brief Callback called by the PHY layer when a frame has been treated.
	 * @param status the status of the treated frame.
	 *
	 * Rises event depending on frame send status
	 */
	void onSendStatus( SendStatus status ) {
		switch( status ) {
		case Delivered:
			pushEvent( new XFStaticEvent( FrameDeliveredEvent::Id ) );
			break;

		case NotDelivered:
			pushEvent( new XFStaticEvent( FrameNotDeliveredEvent::Id ) );
			break;

		case Sent:
			Trace::out("\t- SendStatus = Sent");
			break;

		case InvalidStatus:
			Trace::out("\t- SendStatus = InvalidStatus");
			pushEvent( new XFStaticEvent( FrameNotDeliveredEvent::Id ) );
			break;
		}
	}

};


