/*
 * DataLink.cpp
 *
 *  Created on: Dec 3, 2012
 *      Author: desem
 */

#include <xf/xf.h>
#include <xf/xfevent.h>
#include <xf/xfreactive.h>
#include <xf/xfstaticevent.h>
#include <IAir3TFactory>
#include <trace/trace.h>
#include <desenet/datalink/Node.hpp>
#include <interfaces/iphyobserver.h>

#include <types.h>

using namespace std;

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
	IPhyTransceiver *transceiver;


public:
	DataLink() : 	_state( Initialize ) ,
					_advertiseSubstate( AdvertiseInitialize ) ,
					_establishConnectionSubstate( EstablishConnectionInitialize ) ,
					_connectedSubstate( ConnectedInitialize ) {
		startBehavior();

	}

	bool initialize( IPhyTransceiver & transceiver , Node::NodeId DataLinkId ){
		this->transceiver = &transceiver; //FIXME peut être faux
		transceiver.setObserver(this);
		return true;
	}

	void advertiseStart(uint8_t *desc) 		{ static StartAdvertiseRequestEvent event; 	pushEvent( &event );}
	void advertiseStop() 					{ static StopAdvertiseRequestEvent event; 	pushEvent( &event );}
	void connectRequest(Node *node, int connectionInterval, void *serviceReference)
											{ static ConnectionRequestEvent event; 		pushEvent( &event );}
	void disconnectRequest() 				{ static ConnectionRequestEvent event; 		pushEvent( &event );}

	void dataRequest( DataPdu *data ) 		{
		Frame frame;
		frame = Frame();
		txQueue.push();
	}

