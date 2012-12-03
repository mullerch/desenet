/*
 * LedController.cpp
 *
 *  Created on: Jan 11, 2011
 *      Author: sth
 */


#include <target.h>
#include "ledcontroller.h"


LedController::LedController()
 : _bInitialized(false)
{

}

LedController::~LedController()
{

}

bool LedController::initialize()
{
	if (!_bInitialized)
	{
		GPIO_InitTypeDef gpioInitStructure;

		_bInitialized = true;

		// Enable needed clocks
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	    // Configure PC.12 as output push-pull (LED)
	    GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET);
	    gpioInitStructure.GPIO_Pin =  GPIO_Pin_12;
	    gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOC, &gpioInitStructure);
	}
	return true;
}

ILed::State LedController::state() const
{
	// TODO: To check if its right
	return State(((GPIOC->IDR & 0x00001000) == 0x00001000) ? ILed::On : ILed::Off);
}

void LedController::setState(State state)
{
	setLed(0, (state == ILed::On) ? true : false);
}

void LedController::setLed(int index, bool bEnable)
{
	if (index == 0)
	{
		if (bEnable)
		{
			GPIOC->BRR |= 0x00001000;
		}
		else
		{
			GPIOC->BSRR |= 0x00001000;
		}
	}
}
