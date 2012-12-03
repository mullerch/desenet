/*
 * DataLink.h
 *
 *  Created on: Dec 3, 2012
 *      Author: desem
 */

#ifndef DATALINK_H_
#define DATALINK_H_

#include <interfaces/iair3tfactory.h>
#include <interfaces/iphytransceiver.h>
#include <xf/xfreactive.h>

class DataLink : public XFReactive {

private:
	DataLink();

	virtual void startBehavior();					///< Starts the behavior, resp. the state machine.

	virtual void pushEvent(IXFEvent * pEvent);
	virtual EventStatus process(IXFEvent * pEvent);


	IPhyTransceiver myPhyRTX;
public:


};


#endif /* DATALINK_H_ */
