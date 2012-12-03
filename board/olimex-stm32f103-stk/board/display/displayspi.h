#pragma once


#include <board/shared-spi/displaynrfsharedspi.h>


class DisplaySpi
{
public:
	DisplaySpi(DisplayNrfSharedSpi & sharedSpi);
	virtual ~DisplaySpi();

	bool initialize();

	void enable();		///< Enables the use of the SPI peripheral by the display. Removes ability for nRF chip to use it.
	void disable();		///< Gives SPI again back to nRF chip

protected:
	void sendCommand(unsigned char cmd);		///< Sends a command byte to the display
	void sendData(unsigned char data);			///< Sends a data byte to the display

private:
	inline DisplayNrfSharedSpi & sharedSpi() { return _sharedSpi; }

	inline void setCommandLine()				///< Sets display pin 'D/C' to send a command byte
	{
		// Set D/C line to 0 for command
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
	}

	inline void setDataLine()					///< Sets display pin 'D/C' to send a data byte
	{
		// Set D/C line to 1 for data
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);
	}

private:
	bool _bInitialized;
	DisplayNrfSharedSpi & _sharedSpi;			///< Reference to the shared SPI (Board interface)
};
