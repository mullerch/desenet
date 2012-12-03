#include <inc/isrs.h>


// Variable(s) defined in linker script
extern unsigned long _stack;


// Define the vector table
unsigned int * myvectors[] __attribute__ ((section("vectors"))) =
{
	(unsigned int *) 	&_stack,				// stack pointer
   	(unsigned int *) 	resetHandler,			// code entry point
   	(unsigned int *)	nmi_handler,			// NMI handler (not really)
   	(unsigned int *)	hardfault_handler,		// hard fault handler (let's hope not)
   	(unsigned int *)	MemManageException,
   	(unsigned int *)	BusFaultException,
   	(unsigned int *)	UsageFaultException,
   	(unsigned int *)	0, 0, 0, 0,            	// Reserved
   	(unsigned int *)	SVCHandler,
   	(unsigned int *)	DebugMonitor,
   	(unsigned int *)	0,                      // Reserved
   	(unsigned int *)	PendSVC,
   	(unsigned int *)	SysTickHandler,
   	(unsigned int *)	WWDG_IRQHandler,
   	(unsigned int *)	PVD_IRQHandler,
   	(unsigned int *)	TAMPER_IRQHandler,
   	(unsigned int *)	RTC_IRQHandler,
   	(unsigned int *)	FLASH_IRQHandler,
   	(unsigned int *)	RCC_IRQHandler,
   	(unsigned int *)	EXTI0_IRQHandler,
   	(unsigned int *)	EXTI1_IRQHandler,
   	(unsigned int *)	EXTI2_IRQHandler,
   	(unsigned int *)	EXTI3_IRQHandler,
   	(unsigned int *)	EXTI4_IRQHandler,
   	(unsigned int *)	DMAChannel1_IRQHandler,
   	(unsigned int *)	DMAChannel2_IRQHandler,
   	(unsigned int *)	DMAChannel3_IRQHandler,
   	(unsigned int *)	DMAChannel4_IRQHandler,
   	(unsigned int *)	DMAChannel5_IRQHandler,
   	(unsigned int *)	DMAChannel6_IRQHandler,
   	(unsigned int *)	DMAChannel7_IRQHandler,
   	(unsigned int *)	ADC_IRQHandler,
   	(unsigned int *)	USB_HP_CAN_TX_IRQHandler,
   	(unsigned int *)	USB_LP_CAN_RX0_IRQHandler,
   	(unsigned int *)	CAN_RX1_IRQHandler,
   	(unsigned int *)	CAN_SCE_IRQHandler,
   	(unsigned int *)	EXTI9_5_IRQHandler,
   	(unsigned int *)	TIM1_BRK_IRQHandler,
   	(unsigned int *)	TIM1_UP_IRQHandler,
   	(unsigned int *)	TIM1_TRG_CCUP_IRQHandler,
   	(unsigned int *)	TIM1_CC_IRQHandler,
   	(unsigned int *)	TIM2_IRQHandler,
   	(unsigned int *)	TIM3_IRQHandler,
   	(unsigned int *)	TIM4_IRQHandler,
   	(unsigned int *)	I2C1_EV_IRQHandler,
   	(unsigned int *)	I2C1_ER_IRQHandler,
   	(unsigned int *)	I2C2_EV_IRQHandler,
   	(unsigned int *)	I2C2_ER_IRQHandler,
   	(unsigned int *)	SPI1_IRQHandler,
   	(unsigned int *)	SPI2_IRQHandler,
   	(unsigned int *)	USART1_IRQHandler,
   	(unsigned int *)	USART2_IRQHandler,
   	(unsigned int *)	USART3_IRQHandler,
   	(unsigned int *)	EXTI15_10_IRQHandler,
   	(unsigned int *)	RTCAlarm_IRQHandler,
   	(unsigned int *)	USBWakeUp_IRQHandler
};
