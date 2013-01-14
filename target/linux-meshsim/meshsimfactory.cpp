#include "MeshSimFactory"
#include "MeshSimTransceiver"
#include "MeshSimBoard"
#include "MeshSimBuilder"
#include <phy/Frame>
#include <QtCore/QByteArray>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <assert.h>


/*** Hidden Mesh Simulator options ***********************************************************************************/

const char * 	__MESH_SERVER	= "127.0.0.1";
uint16_t 		__MESH_PORT	 	= 8844;
const char * 	__MESH_NAME 	= "No name";
const char *    __DATALINK_ID	= "191277310808";


/*** MeshSim Factory implementation. *********************************************************************************/

MeshSimFactory::MeshSimFactory( int argc , char ** argv ) : _transceiver( NULL ) , _board( NULL ), _datalink( NULL )
{
	// Parse command line arguments...
	int c;
	while ( ( c = getopt( argc , argv , "s:p:n:i:t:" ) ) != -1 )
	{
		switch( c )
		{
			case 's':
				__MESH_SERVER = optarg;
				break;

			case 'p':
				__MESH_PORT = atoi( optarg );
				break;

			case 'n':
				__MESH_NAME = optarg;
				break;

			case 'i':
				__DATALINK_ID = optarg;
				break;
		}
	}
}

MeshSimFactory::~MeshSimFactory()
{}

DataLink & MeshSimFactory::dataLink(){
	// Create transceiver object on demand.
	if ( !_datalink ) _datalink = new DataLink();
	assert( _datalink );

	// Return the reference to the object.
	return *_datalink;
}

IPhyTransceiver & MeshSimFactory::phyTransceiver()
{
	// Create transceiver object on demand.
	if ( !_transceiver ) _transceiver = new MeshSimTransceiver();
	assert( _transceiver );

	// Return the reference to the object.
	return *_transceiver;
}

IMonochromeDisplay & MeshSimFactory::monochromeDisplay()
{
	// Return the reference to the object.
	return _instanciateBoard();
}

IJoystick & MeshSimFactory::joystick()
{
	// Return the reference to the object.
	return _instanciateBoard();
}

ILed & MeshSimFactory::led()
{
	// Return the reference to the object.
	return _instanciateBoard();
}

QWidget * MeshSimFactory::boardWidget()
{
	return &_instanciateBoard();
}

MeshSimBoard & MeshSimFactory::_instanciateBoard()
{
	// Create the board object on demand.
	if ( !_board ) _board = new MeshSimBoard();
	assert( _board );

	// Return the reference to the object.
	return *_board;
}

IAir3TFactory & IAir3TFactory::factory( int argc , char ** argv )
{
	static MeshSimFactory * factory = NULL;

	// Seed the random number generator.
	srand( time( NULL ) );

	// Create the factory object on demand.
	if ( !factory ) factory = new MeshSimFactory( argc , argv );
	assert( factory );
	QApplication::setApplicationName( __MESH_NAME );

	// Return the reference to the object.
	return *factory;
}

IBuilder & IAir3TFactory::builder()
{
	static MeshSimBuilder builder;
	return builder;
}
