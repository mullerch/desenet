#pragma once
/*
 * Node.hpp
 *
 *  Created on: Dec 10, 2012
 *      Author: desem
 */

#include "utils/address.hpp"
class Node {

public:
	typedef Address<6u> NodeId;
	typedef Address<4> NodeAddress;

	Node(NodeId nodeId) : nodeId(nodeId) { }

	NodeId id() const {
		return nodeId;
	}

	NodeAddress getAddress() {
		return address;
	}

private:
	NodeAddress address;
	NodeId nodeId;
};
