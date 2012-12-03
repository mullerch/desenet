#include "displayspi.h"


DisplaySpi::DisplaySpi(DisplayNrfSharedSpi & sharedSpi)
: _bInitialized(false),
  _sharedSpi(sharedSpi)
{

}

DisplaySpi::~DisplaySpi()
{

}

bool DisplaySpi::initialize()
{
	if (!_bInitialized)
	{
		_bInitialized = true;

		sharedSpi().initialize();

		return true;
	}
	return false;
}

void DisplaySpi::enable()
{
	sharedSpi().setSpiUsedByDisplay();
}

void DisplaySpi::disable()
{
	sharedSpi().setSpiUsedByNrfTransceiver();
}

void DisplaySpi::sendCommand(unsigned char cmd)
{
	setCommandLine();				// Tell display we are sending a command byte
	sharedSpi().sendByte(cmd);
}

void DisplaySpi::sendData(unsigned char data)
{
	setDataLine();					// Tell display we are sending a data byte
	sharedSpi().sendByte(data);
}
