#pragma once

#include <desenet/datalink/DataLink>
#include <interfaces/idatalinkobserver.h>
#include <desenet/datalink/Node>
#include <IJoystickObserver>
#include <IJoystick>
#include <trace/Trace>
#include <desenet/datalink/DataPdu>
#include <desenet/datalink/Common>

using namespace datalink;

#define DATALINK_LAYER_TESTER_PRESENT

/**
 * @ingroup test
 * Test class for the data link layer implementation.
 */
class DataLinkTester: public IDataLinkObserver, public IJoystickObserver {
public:
	/**
	 * Creates and initializes the test object for the data link layer.
	 */
	DataLinkTester(DataLink & dataLink, IJoystick & joystick) :
		_dataLink(dataLink) {
		// Set the observer pointer on the used subsystems.
		_dataLink.setObserver(this);
		joystick.setObserver(this);
	}

	// IDataLinkObserver interface implementation...
	void onNodeAppearIndication(const Node &node) {
		Trace::out("Node %s found.", node.id().toHexString().c_str());
	}

	void onNodeDissapearIndication(const Node &node) {
		Trace::out("Node %s lost.", node.id().toHexString().c_str());
	}

	bool onConnectIndication(const Node::NodeId & peerNode, uint16_t sapi) {
		Trace::out("Connection from %s.", peerNode.toHexString().c_str());

		// We only accept connections on SAPI 0.
		return sapi == 0;
	}

	void onConnectConfirmation(ConnectConfirmationStatus status) {
		if (status == CONNECTED)
			Trace::out("Connection established.");
		else
			Trace::out("Failed to establish connection!");
	}

	void onDisconnectIndication(DisconnectIndicationCause cause) {
		if (cause == DISCONNECT)
			Trace::out("Connection closed.");
		else
			Trace::out("Connection lost.");
	}

	void onDataIndication(const DataPdu &pdu) {
		Trace::out("Received Data: \"%s\".", pdu.getPayloadBytes());
	}

	// IjoystickObserver interface implementation.
	void onPositionChange(IJoystick::Position position) {
		switch (position) {
		case IJoystick::Up:
			Trace::out("Start advertising the node...");
			//_dataLink.startAdvertiseRequest(std::vector<uint8_t>(4, 'A'));
			_dataLink.startAdvertiseRequest((uint8_t*)"AAAA");
			break;

		case IJoystick::Down:
			Trace::out("Stop advertising the node. ");
			_dataLink.stopAdvertiseRequest();
			break;

		case IJoystick::Right:
			Trace::out("Connect to a predefined node (Node A: 0x000000000001).");
			_dataLink.connectRequest(
					Node::NodeId::fromHexString("000000000001"), 200);
			break;

		case IJoystick::Left:
			Trace::out("Disconnect...");
			_dataLink.disconnectRequest();
			break;

		case IJoystick::Pressed: {
			Trace::out("Send a test frame.");
			const char * test = "Lorem Ipsum...";
			DataPdu pdu(test, 8);
			_dataLink.dataRequest(pdu);
		}
			break;

		default:
			break;
		}
	}

private:
	DataLink & _dataLink;
};
