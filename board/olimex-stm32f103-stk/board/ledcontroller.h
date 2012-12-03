#pragma once


#include <ILed>


class LedController : public ILed
{
public:
	LedController();
	virtual ~LedController();

	// Implementation of the ILed interface
	bool initialize();
	State state() const;
	void setState(State state);

	void setLed(int index, bool bEnable);

protected:
	bool _bInitialized;			///< True after the call to the #initialize method.
};

