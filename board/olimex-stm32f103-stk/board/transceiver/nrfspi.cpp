#include <phy/Frame>
#include "nrfspi.h"


NrfSpi::NrfSpi(DisplayNrfSharedSpi & sharedSpi)
 : _bInitialized(false),
   _sharedSpi(sharedSpi),
   _bNrfFeaturesEnabled(false)
{

}

NrfSpi::~NrfSpi()
{

}

bool NrfSpi::initialize()
{
	if (!isInitialized())
	{
		_bInitialized = true;

		sharedSpi().initialize();

		return true;
	}
	return false;
}

NrfSpi::tStatus NrfSpi::status()
{
	return sendCmd(NrfSpi::NOP);
//	return readRegister(STATUS);
}

NrfSpi::tStatus NrfSpi::sendCmd(eNrfSpiCmd cmd)
{
	tStatus status = 0x00;

	assert(cmd != R_RX_PAYLOAD);			// Use readFrame() instead
	assert(cmd != W_TX_PAYLOAD);			// Use sendFrame() instead
	assert(cmd != W_REGISTER);				// Use writeRegister() methods instead
	assert(cmd != R_REGISTER);				// Use readRegister() methods instead
	assert(cmd != W_ACK_PAYLOAD);
	assert(cmd != W_TX_PAYLOAD_NO_ACK);

	enableCS();

	switch (cmd)
	{
	case FLUSH_TX:
	case FLUSH_RX:
	case REUSE_TX_PL:
	case NOP:
		status = sendByte(cmd);
		break;
	case ACTIVATE:
		status = sendByte(cmd);
		sendByte(ACTIVATE_DATA);
		break;
	case R_RX_PL_WID:
		status = sendByte(cmd);
		status = sendByte(NOP);
		break;
	case R_RX_PAYLOAD:
	case W_TX_PAYLOAD:
	case W_REGISTER:
	case R_REGISTER:
	case W_ACK_PAYLOAD:
	case W_TX_PAYLOAD_NO_ACK:
		// To nothing
		break;
	}

	disableCS();
	return status;
}

/**
 * Sends a frame to the destination node. Use TX_ADDR register to
 * set the address for the destination node.
 */
NrfSpi::tStatus NrfSpi::sendFrame(const Frame & frame)
{
	tStatus status = 0x00;

	enableCS();

	status = sendByte(W_TX_PAYLOAD);

	for (size_t i = 0; i < frame.payloadSize(); i++)
	{
		sendByte(frame[i]);
	}

	disableCS();
	return status;
}

NrfSpi::tStatus NrfSpi::readFrame(Frame & frame)
{
	tStatus status = 0x00;

	assert(frame.payloadSize());		// Please specify how many bytes you want to read

	enableCS();

	status = sendByte(R_RX_PAYLOAD);

	for (size_t i = 0; i < frame.payloadSize(); i++)
	{
		frame[i] = sendByte(NOP);
	}

	disableCS();
	return status;
}

NrfSpi::tRegister NrfSpi::readRegister(eNrfRegister reg)
{
	tRegister value;

	// All but not these registers are allowed
	assert(!(reg == TX_ADDR || reg == RX_ADDR_P0 || reg == RX_ADDR_P1));

	enableCS();

	sendByte((R_REGISTER << 5) | reg);
	value = sendByte(NOP);

	disableCS();
	return value;
}

NrfSpi::tStatus NrfSpi::readRegister(eNrfRegister reg, tAddress & address)
{
	tStatus status = 0x00;

	// Only these registers are allowed
	assert(reg == TX_ADDR || reg == RX_ADDR_P0 || reg == RX_ADDR_P1);

	enableCS();

	status = sendByte((R_REGISTER << 5) | reg);
	for (size_t i = 0; i < address.addressSize(); i++)
	{
		address[i] = sendByte(NOP);
	}

	disableCS();
	return status;
}

NrfSpi::tStatus NrfSpi::writeRegister(eNrfRegister reg, tRegister value)
{
	tStatus status = 0x00;

	// All but not these registers are allowed
	assert(!(reg == TX_ADDR || reg == RX_ADDR_P0 || reg == RX_ADDR_P1));

	enableCS();

	status = sendByte((W_REGISTER << 5) | reg);
	sendByte(value);

	disableCS();
	return status;
}

NrfSpi::tStatus NrfSpi::writeRegister(eNrfRegister reg, const tAddress & address)
{
	tStatus status = 0x00;

	// Only these registers are allowed
	assert(reg == TX_ADDR || reg == RX_ADDR_P0 || reg == RX_ADDR_P1);

	enableCS();

	status = sendByte((W_REGISTER << 5) | reg);
	for (size_t i = 0; i < address.addressSize(); i++)
	{
		sendByte(address[i]);
	}

	disableCS();
	return status;
}

bool NrfSpi::activateFeatures(bool enable)
{
	// Check if not already in this state
	if (_bNrfFeaturesEnabled != enable)
	{
		sendCmd(NrfSpi::ACTIVATE);
		_bNrfFeaturesEnabled = enable;
		return true;
	}
	return false;
}

unsigned char NrfSpi::getRXFramePipeNumber()
{
	unsigned char pipeNumber;
	tStatus status = NrfSpi::status();

	// Extract RX_P_NO field
	pipeNumber = ((status >> 1) & 0x07);

	if (!(pipeNumber == 0x07 || pipeNumber == 0x06))
	{
		return pipeNumber;
	}
	assert(false);	// Call this method only when you are sure there is a packet available
	return 0xFF;
}

size_t NrfSpi::getRXFramePaylpoadSize(unsigned char pipeNumber)
{
	assert(pipeNumber >= 0 && pipeNumber < 6);

	if (!featuresEnabled())
	{
		return readRegister((NrfSpi::eNrfRegister)(RX_PW_P0 + pipeNumber));
	}
	else
	{
		// DPL. Read value of next frame in RX FIFO by reading the R_RX_PL_WID register
		return sendCmd(R_RX_PL_WID);
	}
}

NrfSpi::tStatus NrfSpi::sendByte(unsigned char data)
{
	return sharedSpi().sendByte(data);
}
