#pragma once
#include <IPhyObserver>
#include <IJoystickObserver>
#include <IPhyTransceiver>
#include <IJoystick>
#include <IMonochromeDisplay>
#include <phy/Frame>
#include <trace/Trace>
#include <utils/MonochromeDisplayPainter>

#define DATALINK_LAYER_TESTER_PRESENT

/**
 * @ingroup test
 * Tests the PHY interface by sending the joystick events to all reachable nodes and the received events will be used
 * to draw into the display.
 */
class DataLinkTester : public IPhyObserver , public IJoystickObserver
{
public:
	/**
	 * Initializes all needed components and prepares the test object itself.
	 */
	DataLinkTester( IPhyTransceiver & transceiver , IJoystick & joystick , IMonochromeDisplay & display )
		: _transceiver( transceiver ) , _joystick( joystick ) , _display( display ) ,_p( 42 , 24 )
	{
		_transceiver.setObserver( this );
		_transceiver.setReceptionFilterAddress( IPhyTransceiver::FilterAddress::fromHexString( "8E89BED6" ) );

		_joystick.setObserver( this );

		_display.clear();
	}

	void onReceive( const Frame & frame )
	{

	}

	void onSendStatus( SendStatus status )
	{

	}

	void onPositionChange( IJoystick::Position position )
	{

	}

private:
	IPhyTransceiver & _transceiver;
	IJoystick & _joystick;
	IMonochromeDisplay & _display;
	IMonochromeDisplay::Point _p;
};
