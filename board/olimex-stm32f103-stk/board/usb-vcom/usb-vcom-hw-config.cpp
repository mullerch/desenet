#include <target.h>
#include <vcom/usb_vcom.h>
#include <vcom/usb_pwr.h>
#include "usb-vcom-hw-config.h"

#if (USE_USB_TRACE != 0)

// Predeclarations
void Set_USBClock();
void Set_USBPullUp();
void Set_USBInterrupt(bool enableInterrupt = true);

// Some local defines
#define USB_PULLUP_GPIO_PORT GPIOC
#define USB_PULLUP_GPIO_BIT GPIO_Pin_11
#define USB_PULLUP_RCC_APB2PERIPH RCC_APB2Periph_GPIOC
#define USB_PULLUP_PIN PERIPHERAL_BIT(GPIOC->ODR, 11)

#define BB_BIT(REGISTER, BIT_NUMBER, BASE, BB_BASE) \
	((volatile s32*) ((BB_BASE) + (((u32) &(REGISTER)) - (BASE)) * 32 + (BIT_NUMBER) * 4))

#define PERIPHERAL_BIT(REGISTER, BIT_NUMBER) \
	BB_BIT(REGISTER, BIT_NUMBER, PERIPH_BASE, PERIPH_BB_BASE)

#define SRAM_BIT(REGISTER, BIT_NUMBER) \
	BB_BIT(REGISTER, BIT_NUMBER, SRAM_BASE, SRAM_BB_BASE)


// Local variable(s)
static bool bInitialized = false;


void board::usbVcomHwConfigInit()
{
	if (!bInitialized)
	{
		bInitialized = true;

		Set_USBClock();
		Set_USBPullUp();
		PowerOff();

		for (unsigned int i = 500000; i--;) { continue; }
		Set_USBInterrupt(false);

		// Wait a bit to let the Host disconnect the device
		for (unsigned int i = 500000; i--;) { continue; }

//		USB_Cable_Config(ENABLE);
//		for (unsigned int i = 500000; i--;) { continue; }

		Set_USBInterrupt(true);
		USB_Init();

		for (unsigned int i = 500000; i--;) { continue; }
		for (unsigned int i = 500000; i--;) { continue; }
	}
}

void USB_LP_CAN_RX0_IRQHandler(void)						// USB low priority interrupt
{
  USB_Istr();
}

void USB_Cable_Config(FunctionalState NewState)				// USB pull-up control
{
	//if (NewState != DISABLE) GPIO_ResetBits(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_BIT);
	//else GPIO_SetBits(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_BIT);
	//GPIO_WriteBit(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_BIT, NewState ? Bit_RESET : Bit_SET);
	*USB_PULLUP_PIN = NewState ? 0 : 1;
}

void Set_USBClock(void)
{
	/* USBCLK = PLLCLK / 1.5 */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

	/* Enable USB clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

void Set_USBPullUp()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(USB_PULLUP_RCC_APB2PERIPH, ENABLE);

	/* Setup USB pull-up */
	GPIO_InitStructure.GPIO_Pin = USB_PULLUP_GPIO_BIT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(USB_PULLUP_GPIO_PORT, &GPIO_InitStructure);

	USB_Cable_Config(DISABLE);
}

void Set_USBInterrupt(bool enableInterrupt /* = true */)
{
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else /* VECT_TAB_FLASH */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable USB_LP Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = (enableInterrupt) ? ENABLE : DISABLE;
	NVIC_Init(&NVIC_InitStructure);
}

#endif // (USE_USB_TRACE != 0)
