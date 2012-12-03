#pragma once


#include "config.h"
#include <IAir3TFactory>
#include <test/ledblinker.hpp>
#include <board/ledcontroller.h>
#include <board/shared-spi/displaynrfsharedspi.h>


class IPhyTransceiver;
class IMonochromeDisplay;
class IJoystick;
class ILed;
class SPI;
class Uart;


class Factory : public IAir3TFactory
{
public:
	Factory(int argc , char ** argv);
	virtual ~Factory();

	static Factory & instance();

	// Implementation of the IFactory interface
	IPhyTransceiver & phyTransceiver();
	IMonochromeDisplay & monochromeDisplay();
	IJoystick & joystick();
	ILed & led();

	SPI & spi(const int spiNbr);
	Uart & uart(const unsigned char uartNbr);

	DisplayNrfSharedSpi & sharedSpi();

private:
	static Factory * _pInstance;		///< Pointer to only instance of Factory

	LedController _ledController;
	LEDBlinker _ledBlinker;

	IPhyTransceiver * _pTransceiver;
	IMonochromeDisplay * _pDisplay;
	IJoystick * _pJoystick;

	SPI * _SPI[2];						///< Pointers to the 2 SPI peripherals

	DisplayNrfSharedSpi * _pSharedSpi;
};
