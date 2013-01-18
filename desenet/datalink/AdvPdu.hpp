#pragma once

#include <stdint.h>
#include <phy/frame.h>
#include <desenet/datalink/Node>

using namespace std;

class AdvPdu {

public:
	typedef enum PduType {
		ADV,
		CONNECT_REQ,
		CONNECT_CONF
	} PduType;


	AdvPdu(Node::NodeId nodeId, uint8_t* advData) {
		header = ADV;
		payload.nodeId=nodeId;
		memcpy(payload.advData, advData, 4);
	}

	size_t size(){
		return 11;
	}

private:

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
