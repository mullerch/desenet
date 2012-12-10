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

class DataLink : public XFReactive {
private:
	class StartAdvertiseRequestEvent : 		public XFStaticEvent { public: static const int Id = 10; StartAdvertiseRequestEvent() : 	XFStaticEvent( Id ) {} };
	class ConnectionIndicationEvent : 		public XFStaticEvent { public: static const int Id = 11; ConnectionIndicationEvent() : 		XFStaticEvent( Id ) {} };

	class ConnectionRequestEvent : 			public XFStaticEvent { public: static const int Id = 20; ConnectionRequestEvent() : 		XFStaticEvent( Id ) {} };
	class ConnectionConfirmationEvent : 	public XFStaticEvent { public: static const int Id = 21; ConnectionConfirmationEvent() : 	XFStaticEvent( Id ) {} };
	class DataIndicationEvent : 			public XFStaticEvent { public: static const int Id = 22; DataIndicationEvent() : 			XFStaticEvent( Id ) {} };

	class FrameDeliveredEvent : 			public XFStaticEvent { public: static const int Id = 23; FrameDeliveredEvent() : 			XFStaticEvent( Id ) {} };
	class FrameNotDeliveredEvent : 			public XFStaticEvent { public: static const int Id = 24; FrameNotDeliveredEvent() : 		XFStaticEvent( Id ) {} };

	class EstablishConnectionTimeout		{ public: static const int Id = 30; };
	class AdvertiseIntervalTimeout			{ public: static const int Id = 31; };
	class ConnectionPduMaxTimeout			{ public: static const int Id = 32; };
	class ConnectionIntervalTimeout			{ public: static const int Id = 33; };

public:
	DataLink() : 	_state( Initialize ) ,
					_advertiseSubstate( SendAdvertise ) ,
//					_receiveAdvertiseSubstate( WaitForAdvertiseData ) ,
					_establishConnectionSubstate( WaitForConnectionData ) ,
					_connectedSubstate( Initial ) {
		startBehavior();
	}

	void advertiseStart(void *desc) 		{ static StartAdvertiseRequestEvent event; 	pushEvent( &event );}
//	void advertiseStop() 					{ static disc? event; 	pushEvent( &event );}
	void connectRequest(void *peerHandle, int connectionInterval, void *serviceReference)
											{ static ConnectionRequestEvent event; 		pushEvent( &event );}
	void connectIndication( void *peerHandle, void *serviceReference ) {}
	void disconnectRequest() 				{ static ConnectionRequestEvent event; 		pushEvent( &event );}
	void disconnectIndication( void *cause ){}

//	void dataRequest( void * data) 			{ static SendQueuedDataPdu event; 			pushEvent( &event );}
	void dataIndication( void * data) 		{}
	void appear( void *peerHandle, void *peerDescription) {}
	void disappear( void *peerHandle )		{}

	void initialize() {

	}


private:
	enum { Initialize , Idle , Advertise , EstablishConnection , AcceptConnection , Connected } _state , _oldState;
	enum { SendAdvertise , WaitForAdvertiseData , ProcessAdvertiseData } _advertiseSubstate , _oldAdvertiseSubstate;
	enum { WaitForConnectionData , ProcessConnectionData } _establishConnectionSubstate , _oldEstablishConnectionSubstate;
	enum { Initial , SendQueuedDataPdu , WaitDataPdu , ProcessDataPdu , CloseConnection , WaitForNextConnectionEvent , EnableTransceiver } _connectedSubstate , _oldConnectedSubstate;

	enum PeerRole { MASTER , SLAVE };
	enum ConnectionStatus { CONNECTED , LINK_LOSS , PEER_DISCONNECT };

	PeerRole role;

	static const int ESTABLISH_CONNECTION_TIMEOUT = 1000;
	static const int ADVERTISE_INTERVAL_TIMEOUT = 500;
	static const int CONNECTION_PDU_MAX_TIMEOUT = 100;
	static const int CONNECTION_INTERVAL_TIMEOUT = 1000;

	EventStatus processEvent() {

		IXFEvent *e = getCurrentEvent();
		_oldState = _state;
		_advertiseSubstate = _oldAdvertiseSubstate;
//		_receiveAdvertiseSubstate = _oldReceiveAdvertiseSubstate;
		_establishConnectionSubstate = _oldEstablishConnectionSubstate;
		_connectedSubstate = _oldConnectedSubstate;


		switch( _state ) {
		case Initialize:
			if ( e->getEventType() == XFEvent::Initial )
				_state = Idle;
			break;

		case Idle:

			if ( e->getEventType() == XFEvent::Event && e->getId() == StartAdvertiseRequestEvent::Id )
				_state = Advertise;
			break;

		case Advertise:
			SubMachineAdvertise(e);

			if ( e->getEventType() == XFEvent::Event && e->getId() == ConnectionRequestEvent::Id )
				_state = EstablishConnection;
			else if ( e->getEventType() == XFEvent::Event && e->getId() == ConnectionIndicationEvent::Id )
				_state = AcceptConnection;
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
			case Idle:
				Trace::out("->[main state] Idle");
				break;

			case EstablishConnection:
				getThread()->scheduleTimeout(EstablishConnectionTimeout::Id, ESTABLISH_CONNECTION_TIMEOUT, this);
				break;

			default:
				break;
			}
		}

		if( _advertiseSubstate != _oldAdvertiseSubstate ) {
			switch( _advertiseSubstate ) {
			case WaitForAdvertiseData:
				getThread()->scheduleTimeout(AdvertiseIntervalTimeout::Id, ADVERTISE_INTERVAL_TIMEOUT, this);
				break;

			default:
				break;
			}
		}

		if( _connectedSubstate != _oldConnectedSubstate ) {
			switch( _connectedSubstate ) {
			case WaitDataPdu:
				getThread()->scheduleTimeout(ConnectionPduMaxTimeout::Id, CONNECTION_PDU_MAX_TIMEOUT, this);
				break;

			case WaitForNextConnectionEvent:
				getThread()->scheduleTimeout(ConnectionIntervalTimeout::Id, CONNECTION_INTERVAL_TIMEOUT, this);
				break;

			default:
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
	}

	/*
	 * Advertise
	 *
	 */
	void SubMachineAdvertise(IXFEvent *e) {

		switch(_advertiseSubstate) {
		case SendAdvertise:
			//TODO send
			_advertiseSubstate = WaitForAdvertiseData;
			break;

		case WaitForAdvertiseData:

			if ( e->getEventType() == XFEvent::Timeout && e->getId() == AdvertiseIntervalTimeout::Id )
				_advertiseSubstate = SendAdvertise;
			else if ( e->getEventType() == XFEvent::Event && e->getId() == DataIndicationEvent::Id )
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
		case Initial:
			switch(role){
			case MASTER:
				_connectedSubstate = SendQueuedDataPdu;
				break;

			case SLAVE:
				_connectedSubstate = WaitDataPdu;
				break;
			}

		case SendQueuedDataPdu:
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


};



//DataLink::DataLink() {
//	myPhyRTX = IAir3TFactory::factory().phyTransceiver();
//
//	myPhyRTX.setObserver(this);
//}



