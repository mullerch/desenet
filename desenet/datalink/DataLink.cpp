/*
 * DataLink.cpp
 *
 *  Created on: Dec 3, 2012
 *      Author: desem
 */

#include <desenet/datalink/DataLink.h>

DataLink::DataLink() {
	myPhyRTX = IAir3TFactory::factory().phyTransceiver();

	myPhyRTX.setObserver(this);
}
