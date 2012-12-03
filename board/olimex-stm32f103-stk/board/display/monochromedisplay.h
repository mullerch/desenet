#pragma once


#include <IMonochromeDisplay>
#include <board/display/displayspi.h>


class MonochromeDisplay : public IMonochromeDisplay,
						  public DisplaySpi
{
public:
	MonochromeDisplay();
	virtual ~MonochromeDisplay();

	typedef IMonochromeDisplay::Point Point;

	// Implementation of the IMonochromeDisplay interface
	virtual Size resolution() const;
	virtual AcceleratedDrawingMethods acceleratedDrawingMethods() const;
	virtual bool initialize();
	virtual bool enable();
	virtual bool disable();
	virtual PixelColor pixel(const Point & point);
	virtual void setPixel(const Point & point , PixelColor color);
	virtual void clear(PixelColor background = White);
	virtual void writeDisplayMemory(size_t startAddress,
									const uint8_t * const sourceBitmap,
									size_t numberOfBits);
	virtual void readDisplayMemory(size_t startAddress,
								   uint8_t * const destinationBitmap,
								   size_t numberOfBits);

	void setContrast(unsigned char contrast);

protected:
	void enableReset(bool enable);
	void enableDisplay(bool enable);
	void configDisplay();

	/**
	 * Returns the color for a given point. The sourceBitmap has the pixel
	 * arranged horizontally. This means sourceBitmap[0] contains pixel from P(0,0) to P(7,0) which is
	 * in contrast to the _lcdBuffer where _lcdBuffer[0] contains pixel from P(0,0) to P(0,7).
	 *
	 * @point Point from which the color is wanted to be known
	 * @sourceBitmap Bitmap reflecting the source from which the color should be extracted
	 * @return Color of the point.
	 */
	inline PixelColor getPixelColorFromBitmap(const Point & point, const uint8_t * const sourceBitmap) const
	{
		uint8_t pixel;

		pixel = sourceBitmap[(point.x + (Width * point.y)) / 8];
		pixel &= (0x01 << ((point.x + (Width * point.y)) % 8));

		return (pixel) ? Black : White;
	}

	/**
	 * Sets the color for a given point to the destinationBitmap
	 *
	 * @destinationBitmap Location where to write the color
	 * @point Point for the given color
	 * @color Color for the given point
	 */
	inline void setPixelColorToBitmap(uint8_t * const destinationBitmap, const Point & point, PixelColor color)
	{
		if (color == IMonochromeDisplay::Black)
		{
			*(destinationBitmap + (point.y * Width + point.x) / 8) |= (1 << ((point.y * Width + point.x) % 8));
		}
		if (color == IMonochromeDisplay::White)
		{
			*(destinationBitmap + (point.y * Width + point.x) / 8) &= ~( 1 << ((point.y * Width + point.x) % 8));
		}
	}

	void update();

private:
	void sendCommand(unsigned char cmd);
	void sendData(unsigned char data);

protected:
	typedef enum
	{
		Width = 84,
		Height = 48,
		LCD_LINE_HEIGHT = 8,					///< LCD puts 8 pixel in on 8-bit register. Formula: Row * LCD_LINE_HEIGHT = y value
		BUFFER_SIZE = ((Width * Height) / 8)
	} eDimensions;

	static const unsigned char _fontTable[][5];
	unsigned char _lcdBuffer[BUFFER_SIZE];
};
