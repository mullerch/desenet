#pragma once
#include <xf/xfreactive.h>
#include <ILed>


/**
 * @defgroup test Test
 * Several test procedures for specific system compoents.
 * @{
 *
 * @section test_cd Class Diagram
 * The test class diagram:
 * @image html Test.png
 */


/**
 * This class can be used to test the eXecution Framework (XF). It uses the XF for event processing and
 * the led as the output.
 */
class LEDBlinker : public XFReactive
{
public:
	/**
	 * Creates the object and starts directly its behavior.
	 */
	LEDBlinker( ILed & ledController ) : _ledController( ledController )
	{
		_currentState = STATE_INITIAL;

		// Start state machine
		startBehavior();
	}

	virtual ~LEDBlinker()
	{}

protected:
	/**
	 * FSM implementation.
	 */
	virtual EventStatus processEvent()
	{
		eEventStatus eventStatus = EventStatus::Unknown;

		switch (_currentState)
		{
		case STATE_INITIAL:
			{
				if ( getCurrentEvent()->getEventType() == IXFEvent::Initial )
				{
					GEN(XFNullTransition());

					_currentState = STATE_LED_ON;

					eventStatus = EventStatus::Consumed;
				}
			}
			break;
		case STATE_LED_ON:
			{
				if (getCurrentEvent()->getEventType() == IXFEvent::NullTransition ||
					(getCurrentEvent()->getEventType() == IXFEvent::Timeout &&
					 getCurrentTimeout()->getId() == Timeout_LED_OFF_id))
				{
					{
						_ledController = ILed::On;
					}

					getThread()->scheduleTimeout(Timeout_LED_ON_id, 100, this);

					_currentState = STATE_LED_OFF;

					eventStatus = EventStatus::Consumed;
				}
			}
			break;
		case STATE_LED_OFF:
			{
				if (getCurrentEvent()->getEventType() == IXFEvent::Timeout &&
					 getCurrentTimeout()->getId() == Timeout_LED_ON_id)
				{
					{
						_ledController = ILed::Off;
					}

					getThread()->scheduleTimeout(Timeout_LED_OFF_id, 900, this);

					_currentState = STATE_LED_ON;

					eventStatus = EventStatus::Consumed;
				}
			}
			break;
		default:
			break;
		}

		return eventStatus;
	}

protected:
	/**
	 * Timeout identifier(s) for this state machine
	 */
	typedef enum
	{
		Timeout_LED_ON_id = 1,	///< Timeout id for LED_ON state
		Timeout_LED_OFF_id = 2	///< Timeout id for LED_OFF state
	} eTimeoutId;

	/**
	 * Enumeration used to have a unique identifier for every
	 * state in the state machine.
	 */
	typedef enum
	{
		STATE_UNKOWN = 0,			///< Unkown state
		STATE_INITIAL = 1,			///< Initial state
		STATE_LED_ON = 2,			///< LED_ON state
		STATE_LED_OFF = 3			///< LED_OFF state
	} eMainState;

	eMainState _currentState;		///< Attribute indicating currently active state

	ILed & _ledController;
};


/** @} */
