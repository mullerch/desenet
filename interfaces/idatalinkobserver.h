#pragma once
#include <desenet/datalink/Common>
#include <desenet/datalink/DataPdu>
#include <desenet/datalink/Node>

using namespace datalink;

class Frame;

/**
 * @ingroup interfaces
 * @brief This interface has to be implemented by all objects who want to register them self as observer at an object
 * implementing the IPhyTransceiver interface.
 *
 * For every received frame, the method onReceive() will be called and for each send frame, the onSendStatus() method will
 * be called with the actual state of the send operation. Note that depending on the used platform, the context in which
 * the two methods are executed can vary and it can be the same thread, another thread or in the worst case even an
 * interrupt service routine. So do not use the new() method or the malloc() function inside the methods and do not call
 * other methods that could use them. Execute only as much code as needed to process these events (set event flags,
 * push event to queue,...).
 */
class IDataLinkObserver
{
protected:
	// It is just an interface, so constructor and destructor should be not public.
	explicit IDataLinkObserver() {}
	virtual ~IDataLinkObserver() {}

public:
	virtual void onNodeAppearIndication( const Node &node ) = 0;
	virtual void onNodeDissapearIndication( const Node &node ) = 0;

	virtual void onConnectConfirmation( ConnectConfirmationStatus status ) = 0;

	virtual void onDisconnectIndication( DisconnectIndicationCause cause ) = 0;

	//virtual void onConnectIndication( const Node &node, Air3tService &service ) = 0;
	virtual void onDataIndication( const DataPdu &pdu ) = 0;

};
