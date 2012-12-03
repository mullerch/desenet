#pragma once


#include <target.h>


/**
 * HAL class abstracting an SPI peripheral
 */
class SPI
{
	friend class Factory;

public:
	typedef unsigned char tStatus;

	bool initialize();

	void enable();
	void disable();
	void resetStateMachine();

	bool getFlagStatus(unsigned short flagMask);

	tStatus sendByte(unsigned char data);

protected:
	/**
	 * Constructor of the class. Only Factory class is
	 * allowed to create instances of this class
	 *
	 * \param spiNbr The SPI to be accessed. Valid numbers are 0 to 2
	 */
	SPI(int spiNbr);	// Only Factory class is allowed to create instances of this class
	virtual ~SPI();

protected:
	bool _bInitialized;
	SPI_TypeDef* _pSPI;						///< Reference to the SPI structure
};
