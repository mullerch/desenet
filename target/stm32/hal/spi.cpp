#include <target.h>
#include <stm32f10x_spi.h>
#include "spi.h"

SPI::SPI(int spiNbr)
 : _bInitialized(false),
   _pSPI(NULL)
{
	switch (spiNbr)
	{
	case 0:
		_pSPI = SPI1;
		break;
	case 1:
		_pSPI = SPI2;
		break;
	case 2:
		_pSPI = SPI3;
		break;
	default:
		assert(false);	// Passed wrong parameter
	}
}

SPI::~SPI()
{

}

/**
 * Partially initializes the SPI. Other part of the initialization
 * needs to be made in an other place (see in board configuration).
 *
 * After complete initialization the SPI can be enabled using the #enable method.
 */
bool SPI::initialize()
{
//	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef gpioInitStructure;

	if (!_bInitialized)
	{
		_bInitialized = true;

		// Enable SPI1 and GPIOA clocks
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

		if (_pSPI == SPI1)
		{
			// Configure SPI1 pins: NSS, SCK, MISO and MOSI
			gpioInitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			gpioInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &gpioInitStructure);

			// Chip select - output (SPI1_NSS)
			gpioInitStructure.GPIO_Pin = GPIO_Pin_4;
			gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &gpioInitStructure);
		}
		else
		{
			assert(false);	// Initialization for this SPI not implemented yet
		}

		// Reset SPI Interface
	//	SPI_DeInit(SPI1);

	//	// SPI1 configuration
	//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//	SPI_InitStructure.SPI_CRCPolynomial = 7;
	//
	//	// Init SPI
	//	SPI_Init(SPI1, &SPI_InitStructure);
	//
	//	// Enable SPI1
	//	SPI_Cmd(SPI1, ENABLE);

		return true;
	}

	return false;
}

void SPI::enable()
{
	SPI_Cmd(_pSPI, ENABLE);
}

void SPI::disable()
{
	SPI_Cmd(_pSPI, DISABLE);
}

/**
 * Resets the state machine of the SPI. Useful only
 * when accessing multiple chips with one SPI
 */
void SPI::resetStateMachine()
{
	// Reset state machine of the SPI by resetting the periphery.
	SPI_I2S_DeInit(_pSPI);
}

bool SPI::getFlagStatus(unsigned short flagMask)
{
	assert(_pSPI);
	return SPI_I2S_GetFlagStatus(_pSPI, flagMask);
}

SPI::tStatus SPI::sendByte(unsigned char data)
{
	// Loop while DR register in not empty
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	// Send byte through the SPI1 peripheral
	SPI_I2S_SendData(SPI1, data);

	// Wait to receive a byte
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	// Return the byte read from the SPI bus
	return SPI_I2S_ReceiveData(SPI1);
}
