#pragma once
#include "idatalinkobserver.h"
#include <IJoystickObserver>
#include <IPhyTransceiver>
#include <IJoystick>
#include <IMonochromeDisplay>
#include <phy/Frame>
#include <trace/Trace>

#define DATALINK_LAYER_TESTER_PRESENT

/**
 * @ingroup test
 * Tests the PHY interface by sending the joystick events to all reachable nodes and the received events will be used
 * to draw into the display.
 */
class DataLinkTester : public IJoystickObserver, public IDataLinkObserver
{
public:
	/**
	 * Initializes all needed components and prepares the test object itself.
	 */
	DataLinkTester( DataLink & datalink , IJoystick & joystick )
		: _datalink( datalink ) , _joystick( joystick )
	{
		_datalink.setObserver( this );
		//_transceiver.setReceptionFilterAddress( IPhyTransceiver::FilterAddress::fromHexString( "8E89BED6" ) );

		_joystick.setObserver( this );
	}

	void onPositionChange( IJoystick::Position position )
	{
		if ( position != IJoystick::Center )
		{
			//DataLink();
			Trace::out( "Action!" );
			//_datalink.advertiseStart(NULL);
		}
	}

	virtual void onNodeAppear( const Node &node ) {};
	virtual void onNodeDissapear( const Node &node ) {};
	virtual void onConnectConfirm( ConnectStatus status ){};
	virtual void onDataIndication( const Frame &frame ){}

private:
	DataLink & _datalink;
	IJoystick & _joystick;
};
