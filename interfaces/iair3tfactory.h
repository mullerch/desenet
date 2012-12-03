#pragma once
class IPhyTransceiver;
class IMonochromeDisplay;
class IJoystick;
class ILed;
class IBuilder;

#ifndef NULL
#define NULL 0
#endif

/**
 * @defgroup interfaces Interfaces
 * @brief The interfaces package offers all needed interfaces to abstract the differences between the targets the software
 * 		  can run and offers a common way to use the different hardware components from a high-level point of view.
 * @{
 *
 * @section interfcaces_cd Class Diagram
 * The following class diagram shows the relations between the different classes inside the Interface package. The
 * Interface package is used to have a common interface for the high-level software which does not depend the
 * actual target the software is runnung on.
 * @image html Interfaces.png
 *
 * IJoystick is used to controll the real hardware joystick on the Olimex development board or the virtual joystick
 * on the simulated device. The IJoystickObserver interface enables to get notified about joystick events.
 *
 * The IPhyTransceiver interface abstracts the used transceiver and enables the application to configure the PHY and
 * send frames. The IPhyObserver interface allows again to get notified about PHY events which can be either received
 * frames or the status of sent frames.
 *
 * The ILed interface allows the control of the LED in a transparent manner for the real target and the simulator.
 *
 * The IMonochromeDisplay interface abtracts monochrome LCD displays. The interface allows different interfaces with
 * their different accelerated drawing methods. The Point and Size classes inside the class are used to describe
 * points and sizes of objects to draw.
 *
 * The IAir3TFactory is used to create the objects that implement the interfaces for the different supported
 * platforms. The IBuilder interface initializes the objects and establishes the relations between them for
 * the different platforms.
 */


/**
 * @brief This factory interface defines the interface used to instantiate target specific classes that implement a
 * common interface (interfaces inside this folder) and to get the references to singleton objects that implement an
 * common interface. This class has to be implemented on each target and is the only binding between the interfaces
 * and the respective implementations on the different targets.
 */
class IAir3TFactory
{
protected:
	// It is just an interface, so constructor and destructor should be not public.
	explicit IAir3TFactory() {}
	virtual ~IAir3TFactory() {}

public:
	/**
	 * @brief Returns a reference to the object that implements the IPhyTransceiver interface and which enables the
	 * 		  application to communicate.
	 *
	 * @return Reference to the single object that implements the IPhyTrasnceiver interface for the current target.
	 */
	virtual IPhyTransceiver & phyTransceiver() = 0;

	/**
	 * @brief Returns a reference to the object that implements the IMonochromeDisplay interface and which enables the
	 * 		  application to draw onto the screen of the target.
	 *
	 * @return Reference to the single object that implements the IMonochromeDisplay interface for the current target.
	 */
	virtual IMonochromeDisplay & monochromeDisplay() = 0;

	/**
	 * @brief Returns a reference to the object that implements the IJoystick interface and which enables the
	 * 		  application to use the targets joystick as input device.
	 *
	 * @return Reference to the single object that implements the IJoystick interface for the current target.
	 */
	virtual IJoystick & joystick() = 0;

	/**
	 * @brief Returns a reference to the object that implements the ILed interface and which enables the application to
	 * 		  change the state of the LED of the current target via the LED interface.
	 *
	 * @return Reference to the single object that implements the ILed interface for the current target.
	 */
	virtual ILed & led() = 0;

	/**
	 * @brief Returns a reference to the target's factory implementation. Note that this should be the only binding to the
	 * 		  target.
	 *
	 * @param argc 	Argument count, just pass the argc parameter of the main function here.
	 * @param argv 	Arguments vector, just pass the argv parameter of the main function here.
	 * @return		Reference to the target's factory.
	 */
	static IAir3TFactory & factory( int argc = 0 , char ** argv = NULL );

	/**
	 * Returns a reference to the builder instance. The builder is responsible to set up
	 * the application structure prior to launch the application.
	 *
	 * @return Reference to the single instance of IBuilder
	 */
	static IBuilder & builder();
};

//@}
