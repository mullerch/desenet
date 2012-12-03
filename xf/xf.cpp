#include "xf.h"
#include "xftimeoutmanager.h"


bool XF::_bInitialized = false;
XFThread XF::_mainThread;


/**
 * Initializes the eXecution Framework (XF). Call
 * this method prior to initialize the other objects of the system.
 */
void XF::init(int timeInterval)
{
	// Create and initialize XFTimeoutManager
	XFTimeoutManager::getInstance()->setTickInterval(timeInterval);
	// Start it
	XFTimeoutManager::getInstance()->start();
}

/**
 * Starts the execution of the framework. This
 * results in processing the events in the main loop.
 */


#ifdef TC_STM32 /*****************************************************************************************************/

void XF::start()
{
	_mainThread.start();
}

#endif /**************************************************************************************************************/


XFThread * XF::getMainThread()
{
	return &_mainThread;
}
