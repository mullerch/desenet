#pragma once

#include <inc/config.h>			// Defines JOYSTICK_TRACE_VERBOSE
#include <xf/xfreactive.h>
#include <IJoystick>


class Joystick : public IJoystick, public XFReactive
{
public:
	Joystick();
	virtual ~Joystick();

	// Implementations of the IJoystick interface
	bool initialize();
	bool setObserver(IJoystickObserver * observer);
	Position position() const;

protected:
	void _initCenterButtonIrq() const;
	void _initAdc() const;
	unsigned short _readAdcValue();

	// From XFReactive
	virtual EventStatus processEvent();

	// Method(s) called in state machine
	void doCheckJoystickState();

	static void traceOut(const char * const format, ...);

protected:
	bool _bInitialized;
	IJoystickObserver * _pObserver;
	Position _position;

	/**
	 * Values to compare measured values by the ADC
	 */
	typedef enum
	{
		UP_VALUE        = 960,
		DOWN_VALUE      = 190,
		LEFT_VALUE      = 1990,
		RIGHT_VALUE     = 470,
		DIVERSION       =  30
	}	eValue;

	/**
	 * Timeout identifier(s) for this state machine
	 */
	typedef enum
	{
		Timeout_CHECK_JOYSTICK_id = 1	///< Timeout id for CHECK_JOYSTICK state
	} eTimeoutId;

	/**
	 * Enumeration used to have a unique identifier for every
	 * state in the state machine.
	 */
	typedef enum
	{
		STATE_UNKOWN = 0,			///< Unknown state
		STATE_INITIAL = 1,			///< Initial state
		STATE_CHECK_JOYSTICK = 2	///< LED_ON state
	} eMainState;

	eMainState _currentState;		///< Attribute indicating currently active state
};
