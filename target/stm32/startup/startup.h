/*
 * Startup.h
 *
 *  Created on: Feb 14, 2011
 *      Author: sth
 */

#ifndef STARTUP_H_
#define STARTUP_H_


#include "startup-init.h"


class Startup
{
public:
	static void init();

protected:
	static void clkInit();

};

#endif /* STARTUP_H_ */
