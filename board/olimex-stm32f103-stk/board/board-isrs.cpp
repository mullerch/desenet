

#include <target.h>
#include <board/transceiver/nrftransceiver.h>
#include "inc/factory.h"


/**
 * - Used to detect joystick 'center button' connected to
 * the PC5 GPIO.
 * - Used for the Transceiver (nRF24L01). Called when
 * Transceiver has to notify something to the MCU (GPIO RC9)
 */
void EXTI9_5_IRQHandler()
{
	// Joystick 'center button' IRQ
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{

		EXTI_ClearITPendingBit(EXTI_Line6);
	}

	// nRF24L01 IRQ
	if (EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		IPhyTransceiver & phyTransceiver = Factory::instance().phyTransceiver();

		// Call transceivers interrupt reception method
		static_cast<NrfTransceiver*>(&phyTransceiver)->onIrq();

		EXTI_ClearITPendingBit(EXTI_Line9);
	}

	{
		// Clear other pending interrupts
		EXTI_ClearITPendingBit(EXTI_Line5);
		EXTI_ClearITPendingBit(EXTI_Line7);
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

