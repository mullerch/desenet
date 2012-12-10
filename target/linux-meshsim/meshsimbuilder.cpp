#include "MeshSimBuilder"
#include <phy/Frame>
#include <IAir3TFactory>
#include <ILed>
#include <IMonochromeDisplay>
#include <IJoystick>
#include <desenet/datalink/DataLink>
#include <air3t/logic/Controller>
#include <test/ledblinker.hpp>
#include <test/painttester.hpp>
#include <test/localtttcontroller.hpp>
#include <test/phytester.hpp>
#include <test/datalinktester.hpp>
#include <test/applicationlayertester.hpp>
#include <test/simpleFsm.hpp>
#include <assert.h>
#include <desenet/Node.hpp>
extern const char * __MESH_NAME;
extern const char * __DATALINK_ID;


MeshSimBuilder::MeshSimBuilder()
{}

MeshSimBuilder::~MeshSimBuilder()
{}

bool MeshSimBuilder::buildApplication( int argc , char ** argv )
{
	// Initialize common subsystems.
	assert( Frame::initializeFramePool( 10 ) );

	// Initialize interface implementations...
	IAir3TFactory & factory = IAir3TFactory::factory( argc , argv );
	assert( factory.led().initialize() );
	assert( factory.monochromeDisplay().initialize() );
	assert( factory.joystick().initialize() );
	factory.phyTransceiver().initialize();

	// Parse command line arguments and start tests if requested...
	int c;
	optind = 0;
	while ( ( c = getopt( argc , argv , "s:p:n:i:t:" ) ) != -1 )
	{
		switch( c )
		{
			case 't':
				if ( strcmp( optarg , "ledblink" ) == 0 )
				{
					new LEDBlinker( factory.led() );
				}
				else if ( strcmp( optarg , "simplefsm" ) == 0 )
				{
					Trace::out( "Starting Simple FSM..." );
					new CdPlayerHandler();
					return true;
				}
				else if ( strcmp( optarg , "ledblinke" ) == 0 )
				{
					Trace::out( "Starting LED blink test (XF)..." );
					new LEDBlinker( factory.led() );
					return true;
				}
				else if ( strcmp( optarg , "painter" ) == 0 )
				{
					Trace::out( "Starting Painter test..." );
					new PaintTester( factory.joystick() , factory.monochromeDisplay() );
					return true;
				}
				else if ( strcmp( optarg , "local" ) == 0 )
				{
					Trace::out( "Starting Local TicTacToe game..." );
					new LocalTTTController();
					return true;
				}
				else if ( strcmp( optarg , "phy" ) == 0 )
				{
					Trace::out( "Starting PHY test..." );
					new PhyTester( factory.phyTransceiver() , factory.joystick() , factory.monochromeDisplay() );
					return true;
				}
#				ifdef DATALINK_LAYER_TESTER_PRESENT
					else if ( strcmp( optarg , "datalink" ) == 0 )
					{
						Trace::out( "Starting Data link test..." );
						DataLink *dl = new DataLink();
						dl->advertiseStart(NULL);

						return true;
						//FIXME make this to compile (original code)
//						Trace::out( "Starting Data link test..." );
//						assert( DataLink().initialize( factory.phyTransceiver() , Node::NodeId::fromHexString( __DATALINK_ID ) ) );
//						new DataLinkTester( factory.phyTransceiver() , factory.joystick() );
//						return true;
					}
#				endif
#				ifdef APPLICATION_LAYER_TESTER_PRESENT
					else if ( strcmp( optarg , "applayer" ) == 0 )
					{
						Trace::out( "Starting Application layer test..." );
						assert( DataLink().initialize( factory.phyTransceiver() , Node::NodeId::fromHexString( __DATALINK_ID ) ) );
						assert( Application().initialize() );
						new ApplicationLayerTester( factory.joystick() , factory.monochromeDisplay() );
						return true;
					}
#				endif
				break;
		}
	}

#ifdef AIR3T_SOLUTION_PRESENT
	// Start the Air3T game if not another test was requested.
	assert( DataLink().initialize( factory.phyTransceiver() , Node::NodeId::fromHexString( __DATALINK_ID ) ) );
	assert( Application().initialize() );
	new Air3T::Controller( __MESH_NAME , factory.monochromeDisplay() , factory.joystick() );
#else
#	warning Air3T Solution is not present. Add here your solution building process or directly inside the main function...
#endif

	return true;
}