private:
	enum { Initialize , Idle , Advertise , EstablishConnection , AcceptConnection , Connected } _state , _oldState;
	enum { AdvertiseInitialize , SendAdvertise , WaitForAdvertiseData , ProcessAdvertiseData } _advertiseSubstate , _oldAdvertiseSubstate;
	enum { EstablishConnectionInitialize, WaitForConnectionData , ProcessConnectionData } _establishConnectionSubstate , _oldEstablishConnectionSubstate;
	enum { ConnectedInitialize , SendQueuedDataPdu , WaitDataPdu , ProcessDataPdu , CloseConnection , WaitForNextConnectionEvent , EnableTransceiver } _connectedSubstate , _oldConnectedSubstate;

	enum PeerRole { MASTER , SLAVE };
	enum ConnectionStatus { CONNECTED , LINK_LOSS , PEER_DISCONNECT };

	PeerRole role;

	static const int ESTABLISH_CONNECTION_TIMEOUT = 1000;
	static const int ADVERTISE_INTERVAL_TIMEOUT = 500;
	static const int CONNECTION_PDU_MAX_TIMEOUT = 100;
	static const int CONNECTION_INTERVAL_TIMEOUT = 1000;

	EventStatus processEvent() {
		Trace::out("# Event!");

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
			role = MASTER;

			if ( e->getEventType() == XFEvent::Timeout && e->getId() == EstablishConnectionTimeout::Id ) // timeout
				_state = Advertise;
			else if ( e->getEventType() == XFEvent::Event && e->getId() == ConnectionConfirmationEvent::Id )
				_state = Connected;
			break;

		case AcceptConnection:
			//TODO envoi data
			role = SLAVE;

			_state = Connected;
			break;

		case Connected:
			switch( SubMachineConnected(e) ) {
			case LINK_LOSS:
				_state = Advertise;
				break;

			case PEER_DISCONNECT:
				_state = Idle;
				break;

			default:
				break;
			}
			break;
		}




		/* MEALY */

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
				Trace::out("\t->[undefined state error]");
				break;
			}
		}

		if( _advertiseSubstate != _oldAdvertiseSubstate ) {
			switch( _advertiseSubstate ) {
			case AdvertiseInitialize:
				Trace::out("->[Initialize advertisement phase state machine]");
				break;

			case WaitForAdvertiseData:
				Trace::out("\t->[WaitForAdvertiseData]");
				getThread()->scheduleTimeout(AdvertiseIntervalTimeout::Id, ADVERTISE_INTERVAL_TIMEOUT, this);
				break;

			case SendAdvertise:
				Trace::out("\t- sending my advertise");
				pushEvent( new XFNullTransition( WaitNullTransition::Id ) );

			case ProcessAdvertiseData:
				Trace::out("\t- processing advertise received");
				pushEvent( new XFNullTransition( WaitNullTransition::Id ) );

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
	 * Advertise
	 *
	 */
	void SubMachineAdvertise(IXFEvent *e) {

		switch(_advertiseSubstate) {
		case AdvertiseInitialize:
			_advertiseSubstate = SendAdvertise;
			break;

		case SendAdvertise:
			//TODO send
			_advertiseSubstate = WaitForAdvertiseData;
			break;

		case WaitForAdvertiseData:

			if ( e->getEventType() == XFEvent::Timeout && e->getId() == AdvertiseIntervalTimeout::Id ){
				Trace::out("->[SendAdvertise]");
				_advertiseSubstate = SendAdvertise;
			}else if ( e->getEventType() == XFEvent::Event && e->getId() == DataIndicationEvent::Id )
				_advertiseSubstate = ProcessAdvertiseData;
			break;

		case ProcessAdvertiseData:
			//TODO process
			_advertiseSubstate = WaitForAdvertiseData;
			break;

		}

	}


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
	 * EstablishConnection
	 */
	void SubMachineEstablishConnection(IXFEvent *e) {

		switch(_establishConnectionSubstate) {
		case EstablishConnectionInitialize:
			_establishConnectionSubstate = WaitForConnectionData;
			break;

		case WaitForConnectionData:
			if ( e->getEventType() == XFEvent::Event && e->getId() == DataIndicationEvent::Id )
				_establishConnectionSubstate = ProcessConnectionData;
			break;

		case ProcessConnectionData:
			//TODO process
			_establishConnectionSubstate = WaitForConnectionData;
			break;
		}
	}

	/*
	 * Connected
	 */
	ConnectionStatus SubMachineConnected(IXFEvent *e) {

		switch(_connectedSubstate) {
		case ConnectedInitialize:
			switch(role){
			case MASTER:
				_connectedSubstate = SendQueuedDataPdu;
				break;

			case SLAVE:
				_connectedSubstate = WaitDataPdu;
				break;
			}

		case SendQueuedDataPdu:
			if(!txQueue.empty()) {
				transceiver->send(txQueue.front());
				txQueue.pop();
			}

			if ( e->getEventType() == XFEvent::Event && e->getId() == FrameNotDeliveredEvent::Id )
				break;
			else if ( e->getEventType() == XFEvent::Event && e->getId() == FrameDeliveredEvent::Id ) {
				if (1) // disconnection
					_connectedSubstate = CloseConnection;
				else if (1) // plus de data MMF=0 && SMF==0
					_connectedSubstate = WaitForNextConnectionEvent;
				else
					_connectedSubstate = WaitDataPdu;
			}
			break;


		case WaitDataPdu:
			if ( e->getEventType() == XFEvent::Timeout && e->getId() == ConnectionPduMaxTimeout::Id )
				_connectedSubstate = CloseConnection;
			else if ( e->getEventType() == XFEvent::Event && e->getId() == DataIndicationEvent::Id )
				_connectedSubstate = ProcessDataPdu;
			break;


		case ProcessDataPdu:
			//TODO process

			if (1) // disconnection
				_connectedSubstate = CloseConnection;
			else if (1) // plus de data MMF=0 && SMF==0
				_connectedSubstate = WaitForNextConnectionEvent;
			else
				_connectedSubstate = SendQueuedDataPdu;

			break;

		case CloseConnection:
			//TODO indique a la couche app la déco
			//DISCONNECT_IND(state)
			return LINK_LOSS;
			return PEER_DISCONNECT;
			break;

		case WaitForNextConnectionEvent:
			//TODO disable transceiver

			if ( e->getEventType() == XFEvent::Timeout && e->getId() == ConnectionIntervalTimeout::Id )
				_connectedSubstate = EnableTransceiver;
			break;

		case EnableTransceiver:
			break;
		}
		return CONNECTED;
	}

	void onReceive( const Frame &pdu ) {
		rxQueue.push(pdu);
		pushEvent( new XFStaticEvent( DataIndicationEvent::Id ) );
	}

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



//DataLink::DataLink() {
//	myPhyRTX = IAir3TFactory::factory().phyTransceiver();
//
//	myPhyRTX.setObserver(this);
//}



