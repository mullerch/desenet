#ifndef XFTIMEOUTMANAGER_H
#define XFTIMEOUTMANAGER_H

using namespace std;

#ifdef TC_MESHSIM /***************************************************************************************************/

#include <QtCore/QObject>

#endif /**************************************************************************************************************/


#include <list>
#include "xftimeout.h"


/**
 * @ingroup xf
 * The TimeoutManager is responsible to handle timeouts used
 * in state machines. It decrements them accordingly and inject
 * them back to the state machine when timeouted.
 *
 * <b>Requirements:</b>
 * - Implements the Singleton pattern
 * - Handle timeouts (hold, decrement, re-inject)
 */
class XFTimeoutManager


#ifdef TC_MESHSIM /***************************************************************************************************/

	: public QObject

#endif /**************************************************************************************************************/


{
	typedef list<XFTimeout *> TimeoutList;
public:
	virtual ~XFTimeoutManager();

	static XFTimeoutManager * getInstance();	///< Returns a pointer to the single instance of XFTimeoutManager.
	void setTickInterval(int tickInterval);
	void start();

	void scheduleTimeout(int timeoutId, int interval, IXFReactive * pReactive);
	void unscheduleTimeout(int timeoutId, IXFReactive * pReactive);
	void tick();		///< Called regularly in XFTimeoutManager::_tickInterval time

protected:
	XFTimeoutManager();
	void addTimeout(XFTimeout * pNewTimeout);	///< Adds the timeout to #_timeouts.
	void removeTimeouts(int timeoutId, IXFReactive * pReactive);
	void returnTimeout(XFTimeout * pTimeout);	///< Returns timeout back to behavioral class.

	void startHardwareTimer(int tickTime);		///< Starts the hardware timer on the target hardware.

protected:
	static XFTimeoutManager * _pInstance;		///< Pointer to single instance of XFTimeoutManager
	int _tickInterval;							///< Interval in milliseconds the XFTimeoutManager is decrementing the timeouts
	TimeoutList _timeouts;						///< Container holding timeouts to manage


#ifdef TC_MESHSIM /***************************************************************************************************/

private:
	void timerEvent( QTimerEvent * event );

#endif /**************************************************************************************************************/


};

#endif // XFTIMEOUTMANAGER_H
