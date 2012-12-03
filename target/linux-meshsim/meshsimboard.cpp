#include "MeshSimBoard"
#include <stdint.h>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QPoint>
#include <IJoystickObserver>
#include <assert.h>


/*** Mesh Simulator options. *****************************************************************************************/

extern bool 		__XF_PAUSED;		// External boolean to stop XF execution.


/*** MeshSimBoard implementation *************************************************************************************/

IMonochromeDisplay::Size MeshSimBoard::_DisplaySize( 84 , 48 );

MeshSimBoard::MeshSimBoard() : QWidget( NULL ) , _ready( false ) , _displayEnabled( true ) , _ledState( ILed::Off ) ,
							   _position( IJoystick::Center ) ,
							   _display( 84 , 48 , QImage::Format_Mono ) , _joystickObserver( NULL )
{
	// Set the window size to the exact size of the simulation board background image.
	setFixedSize( 295 , 437 );

	// Use the same title for the window as for the application.
	setWindowTitle( QApplication::applicationName() );

	// Set the color table for the off-screen display image.
	_display.setColorTable( QVector<QRgb>() << qRgba( 255 , 255 , 255 , 16 ) << qRgba( 0 , 0 , 0 , 200 ) );
}

MeshSimBoard::~MeshSimBoard()
{}

bool MeshSimBoard::initialize()
{
	// Initialize only once...
	if ( !_ready )
	{
		// Load UI images.
		_background.load( QApplication::applicationDirPath() +
						  "/../target/linux-meshsim/resources/STM32F103-STK.png" );
		_led.load( QApplication::applicationDirPath() +
				   "/../target/linux-meshsim/resources/STM32F103-STK-LEDON.png" );

		_joystick.load( QApplication::applicationDirPath() +
				   	   	"/../target/linux-meshsim/resources/STM32F103-STK-JOYSTICK.png" );

		// Show widget.
		QWidget::show();

		// We want to get always keyboard input.
		grabKeyboard();

		// Initialize LCD.
		_display.fill( Qt::color0 );

		// Ok, we are ready...
		_ready = true;
	}

	return true;
}

IMonochromeDisplay::Size MeshSimBoard::resolution() const
{
	assert( _ready );
	if ( _ready )
		return _DisplaySize;
	else
		return Size( 0 , 0 );
}

IMonochromeDisplay::AcceleratedDrawingMethods MeshSimBoard::acceleratedDrawingMethods() const
{
	static AcceleratedDrawingMethods accelerationMethods = { 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 };

	assert( _ready );
	return accelerationMethods;
}

bool MeshSimBoard::enable()
{
	assert( _ready );
	_displayEnabled = true;
	update();
	return _ready;
}

bool MeshSimBoard::disable()
{
	assert( _ready );
	_displayEnabled = false;
	update();
	return _ready;
}

void MeshSimBoard::clear( IMonochromeDisplay::PixelColor background )
{
	assert( _ready );
	if ( _ready && _displayEnabled )
	{
		// Use selected background color.
		switch ( background )
		{
			case IMonochromeDisplay::White:
				_display.fill( Qt::color0 );
				break;

			case IMonochromeDisplay::Black:
				_display.fill( Qt::color1 );
				break;

			default:
				// The default is white.
				_display.fill( Qt::color0 );
				break;
		}

		// Update simulation UI.
		update();
	}
}

IMonochromeDisplay::PixelColor MeshSimBoard::pixel( const IMonochromeDisplay::Point & point )
{
	assert( _ready );
	if ( _ready && _displayEnabled && point.x < _DisplaySize.width && point.x >= 0 &&
			point.y < _DisplaySize.height && point.y >= 0 )
	{
		// Return the color of the pixel at the given position.
		if ( _display.pixelIndex( QPoint( point.x , point.y ) ) ) return IMonochromeDisplay::Black;
		else return IMonochromeDisplay::White;
	}

	return IMonochromeDisplay::Undefined;
}

void MeshSimBoard::setPixel( const IMonochromeDisplay::Point & point , IMonochromeDisplay::PixelColor color )
{
	assert( _ready );
	if ( _ready && _displayEnabled && point.x < _DisplaySize.width && point.x >= 0 &&
		 point.y < _DisplaySize.height && point.y >= 0  && color != IMonochromeDisplay::Undefined )
	{
		// Change the color of the pixel.
		_display.setPixel( QPoint( point.x , point.y ) , color == White ? Qt::color0 : Qt::color1 );

		// Update simulated UI.
		update();
	}
}

void MeshSimBoard::writeDisplayMemory( size_t startAddress , const uint8_t * const sourceBitmap , size_t numberOfBits )
{
	assert( _ready );
	if ( _ready && _displayEnabled && sourceBitmap )
	{
		// We use just the setPixel method here, since the bitmap lines have to be aligned 32 bit.
		for ( size_t i = 0 ; i < numberOfBits ; ++i )
		{
			_display.setPixel( ( i + startAddress * 8 ) % 84 ,
					           ( i + startAddress * 8 ) / 84 ,
					           *( sourceBitmap + ( i / 8 ) ) & ( 1 << ( i % 8 ) ) ? 1 : 0 );
		}

		update();
	}
}

