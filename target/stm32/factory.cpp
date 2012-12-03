#include <target.h>
#include <IPhyTransceiver>
#include <IMonochromeDisplay>
#include <ILed>
#include <hal/spi.h>
#include <hal/uart.h>
#include <board/joystick.h>
#include <board/display/monochromedisplay.h>
#include <board/transceiver/nrftransceiver.h>
#include "inc/factory.h"
#include "inc/builder.h"


// Initialize static variable(s)
Factory * Factory::_pInstance = NULL;


Factory::Factory(int argc , char ** argv)
: _ledBlinker(_ledController),
  _pTransceiver(NULL),
  _pDisplay(NULL),
  _pJoystick(NULL),
  _pSharedSpi(NULL)
{
	// Initialize attributes
	memset(_SPI, 0, 2 * sizeof(SPI *));

	if (_pInstance)
	{
		assert(false); // Error: Create only one instance of Factory!
		return;
	}

	// Current instance is the singleton instance
	_pInstance = this;

	// Enable needed clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

    // Start state machine(s)
    _ledBlinker.startBehavior();
}

Factory::~Factory()
{

}

/**
 * Returns the only existing instance of Factory
 */
Factory & Factory::instance()
{
	assert(_pInstance);		// Create at least one instance of Factory!
	return *_pInstance;
}

IPhyTransceiver & Factory::phyTransceiver()
{
	if (!_pTransceiver)
	{
		_pTransceiver = new NrfTransceiver();
	}
	assert(_pTransceiver);
	return *_pTransceiver;
}

IMonochromeDisplay & Factory::monochromeDisplay()
{
	if (!_pDisplay)
	{
		_pDisplay = new MonochromeDisplay();

	}
	assert(_pDisplay);
	return *_pDisplay;
}

IJoystick & Factory::joystick()
{
	if (!_pJoystick)
	{
		_pJoystick = new Joystick;

		if (_pJoystick)
		{
			_pJoystick->initialize();
		}
	}
	assert(_pJoystick);
	return *_pJoystick;
}

ILed & Factory::led()
{
	return _ledController;
}

SPI & Factory::spi(const int spiNbr)
{
	assert(spiNbr >= 0 && spiNbr < 2);

	if (!_SPI[spiNbr])
	{
		// Create it
		_SPI[spiNbr] = new SPI(spiNbr);

		if (_SPI[spiNbr])
		{
			_SPI[spiNbr]->initialize();
		}
	}

	assert(_SPI[spiNbr]);
	return *_SPI[spiNbr];
}

Uart & Factory::uart(const unsigned char uartNbr)
{
	assert(uartNbr >= 0 && uartNbr < Uart::UART_COUNT);

	if (!Uart::_sUart[uartNbr])
	{
		// Create it
		new Uart(uartNbr);
	}

	assert(Uart::_sUart[uartNbr]);
	return Uart::uart(uartNbr);
}

DisplayNrfSharedSpi & Factory::sharedSpi()
{
	if (!_pSharedSpi)
	{
		_pSharedSpi = new DisplayNrfSharedSpi();
	}
	assert(_pSharedSpi);
	return *_pSharedSpi;
}

/**
 * Implementation of the factory method needed by the IAir3TFactory interface.
 */
IAir3TFactory & IAir3TFactory::factory(int argc , char ** argv)
{
	static Factory * pFactory = NULL;

	// Create the factory object on demand.
	if (!pFactory)
	{
		pFactory = new Factory(argc , argv);
	}
	assert(pFactory);

	// Return the reference to the object.
	return *pFactory;
}

/**
 * Implementation of the builder method needed by the IAir3TFactory interface.
 */
IBuilder & IAir3TFactory::builder()
{
	static Builder * pBuilder = NULL;

	if (!pBuilder)
	{
		pBuilder = new Builder();
	}
	assert(pBuilder);

	return *pBuilder;
}
