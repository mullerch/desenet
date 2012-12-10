/*
 * DataLink.cpp
 *
 *  Created on: Dec 3, 2012
 *      Author: desem
 */

//#include <desenet/datalink/DataLink.h>
#include <xf/xf.h>
#include <xf/xfevent.h>
#include <xf/xfstaticevent.h>
#include <IAir3TFactory>
//#include <trace/Trace>

class DataLink : public XFReactive {
private:
	class StartAdvertiseRequestEvent : 		public XFStaticEvent { public: static const int Id = 10; StartAdvertiseRequestEvent() : 	XFStaticEvent( Id ) {} };
	class ConnectionIndicationEvent : 		public XFStaticEvent { public: static const int Id = 11; ConnectionIndicationEvent() : 		XFStaticEvent( Id ) {} };

	class ConnectionRequestEvent : 			public XFStaticEvent { public: static const int Id = 20; ConnectionRequestEvent() : 		XFStaticEvent( Id ) {} };
	class ConnectionConfirmationEvent : 	public XFStaticEvent { public: static const int Id = 21; ConnectionConfirmationEvent() : 	XFStaticEvent( Id ) {} };
	class DataIndicationEvent : 			public XFStaticEvent { public: static const int Id = 22; DataIndicationEvent() : 			XFStaticEvent( Id ) {} };

	class FrameDeliveredEvent : 			public XFStaticEvent { public: static const int Id = 23; FrameDeliveredEvent() : 			XFStaticEvent( Id ) {} };
	class FrameNotDeliveredEvent : 			public XFStaticEvent { public: static const int Id = 24; FrameNotDeliveredEvent() : 		XFStaticEvent( Id ) {} };


public:
	DataLink() : 	_state( InitialState ) ,
					_sendAdvertiseSubstate( SendAdvertise ) ,
					_receiveAdvertiseSubstate( WaitForAdvertiseData ) ,
					_establishConnectionSubstate( WaitForConnectionData ) ,
					_connectedSubstate( SendQueuedDataPdu )	{ //FIXME change inital state (dynamic)
		startBehavior();
	}

	void advertiseStart(void *desc) 		{ static StartAdvertiseRequestEvent event; 	pushEvent( &event );}
//	void advertiseStop() 					{ static disc? event; 	pushEvent( &event );}
	void connect(void *peerHandle, int connectionInterval, void *serviceReference)
											{ static ConnectionRequestEvent event; 		pushEvent( &event );}
//	void disconnect() 						{ static ??? event; 	pushEvent( &event );}
	void sendData(void * data) 				{ static SendQueuedDataPdu event; 			pushEvent( &event );}
	void receiveData(void * data) 			{}


private:
	enum { Idle , Advertise , EstablishConnection , AcceptConnection, Connected } _state , _oldState;
	enum { SendAdvertise , CheckPeerNodesPresence } _sendAdvertiseSubstate , _oldSendAdvertiseSubstate;
	enum { WaitForAdvertiseData , ProcessAdvertiseData } _receiveAdvertiseSubstate , _oldReceiveAdvertiseSubstate;
	enum { WaitForConnectionData , ProcessConnectionData } _establishConnectionSubstate , _oldEstablishConnectionSubstate;
	enum { SendQueuedDataPdu , WaitDataPdu , ProcessDataPdu , CloseConnection , WaitForNextConnectionEvent , EnableTransceiver } _connectedSubstate , _oldConnectedSubstate;

	enum role { MASTER , SLAVE };

	EventStatus processEvent() {

		IXFEvent * e = getCurrentEvent();
		_oldState = _state;
		_sendAdvertiseSubstate = _oldSendAdvertiseSubstate;
		_receiveAdvertiseSubstate = _oldReceiveAdvertiseSubstate;
		_establishConnectionSubstate = _oldEstablishConnectionSubstate;
		_connectedSubstate = _oldConnectedSubstate;


		switch( _state ) {
		case Idle:

			if(event == StartAdvertiseRequestEvent)
				currentState = Advertise;
			break;

		case Advertise:

			currentState = EstablishConnection;
			break;

		case EstablishConnection:

			currentState = AcceptConnection;
			break;

		case AcceptConnection:

			currentState = Connected;
			break;

		case Connected:

			break;
		}
	}

	/*
	 * Advertise
	 *
	 */
	DataLink::Advertise(AdvMode mode) {

		switch() {
		case SendAdvertise:

			currentState = ;
			break;

		case CheckPeerNodesPresence:

			currentState = ;
			break;

		case WaitForAdvertiseData:

			currentState = ;
			break;

		case ProcessAdvertiseData:

			currentState = ;
			return;

		default:
			//TODO error
			return;

		}
	}

	/*
	 * EstablishConnection
	 */
	DataLink::EstablishConnection() {

		switch() {
		default:

		case WaitForConnectionData:

			currentState = ProcessConnectionData;
			break;

		case ProcessConnectionData:

			currentState = WaitForConnectionData;
			break;
		}
	}

	/*
	 * Connected
	 */
	DataLink::Connected() {

		switch(currentState) {
		default:

		case SendQueuedDataPdu:

			currentState = WaitDataPdu;
			currentState = CloseConnection;
			currentState = WaitForNextConnectionEvent;
			break;


		case WaitDataPdu:

			currentState = CloseConnection;
			break;


		case ProcessDataPdu:

			currentState = SendQueuedDataPdu;
			currentState = CloseConnection;
			currentState = WaitForNextConnectionEvent;
			break;

		case CloseConnection:

			break;

		case WaitForNextConnectionEvent:

			currentState = EnableTransceiver;
			break;

		case EnableTransceiver:
			return;
			break;
		}
	}

};



DataLink::DataLink() {
	myPhyRTX = IAir3TFactory::factory().phyTransceiver();

	myPhyRTX.setObserver(this);
}