void MeshSimBoard::readDisplayMemory( size_t startAddress , uint8_t * const destinationBitmap , size_t numberOfBits )
{
	assert( _ready );
	if ( _ready && _displayEnabled && destinationBitmap )
	{
		// Clear target bitmap.
		memset( destinationBitmap , 0 , numberOfBits / 8 );
		for ( size_t i = 0 ; i < numberOfBits ; ++i )
		{
			// Copy pixels into the bitmap. Note that we use the pixelIndex() method, since the data is 32 bit aligned.
			if ( _display.pixelIndex( ( i + startAddress * 8 ) % 84 , ( i + startAddress * 8 ) / 84 ) == Qt::color1 )
				*( destinationBitmap + ( i / 8 ) ) |= 1 << ( i % 8 );
		}
	}
}

bool MeshSimBoard::setObserver( IJoystickObserver * observer )
{
	assert(_ready );

	// We can have only one observer.
	_joystickObserver = observer;
	return observer;
}

IJoystick::Position MeshSimBoard::position() const
{
	assert( _ready );
	return _position;
}

ILed::State MeshSimBoard::state() const
{
	assert( _ready );
	return _ledState;
}

void MeshSimBoard::setState( State state )
{
	assert( _ready );
	// Change the led and update the UI only if really needed.
	if ( state != _ledState )
	{
		_ledState = state;
		update();
	}
}

void MeshSimBoard::paintEvent( QPaintEvent * event )
{
	QPainter painter( this );

	// Draw opaque if the XF has been stopped.
	if ( __XF_PAUSED ) painter.setOpacity( 0.25 );

	// Draw background.
	painter.drawPixmap( 0 , 0 , _background );

	// Draw shiny Led pixmap, if led is in state On.
	if ( _ledState == ILed::On ) painter.drawPixmap( 42  , 396 , _led );

	// Draw joystick position.
	QPoint joystickPosition( 144 , 391 );
	if ( _position & IJoystick::Left ) joystickPosition += QPoint( -3 , 0 );
	if ( _position & IJoystick::Up ) joystickPosition += QPoint( 0 , -3 );
	if ( _position & IJoystick::Right ) joystickPosition += QPoint( 3 , 0 );
	if ( _position & IJoystick::Down ) joystickPosition += QPoint( 0 , 3 );
	painter.drawPixmap( joystickPosition , _joystick );

	// Draw off-screen display if it is enabled.
	if ( _displayEnabled )
		painter.drawImage( QRectF( 65 , 222 , 2 * 84 , 2 * 48 ) , _display );
}

void MeshSimBoard::mousePressEvent( QMouseEvent * event )
{
	// Depending which direction the joystick was moved, call the hander of the observer.
	if ( QRect( 138 , 363 , 26 , 26 ).contains( event->pos() ) )
		_position = IJoystick::Up;
	else if ( QRect( 116 , 385 , 26 , 26 ).contains( event->pos() ) )
		_position = IJoystick::Left;
	else if ( QRect( 138 , 406 , 26 , 26 ).contains( event->pos() ) )
		_position = IJoystick::Down;
	else if ( QRect( 160 , 385 , 26 , 26 ).contains( event->pos() ) )
		_position = IJoystick::Right;
	else if ( QRect( 138 , 385 , 26 , 26 ).contains( event->pos() ) )
		_position = IJoystick::Pressed;
	else if ( QRect( 240 , 93 , 35 , 16 ).contains( event->pos() ) )
		QApplication::quit();
	else
		return;

	// We need to have a registered observer for joystick events.
	if ( _joystickObserver && !__XF_PAUSED )
	{
		_joystickObserver->onPositionChange( _position );
	}

	// Update UI.
	update();
}

void MeshSimBoard::mouseReleaseEvent( QMouseEvent * event )
{
	// React only if there was a position other than Center.
		if ( _position != IJoystick::Center )
		{
			_position = IJoystick::Center;

			// We need to have a registered observer for joystick events.
			if ( _joystickObserver && ! __XF_PAUSED )
			{
				_joystickObserver->onPositionChange( _position );
			}

			// Update UI.
			update();
		}
}

void MeshSimBoard::keyPressEvent( QKeyEvent * event )
{
	// Depending which key was pressed, call the hander of the observer.
	if ( event->key() == Qt::Key_Up ) _position = IJoystick::Up;
	else if ( event->key() == Qt::Key_Left ) _position = IJoystick::Left;
	else if ( event->key() == Qt::Key_Down ) _position = IJoystick::Down;
	else if ( event->key() == Qt::Key_Right ) _position = IJoystick::Right;
	else if ( event->key() == Qt::Key_Enter ||
			   event->key() == Qt::Key_Return ) _position = IJoystick::Pressed;
	else return;

	// We need to have a registered observer for joystick events.
	if ( _joystickObserver && ! __XF_PAUSED )
	{
		_joystickObserver->onPositionChange( _position );
	}

	// Update UI.
	update();
}

void MeshSimBoard::keyReleaseEvent( QKeyEvent * event )
{
	// React only if there was a position other than Center.
	if ( _position != IJoystick::Center )
	{
		_position = IJoystick::Center;

		// We need to have a registered observer for joystick events.
		if ( _joystickObserver && ! __XF_PAUSED )
		{
			_joystickObserver->onPositionChange( _position );
		}

		// Update UI.
		update();
	}
}
