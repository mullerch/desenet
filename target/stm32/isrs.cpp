#include <inc/config.h>
#include <startup/startup.h>
#include <trace/trace.h>

extern int main();

/**
 * Default Interrupt Service Routines
 */

void resetHandler()
{
	// Call system startup initialization
    Startup::init();

    // Call the application's entry point.
    main();

    Trace::out("Warning: Exit Application");

    // Call global/static destructors
    fini_array();
}

void nmi_handler(void) { Trace::out("Error: NMI Handler!"); while(1){} }
void hardfault_handler(void) { Trace::out("Error: Hard Fault Handler!"); while(1){} }
void MemManageException() {}
void BusFaultException() { Trace::out("Error: Bus Fault Exception!"); while(1){} }
void UsageFaultException() {}
void SVCHandler() {}
void DebugMonitor() {}
void PendSVC() {}
//void SysTickHandler {}		// Defined in XFTimeoutManager.cpp
void WWDG_IRQHandler() {}
void PVD_IRQHandler() {}
void TAMPER_IRQHandler() {}
void RTC_IRQHandler() {}
void FLASH_IRQHandler() {}
void RCC_IRQHandler() {}
void EXTI0_IRQHandler() {}
void EXTI1_IRQHandler() {}
void EXTI2_IRQHandler() {}
void EXTI3_IRQHandler() {}
void EXTI4_IRQHandler() {}
void DMAChannel1_IRQHandler() {}
void DMAChannel2_IRQHandler() {}
void DMAChannel3_IRQHandler() {}
void DMAChannel4_IRQHandler() {}
void DMAChannel5_IRQHandler() {}
void DMAChannel6_IRQHandler() {}
void DMAChannel7_IRQHandler() {}
void ADC_IRQHandler() {}
void USB_HP_CAN_TX_IRQHandler() {}
#if (USE_USB_TRACE == 0)
	void USB_LP_CAN_RX0_IRQHandler() {}			// Defined in usb-vcom-hw-config.h
#endif
void CAN_RX1_IRQHandler() {}
void CAN_SCE_IRQHandler() {}
//void EXTI9_5_IRQHandler() {}					// Defined in board/board-isrs.cpp
void TIM1_BRK_IRQHandler() {}
void TIM1_UP_IRQHandler() {}
void TIM1_TRG_CCUP_IRQHandler() {}
void TIM1_CC_IRQHandler() {}
void TIM2_IRQHandler() {}
void TIM3_IRQHandler() {}
void TIM4_IRQHandler() {}
void I2C1_EV_IRQHandler() {}
void I2C1_ER_IRQHandler() {}
void I2C2_EV_IRQHandler() {}
void I2C2_ER_IRQHandler() {}
void SPI1_IRQHandler() {}
void SPI2_IRQHandler() {}
void USART1_IRQHandler() {}
#if (USE_USART2_TRACE == 0)
	void USART2_IRQHandler() {}					// Defined in uart.cpp
#endif
void USART3_IRQHandler() {}
void EXTI15_10_IRQHandler() {}
void RTCAlarm_IRQHandler() {}
void USBWakeUp_IRQHandler() {}
