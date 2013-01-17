/*
 * Node.hpp
 *
 *  Created on: Dec 10, 2012
 *      Author: desem
 */

#ifndef NODE_HPP_
#define NODE_HPP_

#include "utils/address.hpp"
class Node{
public:
	typedef Address<6> NodeId;

	typedef Address<4> NodeAddress;

	NodeAddress address;
};

#endif /* NODE_HPP_ */
