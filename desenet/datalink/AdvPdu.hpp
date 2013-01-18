#pragma once
/**
 * @file AdvPdu.hpp
 * @date fall 2012 - 18.01.2013
 * @author Christian Muller & Romain Maffina
 */

#include <stdint.h>
#include <phy/frame.h>
#include <desenet/datalink/Node>

using namespace std;

/**
 * @brief Represents a DL advertise PDU.
 *
 * This PDU is used during the advertising phase to :
 * - informs other nodes of our presence
 * - request a new connection with a specific node
 * - confirm a new connection with a specific node
 */
class AdvPdu {

public:

	/**
	 * @enum PduType
	 * @brief Allow to differentiate the type of advertising PDU.
	 */
	typedef enum PduType {
		ADV,			/**< Advertising type */
		CONNECT_REQ,	/**< New connection request type */
		CONNECT_CONF	/**< New connection confirm type */
	} PduType;

	/**
	 * @param nodeId our node ID (to inform others)
	 * @param advData pointer to the advertising description infos
	 */
	AdvPdu(Node::NodeId nodeId, uint8_t* advData) {
		header = ADV;
		payload.nodeId=nodeId;
		memcpy(payload.advData, advData, 4); //hard-coded for debbuging purpose
	}

	/**
	 * return size
	 */
	size_t size(){
		return 11; //hard-coded for debugging purpose
	}

private:

	/**
	 * @struct AdvPayload
	 * @brief Define the structure of a Advertising payload
	 */
	typedef struct AdvPayload {
		Node::NodeId nodeId;
		uint8_t advData[4];
		//AdvPayload(Node::NodeId nodeId, uint8_t* advData) : nodeId(nodeId), advData(advData) {}
	} AdvPayload;

	/*
	 * AdvPdu has to be adapted for use with the following struct too
	 * So it can assume connection requests
	 */
//	typedef struct ConnectPayload {
//		Node::NodeId peerNodeId;
//		Node::NodeId myNodeId;
//
//		struct {
//			Address<4u> accessAddress;
//			uint8_t radioChannel;
//			uint16_t connInterval;
//			uint16_t serviceUUID;
//		} connectionParameters;
//
//	} ConnectPayload;

	PduType header;
	AdvPayload payload;

};
