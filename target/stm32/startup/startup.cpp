/*
 * Startup.cpp
 *
 *  Created on: Feb 14, 2011
 *      Author: sth
 */

#include <target.h>
#include <stm32f10x_flash.h>
#include "startup.h"


// Defined in linker script
extern unsigned long _flash_data;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;


void Startup::init()
{
    register unsigned long *pulSrc, *pulDest;

    // Init system clock
	clkInit();

    // Copy the data segment initializers from flash to SRAM.
    pulSrc = &_flash_data;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *(pulDest++) = *(pulSrc++);
    }

    // Zero fill the bss segment.
    for(pulDest = &_bss; pulDest < &_ebss; )
    {
        *(pulDest++) = 0;
    }

/*    // Fill the stack with a known value.
    for(pulDest = pulStack; pulDest < pulStack + STACK_SIZE; )
    {
    	*pulDest++ = 0xA5A5;
    }
*/

    // Call global/static constructors (used with arm-none-eabi toolchain)
	init_array();

    //
	__do_global_ctors();
}

void Startup::clkInit()
{
	// Set STKALIGN in NVIC
	SCB->CCR |= 0x200;

	// 1. Clocking the controller from internal HSI RC (8 MHz)
	RCC_HSICmd(ENABLE);
	// wait until the HSI is ready
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	// 2. Enable ext. high frequency OSC
	RCC_HSEConfig(RCC_HSE_ON);
	// wait until the HSE is ready
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	// 3. Init PLL
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9); // 72MHz
//  RCC_PLLConfig(RCC_PLLSource_HSE_Div2,RCC_PLLMul_9); // 72MHz
	RCC_PLLCmd(ENABLE);
	// wait until the PLL is ready
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	// 4. Set system clock dividers
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);

	// Flash 1 wait state
//	*(vu32 *)0x40022000 = 0x12;
//	*(vu32 *)0x40022000 = 0x01;

    // Enable Prefetch Buffer
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    // Flash 2 wait state
    FLASH_SetLatency(FLASH_Latency_2);

	// Select PLL as system clock source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // Wait till PLL is used as system clock source
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
}
