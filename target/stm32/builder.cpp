#include "inc/config.h"
#include <IAir3TFactory>
#include <hal/uart.h>
#include <trace/trace.h>
#include "inc/builder.h"
#include "inc/factory.h"
#include <board/joystick.h>
#include <IPhyTransceiver>
#include <IMonochromeDisplay>
#include <utils/MonochromeDisplayPainter>
#include <phy/Frame>

#if (USE_USB_TRACE != 0)
#	include <board/usb-vcom/usb-vcom.h>
#endif

#ifdef LEDBLINK_TESTER
#	include <test/ledblinker.hpp>
#endif

#ifdef PAINT_TESTER
#	include <test/painttester.hpp>
#endif

#ifdef FSM_TESTER
#	include <test/simpleFsm.hpp>
#endif

#ifdef LOCALTTT_TESTER
#	include <test/localtttcontroller.hpp>
#endif

#ifdef PHY_TESTER
#	include <test/phytester.hpp>
#endif

#ifdef AIR3T_APPLICATION
#	include <desenet/application/Application>
#	include <desenet/datalink/DataLink>
#	include <air3t/logic/Controller>

	using namespace DeSeNet;
#endif


Builder::Builder()
{

}

Builder::~Builder()
{

}

bool Builder::buildApplication(int argc, char ** argv)
{
	Factory & factory =  static_cast<Factory &>(IAir3TFactory::factory(argc, argv));

	// Initialize frame pool used by Frame instances
	Frame::initializeFramePool(3);

#if (USE_USART2_TRACE != 0)
	// Create instance of UART (USART2)
	factory.uart(1).initialize(115200);
#endif

#if (USE_USB_TRACE != 0)
    // Init board specific stuff for VCOM over USB
    board::usbVcomHwConfigInit();
#endif

	Trace::out("---------------------------------------------------------------");
	Trace::out("Initializing application...");

	// Create instance representing the joystick buttons and initialize it
	factory.joystick().initialize();

	// Initialize the LedController
	factory.led().initialize();

	// Initialize SPI used by monochrome display & nRF chip
	factory.sharedSpi().initialize();

	// Initialize display
	factory.monochromeDisplay().initialize();

#ifdef LEDBLINK_TESTER
	new LEDBlinker( factory.led() );
#endif

#ifdef PAINT_TESTER
	new PaintTester(factory.joystick() , factory.monochromeDisplay());

	MonochromeDisplayPainter painter(&factory.monochromeDisplay(), false);
	painter.clear();
	painter.drawText(IMonochromeDisplay::Point(15, 3*8), "Ready...", IMonochromeDisplay::Black);
	painter.end();
#endif

#ifdef FSM_TESTER
	new CdPlayerHandler();
#endif

#ifdef LOCALTTT_TESTER
	new LocalTTTController();
#endif

#ifdef PHY_TESTER
	Trace::out("Initializing PhyTester");
	// Create instance of NrfTransceiver and initialize it.
	factory.phyTransceiver().initialize();

	new PhyTester(factory.phyTransceiver(),
				  factory.joystick(),
				  factory.monochromeDisplay());
#endif

#ifdef DATALINK_TESTER
	// Initialize first DataLink. Otherwise DataLinkTester will not work
#	if (DATALINK_TESTER_NODE_A == 1)
		// Create Node A
		Trace::out("Creating Node A");
		DataLink().initialize(factory.phyTransceiver(), Node::NodeId::fromHexString("000000000001"));
#	else
		// Create Node B
		Trace::out("Creating Node B");
		DataLink().initialize(factory.phyTransceiver(), Node::NodeId::fromHexString("000000000002"));
#	endif
	// Initialize data link tester
	static DataLinkTester tester(factory.phyTransceiver() , factory.joystick());
#endif

#ifdef AIR3T_APPLICATION
	// Initialize first DataLink. Otherwise DataLinkTester will not work
	Trace::out( "Creating " AIR3T_APPLICATION_NODENAME );
	DataLink().initialize(factory.phyTransceiver(), Node::NodeId::fromHexString( AIR3T_APPLICATION_NODEID ));
	Application().initialize();
	new Air3T::Controller(AIR3T_APPLICATION_NODENAME, factory.monochromeDisplay(), factory.joystick());
#endif

	Trace::out("Starting application...");

	return true;
}
