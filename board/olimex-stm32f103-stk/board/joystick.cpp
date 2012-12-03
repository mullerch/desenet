#include <target.h>
#include <stm32f10x_adc.h>
#include <IJoystickObserver>
#include "joystick.h"

#if (JOYSTICK_TRACE_VERBOSE != 0)
	#include <trace/trace.h>
	#include <stdarg.h>
	#include <stdio.h>
#endif

Joystick::Joystick()
{
	_bInitialized = false;
	_pObserver = NULL;
	_position = Invalid;
	_currentState = STATE_INITIAL;
}

Joystick::~Joystick()
{

}

bool Joystick::initialize()
{
	if (!_bInitialized)
	{
		_bInitialized = true;

		_initCenterButtonIrq();
		_initAdc();

		// Start state machine
		startBehavior();

		return true;
	}
	return false;
}

bool Joystick::setObserver(IJoystickObserver * observer)
{
	_pObserver = observer;

	// Tell them the actual position
	//_pObserver->onPositionChange(position());

	return true;
}

IJoystick::Position Joystick::position() const
{
	return _position;
}

/**
 * Initializes and enables the interrupt for the
 * joystick center button (GPIO PC6).
 */
void Joystick::_initCenterButtonIrq() const
{
	GPIO_InitTypeDef gpioInitStructure;
	EXTI_InitTypeDef extiInitStructure;
	NVIC_InitTypeDef nvicInitStructure;

	// Enable needed clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO  | RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure USB host interrupt pin as input floating */
	gpioInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOC, &gpioInitStructure);

	// Define GPIO to
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);

	// Configure external interrupt for PC6
	EXTI_ClearITPendingBit(EXTI_Line6);
	extiInitStructure.EXTI_Line = EXTI_Line6;
	extiInitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	extiInitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&extiInitStructure);

	// Enable EXTI interrupt for PC6
	nvicInitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvicInitStructure.NVIC_IRQChannelSubPriority = 0x00;
	nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInitStructure);
}

/**
 * Initializes ADC15 used to measure the joysticks button state (left, right, up, down).
 * ADC1 is used for the conversion
 */
void Joystick::_initAdc() const
{
	GPIO_InitTypeDef	gpioInitStructure;
	ADC_InitTypeDef		adcInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	ADC_DeInit(ADC1);

	// Set RC5 as analog input
	gpioInitStructure.GPIO_Pin = GPIO_Pin_5;
	gpioInitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	gpioInitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init (GPIOC, &gpioInitStructure);

	// ADC Structure Initialization
	ADC_StructInit(&adcInitStructure);
	// Preinit
	adcInitStructure.ADC_Mode = ADC_Mode_Independent;
	adcInitStructure.ADC_ScanConvMode = DISABLE;
	adcInitStructure.ADC_ContinuousConvMode = DISABLE;
	adcInitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adcInitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	adcInitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &adcInitStructure);

	// Enable the ADC
	ADC_Cmd(ADC1, ENABLE);
}

unsigned short Joystick::_readAdcValue()
{
	// Configure channel
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);

	// Start the conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	// Wait until conversion completion
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	// Get the conversion value
	return ADC_GetConversionValue(ADC1);
}

EventStatus Joystick::processEvent()
{
	eEventStatus eventStatus = EventStatus::Unknown;

	switch (_currentState)
	{
	case STATE_INITIAL:
		{
			if (getCurrentEvent()->getEventType() == IXFEvent::Initial)
			{
				GEN(XFNullTransition());

				_currentState = STATE_CHECK_JOYSTICK;

				eventStatus = EventStatus::Consumed;
			}
		}
		break;
	case STATE_CHECK_JOYSTICK:
		{
			if (getCurrentEvent()->getEventType() == IXFEvent::NullTransition ||
				(getCurrentEvent()->getEventType() == IXFEvent::Timeout &&
				 getCurrentTimeout()->getId() == Timeout_CHECK_JOYSTICK_id))
			{
				{
					doCheckJoystickState();
				}

				getThread()->scheduleTimeout(Timeout_CHECK_JOYSTICK_id, 50, this);

				_currentState = STATE_CHECK_JOYSTICK;

				eventStatus = EventStatus::Consumed;
			}
		}
		break;
	default:
		break;
	}

	return eventStatus;
}

void Joystick::doCheckJoystickState()
{
	// Store previously calculated value
	const Position oldPosition = _position;

	// Check first if center button is pressed
	if((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)) == Bit_SET)
	{
		_position = Pressed;
	}
	else
	{	// Check other positions
		unsigned short value;

		// Read the voltage on the PC5/ADC15 pin. BUTTONS pin
		value = _readAdcValue();

#if (JOYSTICK_TRACE_VERBOSE != 0)
		if (value)
		{
			traceOut("Joystick: Measured value -> %d", value);
		}
#endif

		if ( value == 0)
		{
			_position = Center;
		}
		else if((value > (LEFT_VALUE-DIVERSION)) &&
				(value < (LEFT_VALUE+DIVERSION)))
		{
			_position = Left;
		}
		else if ((value > (RIGHT_VALUE-DIVERSION)) &&
				 (value < (RIGHT_VALUE+DIVERSION)) )
		{
			_position = Right;
		}
		else if ((value > (UP_VALUE-DIVERSION)) &&
				 (value < (UP_VALUE+DIVERSION)))
		{
			_position = Up;
		}
		else if ((value > (DOWN_VALUE-DIVERSION)) &&
				 (value < (DOWN_VALUE+DIVERSION)))
		{
			_position = Down;
		}
	}

	// Did the position of the joystick change?
	if (oldPosition != _position)
	{
#if (JOYSTICK_TRACE_VERBOSE != 0)
		switch (_position)
		{
		case Center:
			traceOut("Joystick: released");
			break;
		case Pressed:
			traceOut("Joystick: pressed");
			break;
		case Left:
			traceOut("Joystick: left");
			break;
		case Right:
			traceOut("Joystick: right");
			break;
		case Up:
			traceOut("Joystick: up");
			break;
		case Down:
			traceOut("Joystick: down");
			break;
		default:
			// Say nothing
			break;
		}
#endif // (JOYSTICK_TRACE_VERBOSE != 0)

		// Notify observer
		if (_pObserver)
		{
			_pObserver->onPositionChange(position());
		}
	}
}

void Joystick::traceOut(const char * const format, ...)
{
#if (JOYSTICK_TRACE_VERBOSE != 0)
	static const unsigned int STR_SIZE = 64;
	static char str[STR_SIZE];

	va_list args;

	va_start(args, format);
	vsiprintf(str, format, args);
	va_end(args);

	assert(strlen(str) < STR_SIZE);		// Uups! str is too small

	Trace::out(str);
#endif
}
