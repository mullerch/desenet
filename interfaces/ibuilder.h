#pragma once

#ifndef NULL
#define NULL 0
#endif

/**
 * @ingroup interfaces
 * @brief This Interfaces has to be implemented by a system builder in order to create the needed instances of classes
 * 		  that implement these interfaces and to initialize the relations between these objects.
 */
class IBuilder
{
protected:
	// It is just an interface, so constructor and destructor should be not public.
	explicit IBuilder() {}
	virtual ~IBuilder() {}

public:
	/**
	 * @brief Creates all needed objects for the application. It responsible to instantiate the needed
	 * 		  instances in the right order by calling the Factories instantiation methods and
	 * 		  initializes the instances accordingly.
	 *
	 * @param argc 	Argument count, just pass the argc parameter of the main function here.
	 * @param argv 	Arguments vector, just pass the argv parameter of the main function here.
	 * @return 		True in case the builder could properly set up and initialize the needed instances.
	 */
	virtual bool buildApplication( int argc = 0 , char ** argv = NULL ) = 0;
};
