#pragma once
/**
 * @file idatalinkobserver.h
 * @date fall 2012 - 18.01.2013
 * @author Christian Muller & Romain Maffina
 */

#include <desenet/datalink/Common>
#include <desenet/datalink/DataPdu>
#include <desenet/datalink/Node>

using namespace datalink;

class Frame;

/**
 * @ingroup interfaces
 * @brief This interface represent a observer of the datalink and has to be implemented by all objects
 * that want use the DL and be informed (through the following callbacks)
 */
class IDataLinkObserver
{
protected:
	explicit IDataLinkObserver() {}
	virtual ~IDataLinkObserver() {}

public:
	/**
	 * @brief A node has appear
	 * @param node the new node in the network
	 */
	virtual void onNodeAppearIndication( const Node &node ) = 0;

	/**
	 * @brief A node has dissapear
	 * @param node the node that went
	 */
	virtual void onNodeDissapearIndication( const Node &node ) = 0;

	/**
	 * @brief The connection has been confirmed
	 * @param status the status
	 */
	virtual void onConnectConfirmation( ConnectConfirmationStatus status ) = 0;

	/**
	 * @brief A disconnection happen
	 * @param cause the cause
	 */
	virtual void onDisconnectIndication( DisconnectIndicationCause cause ) = 0;

	//virtual void onConnectIndication( const Node &node, Air3tService &service ) = 0;

	/**
	 * @brief a new data has been received
	 * @param pdu the data
	 */
	virtual void onDataIndication( const DataPdu &pdu ) = 0;

};
