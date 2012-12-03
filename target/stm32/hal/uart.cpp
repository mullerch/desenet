#include <inc/config.h>
#include <target.h>
#include <stm32f10x_usart.h>
#include "uart.h"

Uart * Uart::_sUart[Uart::UART_COUNT];				// Comment: Startup script should initialize pointers to zero (.bss section)
bool Uart::_sInitialized[Uart::UART_COUNT];			// Same for other arrays

Uart::Uart(const unsigned char uartNbr)
 : _uartNbr(-1),
   _pUart(NULL)
{
	assert(uartNbr >= 0 && uartNbr < UART_COUNT);

	if (uartNbr >= 0 && uartNbr < UART_COUNT)
	{
		if (!_sUart[uartNbr])
		{
			// Register instance
			_sUart[uartNbr] = this;
			_uartNbr = uartNbr;

			// Init _pUart
			switch(_uartNbr)
			{
			case 0:
				_pUart = USART1;
				break;
			case 1:
				_pUart = USART2;
				break;
			case 2:
				_pUart = USART3;
				break;
			default:
				assert(false);
			}
		}
		else
		{
			assert(false); 	// Error: Instance for specified UART already exists.
		}
	}
}

Uart::~Uart()
{

}

bool Uart::initialize(const unsigned long baudRate)
{
	if (_uartNbr == 0 || _uartNbr == 2)
	{
		// TODO: implement
		assert(false);
	}

	if (_uartNbr == 1)
	{	// Initialization for USART2

		GPIO_InitTypeDef gpioInitStructure;
		USART_ClockInitTypeDef  usartClockInitStructure;
		NVIC_InitTypeDef nvicInitStructure;
		USART_InitTypeDef usartInitStructure;

		// Enable clock for needed peripherals
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		// Tx Line Config
		gpioInitStructure.GPIO_Pin = GPIO_Pin_2;			// UART2 Tx PA.2
		gpioInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &gpioInitStructure);

		// Rx Line Config
		gpioInitStructure.GPIO_Pin = GPIO_Pin_3;			// UART2 Rx PA.3
		gpioInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &gpioInitStructure);

// Do not call code below in case you are using the SP1
// USRT2_CK and SPI1_NSS are on the same pin.
//		gpioInitStructure.GPIO_Pin = GPIO_Pin_4;
//		gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		gpioInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_Init(GPIOA, &gpioInitStructure);

		// Init clock for USART
		usartClockInitStructure.USART_Clock = USART_Clock_Disable;
		usartClockInitStructure.USART_CPOL = USART_CPOL_Low;
		usartClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
		usartClockInitStructure.USART_LastBit = USART_LastBit_Disable;
		USART_ClockInit(_pUart, &usartClockInitStructure);

		// Enable USART2 interrupt
		nvicInitStructure.NVIC_IRQChannel = USART2_IRQn;
		nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
		nvicInitStructure.NVIC_IRQChannelSubPriority = 0x00;
		nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;		// Enable USART2 interrupt
		NVIC_Init(&nvicInitStructure);

		// Configure USART
		usartInitStructure.USART_BaudRate = baudRate;
		usartInitStructure.USART_WordLength = USART_WordLength_8b;
		usartInitStructure.USART_StopBits = USART_StopBits_1;
		usartInitStructure.USART_Parity = USART_Parity_No;
		usartInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		usartInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(_pUart, &usartInitStructure);

		// Clear pending interrupts
		USART_ClearITPendingBit(_pUart, USART_IT_RXNE);
		USART_ClearITPendingBit(_pUart, USART_IT_TC);

		// Enable USART Receive interrupts
		USART_ITConfig(_pUart, USART_IT_RXNE, ENABLE);

		// Insider hack:
		// In order to output USART2_CK, the SSOE bit in the SPI1_CR2
		// register must be set to configure the pin in output mode.
		SPI1->CR2 |= SPI_CR2_SSOE;

		// Enable UARt
		enable();

		return true;
	}

	return false;
}

bool Uart::enable()
{
	  // Enable the usart
	  USART_Cmd(_pUart, ENABLE);
	  return true;
}

bool Uart::disable()
{
	  // enable the usart
	  USART_Cmd(_pUart, DISABLE);
	  return true;
}


void Uart::write(const char * str,
				 unsigned int length /* = 0 */)
{
	if (!length)
	{
		length = strlen(str);
	}

	for (unsigned int i = 0; i < length; i++)
	{
		// Send next character
		USART_SendData(_pUart, str[i]);

		// Wait until previous character got transfered
		while (USART_GetFlagStatus(_pUart, USART_FLAG_TXE) == RESET);
	}
}


void Uart::onIsr()
{

}

#if (USE_USART2_TRACE != 0)
/**
 * Interrupt Service Routine (ISR) for USART2
 */
void USART2_IRQHandler()
{
	unsigned char value;

	// Check overrun
	if (USART_GetITStatus(USART2, USART_FLAG_ORE) == SET)
	{
		// Clear ORE flag by reading first SR and then DR
		value = USART3->SR;	// Read status then...
		value = USART3->DR;	// Read data
	}

	// Check new character in RX register
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
		value = USART_ReceiveData(USART2);

		// TODO: Buffer received data in a buffer
	}
}
#endif // (USE_USART2_TRACE != 0)

