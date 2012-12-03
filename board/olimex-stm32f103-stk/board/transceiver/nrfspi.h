#pragma once

#include <target.h>
#include <utils/Address>
#include <board/shared-spi/displaynrfsharedspi.h>


class Frame;


class NrfSpi
{
public:
	NrfSpi(DisplayNrfSharedSpi & sharedSpi);
	virtual ~NrfSpi();

	bool initialize();													///< Initializes the NrfSpi. Should be called after construction.
	inline bool isInitialized() const { return _bInitialized; }			///< Returns true if the #initialize method was already called.

	typedef unsigned char tStatus;
	typedef unsigned char tRegister;
	typedef Address<4>	tAddress;		///< Type used for source and destination address
	typedef enum
	{
		R_REGISTER			= 0x00,
		W_REGISTER			= 0x01,
		R_RX_PAYLOAD		= 0x61,
		W_TX_PAYLOAD		= 0xA0,
		FLUSH_TX 			= 0xE1,
		FLUSH_RX			= 0xE2,
		REUSE_TX_PL			= 0xE3,
		ACTIVATE			= 0x50,
		R_RX_PL_WID			= 0x60,
		W_ACK_PAYLOAD		= 0xA1,		// Not complete. Use it as mask.
		W_TX_PAYLOAD_NO_ACK	= 0xB0,
		NOP					= 0xFF
	} eNrfSpiCmd;

	typedef enum
	{
		CONFIG			= 0x00,
		EN_AA			= 0x01,
		EN_RXADDR		= 0x02,
		SETUP_AW		= 0x03,
		SETUP_RETR		= 0x04,
		RF_CH			= 0x05,
		RF_SETUP		= 0x06,
		STATUS			= 0x07,
		OBSERVE_TX		= 0x08,
		CD				= 0x09,
		RX_ADDR_P0		= 0x0A,
		RX_ADDR_P1		= 0x0B,
		TX_ADDR			= 0x10,
		RX_PW_P0		= 0x11,
		RX_PW_P1		= 0x12,
		FIFO_STATUS		= 0x17,
		DYNPD			= 0x1C,
		FEATURE			= 0x1D
	} eNrfRegister;

	static const unsigned char CONFIG_REG_PRIM_RX	= 0x01;
	static const unsigned char CONFIG_REG_PWR_UP	= 0x02;
	static const unsigned char CONFIG_REG_CRCO		= 0x04;
	static const unsigned char CONFIG_REG_EN_CRC	= 0x08;

	static const unsigned char STATUS_REG_TX_FULL	= 0x01;
	static const unsigned char STATUS_REG_MAX_RT	= 0x10;
	static const unsigned char STATUS_REG_RX_DR		= 0x40;
	static const unsigned char STATUS_REG_TX_DS		= 0x20;

	static const unsigned char FEATURE_REG_EN_DPL	= 0x04;
	static const unsigned char ACTIVATE_DATA		= 0x73;

	tStatus status();														///< Returns the status byte from the transceiver.

	tStatus sendCmd(eNrfSpiCmd cmd);										///< Sends a command to the transceiver. See eNrfSpiCmd for possible commands.
	tStatus sendFrame(const Frame & frame);									///< Sends a frame to the transceiver.
	tStatus readFrame(Frame & frame);										///< Reads n-bytes from RX FIFO according to the payload size specified in frame

	tRegister readRegister(eNrfRegister reg);								///< Used to read the transceivers registers (one byte registers)
	tStatus readRegister(eNrfRegister reg, tAddress & address);				///< Used to read the transceivers address registers (3 - 5 byte registers)

	tStatus writeRegister(eNrfRegister reg, tRegister value);				///< Writes a value to the transceivers register
	tStatus writeRegister(eNrfRegister reg, const tAddress & address);		///< Writes an address to the transceivers register

	bool activateFeatures(bool enable);										///< Activates some features of the transceiver chip
	inline bool featuresEnabled() const { return _bNrfFeaturesEnabled; }	///< Returns true if the features of the transceiver are enabled

	unsigned char getRXFramePipeNumber();									///< Returns the pipe number for the next waiting RX frame in the RX FIFO
	size_t getRXFramePaylpoadSize(unsigned char pipeNumber);				///< Returns the size of the next waiting RX frame

protected:
	#define CSN_TIME      2
	#define CE_HIGH_TIME  10000

	inline void sDelay(unsigned long a) const { while (--a!=0); }
	inline void enableCS() const { sDelay(CSN_TIME); GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET); }		///< Selects transceiver
	inline void disableCS() const { GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET); }						///< Releases transceiver
	inline void enableCE() const { GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);  sDelay(CE_HIGH_TIME); }
	inline void disableCE() const { GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET); }

	tStatus sendByte(unsigned char data);

private:
	inline DisplayNrfSharedSpi & sharedSpi() { return _sharedSpi; }

protected:
	bool _bInitialized;
	DisplayNrfSharedSpi & _sharedSpi;			///< Reference to the shared SPI (Board interface)
	bool _bNrfFeaturesEnabled;					///< True when nrf features are enabled (See: SPI command 'ACTIVATE')
};

