#pragma once
#include <IPhyObserver>
#include <IJoystickObserver>
#include <IPhyTransceiver>
#include <IJoystick>
#include <IMonochromeDisplay>
#include <phy/Frame>
#include <trace/Trace>
#include <utils/MonochromeDisplayPainter>


/**
 * @ingroup test
 * Tests the PHY interface by sending the joystick events to all reachable nodes and the received events will be used
 * to draw into the display.
 */
class PhyTester : public IPhyObserver , public IJoystickObserver
{
public:
	/**
	 * Initializes all needed components and prepares the test object itself.
	 */
	PhyTester( IPhyTransceiver & transceiver , IJoystick & joystick , IMonochromeDisplay & display )
		: _transceiver( transceiver ) , _joystick( joystick ) , _display( display ) ,_p( 42 , 24 )
	{
		_transceiver.setObserver( this );
		_transceiver.setReceptionFilterAddress( IPhyTransceiver::FilterAddress::fromHexString( "8E89BED6" ) );

		_joystick.setObserver( this );

		_display.clear();
	}

	void onReceive( const Frame & frame )
	{
		IJoystick::Position position = static_cast<IJoystick::Position>( frame[0] );

		switch ( position )
		{
			case IJoystick::Up:
				_p.y -= 1;
				break;

			case IJoystick::Down:
				_p.y += 1;
				break;

			case IJoystick::Left:
				_p.x -= 1;
				break;

			case IJoystick::Right:
				_p.x += 1;
				break;

			case IJoystick::Pressed:
				_display.clear();
				break;

			default:
				break;
		}

		MonochromeDisplayPainter( &_display , false ).drawPixel( _p , IMonochromeDisplay::Black );
	}

	void onSendStatus( SendStatus status )
	{
		if ( status != IPhyObserver::Delivered )
			Trace::out( "Error sending frame!" );
	}

	void onPositionChange( IJoystick::Position position )
	{
		if ( position != IJoystick::Center )
		{
			Frame frame( IPhyTransceiver::FilterAddress::fromHexString( "8E89BED6" ) , 1 );
			frame[0] = position;
			_transceiver.send( frame );
		}
	}

private:
	IPhyTransceiver & _transceiver;
	IJoystick & _joystick;
	IMonochromeDisplay & _display;
	IMonochromeDisplay::Point _p;
};
