#pragma once


#include <IBuilder>


/**
 * Is responsible to build (set up) the whole application. It
 * asks the Factory for the needed instances of the different classes
 * and establishes the relations between them.
 */
class Builder : public IBuilder
{
public:
	Builder();
	virtual ~Builder();

	// Interface operations from IBuilder
	bool buildApplication(int argc = 0, char ** argv = NULL);
};

