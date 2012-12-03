#pragma once


/*
 * Interrupt priorities for the desenet-stm32 project.
 * Lower priority takes precedence:
 *
 * +----------------------------------------------+
 * | Interrupt Name | Preemption Prio | Sub-Prio  |
 * +----------------------------------------------+
 * | Reset          |      -3         |     x     |
 * | NMI            |      -2         |     x     |
 * | HardFault      |      -1         |     x     |
 * +----------------------------------------------+
 * | SysTick		|      0x00		  |     0     |
 * | NrfTransceiver |      0x03       |     0     |
 * | USB VCOM		|      0x04       |     0     |
 * | UART (trace)   |      0x05		  |     0     |
 * | Joystick		|      0x0F       |     0     |
 * +----------------------------------------------+
 */


void resetHandler();
void nmi_handler(void);
void hardfault_handler(void);
void MemManageException();
void BusFaultException();
void UsageFaultException();
void SVCHandler();
void DebugMonitor();
void PendSVC();
void SysTickHandler();
void WWDG_IRQHandler();
void PVD_IRQHandler();
void TAMPER_IRQHandler();
void RTC_IRQHandler();
void FLASH_IRQHandler();
void RCC_IRQHandler();
void EXTI0_IRQHandler();
void EXTI1_IRQHandler();
void EXTI2_IRQHandler();
void EXTI3_IRQHandler();
void EXTI4_IRQHandler();
void DMAChannel1_IRQHandler();
void DMAChannel2_IRQHandler();
void DMAChannel3_IRQHandler();
void DMAChannel4_IRQHandler();
void DMAChannel5_IRQHandler();
void DMAChannel6_IRQHandler();
void DMAChannel7_IRQHandler();
void ADC_IRQHandler();
void USB_HP_CAN_TX_IRQHandler();
void USB_LP_CAN_RX0_IRQHandler();
void CAN_RX1_IRQHandler();
void CAN_SCE_IRQHandler();
void EXTI9_5_IRQHandler();
void TIM1_BRK_IRQHandler();
void TIM1_UP_IRQHandler();
void TIM1_TRG_CCUP_IRQHandler();
void TIM1_CC_IRQHandler();
void TIM2_IRQHandler();
void TIM3_IRQHandler();
void TIM4_IRQHandler();
void I2C1_EV_IRQHandler();
void I2C1_ER_IRQHandler();
void I2C2_EV_IRQHandler();
void I2C2_ER_IRQHandler();
void SPI1_IRQHandler();
void SPI2_IRQHandler();
void USART1_IRQHandler();
void USART2_IRQHandler();
void USART3_IRQHandler();
void EXTI15_10_IRQHandler();
void RTCAlarm_IRQHandler();
void USBWakeUp_IRQHandler();

