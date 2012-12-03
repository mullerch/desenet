#include <IAir3TFactory>
class MeshSimTransceiver;
class MeshSimBoard;
class QWidget;


class MeshSimFactory : public IAir3TFactory
{
public:
	// Constructor and destructor, note that the constructor parses the application arguments.
	MeshSimFactory( int argc , char ** argv );
	virtual ~MeshSimFactory();

	// IAir3TFactory implementation.
	IPhyTransceiver & phyTransceiver();
	IMonochromeDisplay & monochromeDisplay();
	IJoystick & joystick();
	ILed & led();
	//static IAir3TFactory & factory( int argc = 0 , char ** argv = NULL );
	//static IBuilder & builder();

	// Package interface to get a pointer to the widget simulating the device.
	QWidget * boardWidget();

private:
	// We implement 3 interfaces (IMonochromeDisplay,IJoystick and ILed) with just one class.
	MeshSimBoard & _instanciateBoard();

	// The pointers to the objects...
	MeshSimTransceiver * _transceiver;
	MeshSimBoard * _board;
};
