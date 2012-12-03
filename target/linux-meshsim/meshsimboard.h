#pragma once
#include <QtGui/QWidget>
#include <IMonochromeDisplay>
#include <IJoystick>
#include <ILed>
#include <QtGui/QPixmap>
#include <QtGui/QImage>


class MeshSimBoard : public QWidget , public virtual IMonochromeDisplay ,
					 public virtual IJoystick , public virtual ILed
{
public:
	// Constructor and destructor.
	MeshSimBoard();
	virtual ~MeshSimBoard();

	// Interface implementation for IMonochromeDisplay, IJoystick and ILed.
	bool initialize();

	// Interface implementation for IMonochromeDisplay.
	Size resolution() const;
    AcceleratedDrawingMethods acceleratedDrawingMethods() const;
	bool enable();
	bool disable();
	void clear( PixelColor background );
	PixelColor pixel( const Point & point );
	void setPixel( const Point & point , PixelColor color );
	void writeDisplayMemory( size_t startAddress , const uint8_t * const sourceBitmap , size_t numberOfBits );
	void readDisplayMemory( size_t startAddress , uint8_t * const destinationBitmap , size_t numberOfBits );

	// Interface implementation for IJoystick.
	bool setObserver( IJoystickObserver * observer );
	Position position() const;

	// Interface implementation for ILed.
	State state() const;
	void setState( State state );

protected:
	// Qt event reception handlers.
	void paintEvent( QPaintEvent * event );
	void mousePressEvent( QMouseEvent * event );
	void mouseReleaseEvent( QMouseEvent * event );
	void keyPressEvent( QKeyEvent * event );
	void keyReleaseEvent( QKeyEvent * event );

private:
	static IMonochromeDisplay::Size _DisplaySize;		// The display size (fixed).

	bool _ready;										// Are we initialized?
	bool _displayEnabled;								// Is the display enabled?
	ILed::State _ledState;								// The actual state of the LED.
	IJoystick::Position _position;						// The actual position of the joystick.
	QImage _display;									// Display buffer.

	IJoystickObserver * _joystickObserver;				// Pointer to the joystick observer object.

	QPixmap _background;								// Pixmap containing the rendered background.
	QPixmap _led;										// Pixmap containing the enlighten LED image.
	QPixmap _joystick;									// Pixmap containing the joystick.
};
