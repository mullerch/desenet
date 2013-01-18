#pragma once
/**
 * @file Common.h
 * @date fall 2012 - 18.01.2013
 * @author Christian Muller & Romain Maffina
 * @brief A common file for all datalink stuff
 */

namespace datalink {

	/**
	 * @enum ConnectConfirmationStatus
	 * @brief Indicated status of the connection process
	 */
	enum ConnectConfirmationStatus{
		CONNECTED, /**< Connected */
		PROCESS /**< Still connecting */
	};

	/**
	 * @enum DisconnectIndicationCause
	 * @brief Indicates the cause of a disconnection
	 */
	enum DisconnectIndicationCause{
		DISCONNECT, /**< the connection has been closed normally */
		LINK_LOSS, /**< the carrying link is down or the connection is lost */
		NO_DISCONNECT /**< indicates no disconnection */
	};
}
