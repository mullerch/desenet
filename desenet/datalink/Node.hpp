#pragma once
/**
 * @file Node.hpp
 * @date fall 2012 - 18.01.2013
 * @author Christian Muller & Romain Maffina
 */

#include "utils/address.hpp"

/**
 * @brief Represents a node in the desenet network.
 */
class Node {

public:
	typedef Address<6u> NodeId;
	typedef Address<4> NodeAddress;

	Node(NodeId nodeId) : nodeId(nodeId) {

	}

	/**
	 * @return the node ID
	 */
	NodeId id() const {
		return nodeId;
	}

	/**
	 * @return the node address
	 */
	NodeAddress getAddress() {
		return address;
	}

private:
	NodeAddress address;
	NodeId nodeId;
};
