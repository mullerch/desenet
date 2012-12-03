#include <target.h>
#include <inc/factory.h>
#include <math.h>
#include <trace/trace.h>
#include "monochromedisplay.h"

MonochromeDisplay::MonochromeDisplay()
 : DisplaySpi(Factory::instance().sharedSpi())
{

}

MonochromeDisplay::~MonochromeDisplay()
{

}

IMonochromeDisplay::Size MonochromeDisplay::resolution() const
{
	return Size(Width, Height);
}

IMonochromeDisplay::AcceleratedDrawingMethods MonochromeDisplay::acceleratedDrawingMethods() const
{
	AcceleratedDrawingMethods accels;

	memset(&accels, 0, sizeof(AcceleratedDrawingMethods));

	accels.memoryCopyAccelerated = 1;

	return accels;
}

bool MonochromeDisplay::initialize()
{
	GPIO_InitTypeDef gpioInitStructure;

	// Enable needed clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// Initialize SPI (for general use)
	DisplaySpi::initialize();

	// Configure PB2 as Output push-pull, used as D/C
	gpioInitStructure.GPIO_Pin = GPIO_Pin_2;
	gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpioInitStructure);
	// D/C high
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);

	// Configure PC7 and PC10 as Output push-pull, used as LCD_RES and LCD_E
	gpioInitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_10;
	gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &gpioInitStructure);

	// LCD_E - disable
	GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_SET);
	// Set Reset pin (active low)
	GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);

	// Reset the display
	enableReset(true);
	for (unsigned int i = 2000; i--;) { continue; }
	enableReset(false);

	// Setup display
	configDisplay();
	setContrast(0x45);
	clear();
	update();

	return true;
}

bool MonochromeDisplay::enable()
{
	// TODO
	return true;
}

bool MonochromeDisplay::disable()
{
	// TODO
	return true;
}

IMonochromeDisplay::PixelColor MonochromeDisplay::pixel(const Point & point)
{
	unsigned int    index   = 0;
	unsigned char   offset  = 0;
	unsigned char   data    = 0;
	unsigned char 	mask;

	// check for out off range
	assert(point.x < Width);
	assert(point.y < Height);

	index = ((point.y / 8) * Width) + point.x;
	offset  = point.y - ((point.y / 8) * 8);
	mask = 0x01 << offset;

	data = _lcdBuffer[index];

	if ((data & mask) == mask)
	{
		return Black;
	}
	else
	{
		return White;
	}
}

void MonochromeDisplay::setPixel(const Point & point , PixelColor color)
{
	unsigned int    index   = 0;
	unsigned char   offset  = 0;
	unsigned char   data    = 0;

	// check for out off range
	if (point.x > Width) return;
	if (point.y > Height) return;

	index = ((point.y / 8) * Width) + point.x;
	offset  = point.y - ((point.y / 8) * 8);

	data = _lcdBuffer[index];

	if (color == White)
	{
		data &= (~(0x01 << offset));
	}
	else if (color == Black)
	{
		data |= (0x01 << offset);
	}

	_lcdBuffer[index] = data;
}

void MonochromeDisplay::clear(PixelColor background /* = White */)
{
	memset(_lcdBuffer, 0x00, sizeof(_lcdBuffer));
}

void MonochromeDisplay::writeDisplayMemory(size_t startAddress,
										   const uint8_t * const sourceBitmap,
										   size_t numberOfBits)
{
	for (size_t i = (startAddress * 8); i < (numberOfBits + (startAddress * 8)); i++)
	{
		Point point((i % Width), (i / Width));

		setPixel(point, getPixelColorFromBitmap(point, sourceBitmap - startAddress));
	}

	update();
}

void MonochromeDisplay::readDisplayMemory(size_t startAddress,
										  uint8_t * const destinationBitmap,
										  size_t numberOfBits)
{
	assert(startAddress == 0);	// Not tested with values other then zero

	for (size_t i = startAddress; i < numberOfBits; i++)
	{
		Point point((i % Width), (i / Width));

		setPixelColorToBitmap(destinationBitmap, point, pixel(point));
	}
}

void MonochromeDisplay::sendCommand(unsigned char cmd)
{
	enableDisplay(true);

	DisplaySpi::sendCommand(cmd);

	enableDisplay(false);
}

void MonochromeDisplay::sendData(unsigned char data)
{
	enableDisplay(true);

	DisplaySpi::sendData(data);

	enableDisplay(false);
}

/**
 * Sets or clears the reset line. The method is responsible to check if the
 * reset line is active high or active low.
 * Comment: The state machine does automatically a reset after calling the
 * initialization method. Use the method isDisplayReady() to check if the
 * display is ready for usage.
 * \param enable Set to true to set the display in reset state, false to leave reset state
 */
void MonochromeDisplay::enableReset(bool enable)
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_7, (enable) ? Bit_RESET : Bit_SET);
}

/**
 * Enables/disables the display controller.
 * \param enable Set to true to enable the display controller, false to disable it.
 */
void MonochromeDisplay::enableDisplay(bool enable)
{
	// Enable display controller (active low) -> LCD_E low
	GPIO_WriteBit(GPIOC, GPIO_Pin_10, (enable) ? Bit_RESET : Bit_SET);
}

void MonochromeDisplay::setContrast(unsigned char contrast)
{
	DisplaySpi::enable();		// Setup SPI for display
	enableDisplay(true);		// Enable display

	// LCD Extended Commands
	DisplaySpi::sendCommand(0x21);

	// Set LCD Vop (Contrast)
	DisplaySpi::sendCommand(0x80 | contrast);

	//  LCD Standard Commands, horizontal addressing mode
	DisplaySpi::sendCommand(0x20);

	enableDisplay(false);
	DisplaySpi::disable();
}

void MonochromeDisplay::configDisplay()
{
	DisplaySpi::enable();		// Setup SPI for display
	enableDisplay(true);		// Enable display

	// Send sequence of command
	DisplaySpi::sendCommand(0x21);		// LCD Extended Commands.
	DisplaySpi::sendCommand(0xC8);		// Set LCD Vop (Contrast).
	DisplaySpi::sendCommand(0x06);		// Set Temp coefficent.
	DisplaySpi::sendCommand(0x13);		// LCD bias mode 1:48.
	DisplaySpi::sendCommand(0x20);		// LCD Standard Commands, Horizontal addressing mode.
	DisplaySpi::sendCommand(0x08);		// LCD blank
	DisplaySpi::sendCommand(0x0C);		// LCD in normal mode.

	enableDisplay(false);
	DisplaySpi::disable();
}

void MonochromeDisplay::update()
{
	DisplaySpi::enable();		// Setup SPI for display
	enableDisplay(true);		// Enable display

	//  Set base address X=0 Y=0
	DisplaySpi::sendCommand(0x80);
	DisplaySpi::sendCommand(0x40);

	//  Serialize the video buffer.
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		DisplaySpi::sendData(_lcdBuffer[i]);
	}

	enableDisplay(false);
	DisplaySpi::disable();
}


