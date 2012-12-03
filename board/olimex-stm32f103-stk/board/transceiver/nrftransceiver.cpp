#include <target.h>
#include <critical.h>
#include <IPhyObserver>
#include <phy/Frame>
#include <trace/trace.h>
#include <inc/factory.h>
#if (NRFTRANSCEIVER_TRACE_VERBOSE != 0)
	#include <stdarg.h>
	#include <stdio.h>
#endif
#include "nrftransceiver.h"
#include <xf/xfstaticevent.h>


const unsigned char RX_ADDRESS_P0[4]  = {5,6,7,8};
const unsigned char RX_ADDRESS_P1[4]  = {0,1,2,3};
const unsigned char TX_ADDRESS[4]     = {5,6,7,8};

NrfTransceiver::tAddress gFilterAddress01(RX_ADDRESS_P0);
NrfTransceiver::tAddress gFilterAddress02(RX_ADDRESS_P1);
NrfTransceiver::tAddress gDestinationAddress(TX_ADDRESS);


NrfTransceiver::NrfTransceiver()
 : NrfSpi(Factory::instance().sharedSpi()),
   _bInitialized(false),
   _channel(2),			// Reset value given by register reset value (RF_CH register)
   _pObserver(NULL),
   _bEnabled(false),
   _autoAck(true)		// Per default enabled by the NrfTransceiver hardware
{

}

NrfTransceiver::~NrfTransceiver()
{

}

bool NrfTransceiver::initialize()
{
	if (!_bInitialized)
	{
		unsigned char status;
		GPIO_InitTypeDef gpioInitStructure;

		_bInitialized = true;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

		// Chip enable (NRF_CE)
		gpioInitStructure.GPIO_Pin = GPIO_Pin_8;
		gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		gpioInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &gpioInitStructure);

		// Initialize SPI used for NrfTransceiver
		NrfSpi::initialize();

		//
		// Now the nRF transceiver can be accessed correctly
		//

		NrfSpi::disableCS();		// Release transceiver
		NrfSpi::disableCE();		// Discard transmission

		// Reset FIFOs
		flushTxFifo();
		flushRxFifo();

		// Reset NRF IRQ status bits
		status = NrfSpi::writeRegister(NrfSpi::STATUS, (NrfSpi::STATUS_REG_RX_DR | NrfSpi::STATUS_REG_TX_DS | NrfSpi::STATUS_REG_MAX_RT));

		// Set address width used
		setAddressWidth(tAddress::addressSize());
		assert((size_t)(NrfSpi::readRegister(NrfSpi::SETUP_AW) + 2) == tAddress::addressSize());

		// Write RX_ADDR_P0 register -> Set receive address data Pipe0 -> address in RX_ADDRESS_P0 array
//		setReceptionFilterAddress(gFilterAddress01);

		// Write RX_ADDR_P1 register -> Set receive address data Pipe1 -> address in RX_ADDRESS_P1 array
//		NrfSpi::writeRegister(NrfSpi::RX_ADDR_P1, gFilterAddress02);

		// Write TX_ADDR register -> Transmit address. Used for a PTX device only. Address in TX_ADDRESS array
//		setDestinationAddress(gDestinationAddress);

		activateFeatures(true);
		enableDPL(true, 0x03);		// Enable dynamic payload size
		assert((NrfSpi::readRegister(NrfSpi::FEATURE) & NrfSpi::FEATURE_REG_EN_DPL) == FEATURE_REG_EN_DPL);

		// Basic configuration for CONFIG register: Enable CRC
		NrfSpi::writeRegister(NrfSpi::CONFIG, NrfSpi::CONFIG_REG_EN_CRC);

		// Initialize and enable IRQ for the NRF_IRQ line (GPIO RC9)
		_initNrfTransceiverIrq();

		// Set (again) hardware default values of the Nrf chip
		setRadioChannel(2);
		setAutoAck(true);

		// Enable transceiver (and enter RX mode)
		setMode(Active);
	}

	return true;
}

void NrfTransceiver::_initNrfTransceiverIrq() const
{
	GPIO_InitTypeDef gpioInitStructure;
	EXTI_InitTypeDef extiInitStructure;
	NVIC_InitTypeDef nvicInitStructure;

	// Enable needed clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO  | RCC_APB2Periph_GPIOC, ENABLE);

    // Configure nRF interrupt pin as input floating
	gpioInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &gpioInitStructure);

	// Define GPIO to
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);

	// Configure external interrupt for PC9
	EXTI_ClearITPendingBit(EXTI_Line9);
	extiInitStructure.EXTI_Line = EXTI_Line9;
	extiInitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	extiInitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&extiInitStructure);

	// Enable EXTI interrupt for PC9
	nvicInitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	nvicInitStructure.NVIC_IRQChannelSubPriority = 0x00;
	nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInitStructure);
}

/**
 * Called out of the interrupt service routine. Is called when
 * nRF24L01 chip is lowering the NRF_IRQ line.
 * The code insight this method is executed in an ISR!
 */
void NrfTransceiver::onIrq()
{
	static XFStaticEvent irqEvent( 0 , this );

	enterISR();
	pushEvent( &irqEvent );
	exitISR();
}

EventStatus NrfTransceiver::processEvent()
{
	unsigned char status;

	if (!NrfSpi::isInitialized())
	{
		return EventStatus::NotConsumed;
	}

	status = NrfSpi::status();

	if ((status & NrfSpi::STATUS_REG_TX_FULL) == NrfSpi::STATUS_REG_TX_FULL)
	{
		// Flush TX fifo
		flushTxFifo();
		traceOut("irq:FLUSH_TX");
	}

	if ((status & NrfSpi::STATUS_REG_RX_DR) == NrfSpi::STATUS_REG_RX_DR)
	{
		unsigned char pipe = NrfSpi::getRXFramePipeNumber();
		size_t size = NrfSpi::getRXFramePaylpoadSize(pipe);

		if (size)
		{
			Frame frame;

			frame.setPayloadSize(size);

			// Read payload data
			NrfSpi::readFrame(frame);

			traceOut("Nrf: RX Frame (size %d): %s", frame.payloadSize(), frame.toString());

			if (_pObserver)
			{
				observer().onReceive(frame);
			}
			else
			{
				traceOut("Nrf Irq: Observer not set");
			}
		}
		else
		{
			traceOut("Nrf Irq: Rxed Frame with zero length!");

			// Test: Flushing RX FIFO
			traceOut("Nrf Irq: Flushing RX FIFO!");
			flushRxFifo();
		}

		// Clear RX_DR bit in status register
		status = NrfSpi::writeRegister(NrfSpi::STATUS, (status | NrfSpi::STATUS_REG_RX_DR));
	}

	if ((status & NrfSpi::STATUS_REG_TX_DS) == NrfSpi::STATUS_REG_TX_DS)
	{
		traceOut("irq::TX_DS");

		// Clear TX_DS bit in status register
		status = NrfSpi::writeRegister(NrfSpi::STATUS, (status | NrfSpi::STATUS_REG_TX_DS));

		observer().onSendStatus(IPhyObserver::Delivered);

		// Change from TX to RX mode
		enterRXMode();
	}

	if ((status & NrfSpi::STATUS_REG_MAX_RT) == NrfSpi::STATUS_REG_MAX_RT)
	{
		traceOut("irq:MAX_RT");

		// Discard frame by flushing TX fifo
		// See nRF24L01 datasheet on page 43, last paragraph
		flushTxFifo();

		// Clear max RT
		status = NrfSpi::writeRegister(NrfSpi::STATUS, (status | NrfSpi::STATUS_REG_MAX_RT));

		observer().onSendStatus(IPhyObserver::NotDelivered);

		// Change from TX to RX mode
		enterRXMode();
	}

	return EventStatus::Consumed;
}

void NrfTransceiver::setDestinationAddress(tAddress address)
{
	NrfSpi::writeRegister(NrfSpi::TX_ADDR, address);
}

void NrfTransceiver::setAddressWidth(unsigned char widthInByte)
{
	assert(widthInByte == 3 || widthInByte == 4 || widthInByte == 5);

	NrfSpi::writeRegister(NrfSpi::SETUP_AW, widthInByte - 2);
}

bool NrfTransceiver::activateFeatures(bool enable)
{
	return NrfSpi::activateFeatures(enable);
}

/**
 * Enable/disable dynamic payload length (DPL).
 *
 * \enable Set to true to enable DPL
 * \pipeMask Bitmask to define for which RX pipe DPL should be enabled. The LSB corresponds to pipe0.
 *
 * Attention:
 *  - You need to enable first call activateFeatues() in order to use
 *    the DPL feature.
 *  - You also need to enable <em>Enhanced ShockBurst</em>.
 */
void NrfTransceiver::enableDPL(bool enable, unsigned char pipeMask /* = 0x00 */)
{
	NrfSpi::tRegister reg;
	assert(_bNrfFeaturesEnabled);	// Call activateFeatues first

	reg = NrfSpi::readRegister(FEATURE);

	if (enable)
	{
		assert(pipeMask);		// Set at least to 0x01 (DPL on pipe0)

		reg |= FEATURE_REG_EN_DPL;
	}
	else
	{
		reg &= ~FEATURE_REG_EN_DPL;
	}

	NrfSpi::writeRegister(FEATURE, reg);

	if (enable)
	{
		// Enable DPL on corresponding pipes
		NrfSpi::writeRegister(NrfSpi::DYNPD, pipeMask);
	}
}

void NrfTransceiver::traceOut(const char * const format, ...)
{
#if (NRFTRANSCEIVER_TRACE_VERBOSE != 0)
	static char str[128];

	va_list args;

	va_start(args, format);
	vsiprintf(str, format, args);
	va_end(args);

	assert(strlen(str) < 128);		// Uups! str is too small

	Trace::out(str);
#endif
}

IPhyTransceiver::Mode NrfTransceiver::mode() const
{
	return (_bEnabled) ? Active : Inactive;
}

bool NrfTransceiver::setMode(Mode mode)
{
	assert(NrfSpi::isInitialized());	// Call initialize() first!

	return enableTransceiver((mode == Active) ? true : false);
}

bool NrfTransceiver::enableTransceiver(bool bEnable)
{
	if (_bEnabled != bEnable)
	{
		unsigned char status, config;

		_bEnabled = bEnable;

		config = NrfSpi::readRegister(NrfSpi::CONFIG);

		if (bEnable)
		{
			// power-up, RX
			config |= NrfSpi::CONFIG_REG_PWR_UP |
					  NrfSpi::CONFIG_REG_PRIM_RX;
			status = NrfSpi::writeRegister(NrfSpi::CONFIG, config);

			// Enable transceiver
			NrfSpi::enableCE();
		}
		else
		{
			// Power down the chip
			config &= ~NrfSpi::CONFIG_REG_PWR_UP;
			status = NrfSpi::writeRegister(NrfSpi::CONFIG, config);

			// Disable transceiver
			NrfSpi::disableCE();
		}

		// read
		//config = NrfSpi::readRegister(NrfSpi::CONFIG);
	}
	return true;
}

void NrfTransceiver::enterTXMode()
{
	unsigned char config, status;

	// Enter RX mode
	config = NrfSpi::readRegister(NrfSpi::CONFIG);
	config &= ~NrfSpi::CONFIG_REG_PRIM_RX;		// Clear PRIM_RX_BIT to enter TX mode
	// Write new value to config register
    status = NrfSpi::writeRegister(NrfSpi::CONFIG, config);
}

void NrfTransceiver::enterRXMode()
{
	unsigned char config, status;

	// Enter RX mode
	config = NrfSpi::readRegister(NrfSpi::CONFIG);
	config |= NrfSpi::CONFIG_REG_PRIM_RX;		// Set PRIM_RX_BIT to enter RX mode
	// Write new value to config register
    status = NrfSpi::writeRegister(NrfSpi::CONFIG, config);
}

IPhyTransceiver::RadioChannel NrfTransceiver::radioChannel() const
{
	return _channel;
}

bool NrfTransceiver::setRadioChannel(RadioChannel channel)
{
	assert(NrfSpi::isInitialized());	// Call initialize() first!

	// Only values from 0 to 125 are allowed
	if (channel < 126)
	{
		if (_channel != channel)
		{
			_channel = channel;

			traceOut("Nrf: Changed radio channel: %d", _channel);

			NrfSpi::writeRegister(NrfSpi::RF_CH, channel);
		}
		return true;
	}
	return false;
}

IPhyTransceiver::FilterAddress NrfTransceiver::receptionFilterAddress() const
{
	return _filterAddress;
}

bool NrfTransceiver::setReceptionFilterAddress(const FilterAddress & address)
{
	if (_filterAddress != address)
	{
		_filterAddress = address;
		NrfSpi::writeRegister(NrfSpi::RX_ADDR_P0, address);
	}
	return true;
}

bool NrfTransceiver::autoAck() const
{
	return _autoAck;
}

bool NrfTransceiver::setAutoAck(bool autoAck)
{
	if (_autoAck != autoAck)
	{
		tRegister regValue = 0x3F;	// Keeps the other data pipes autoACK enabled

		_autoAck = autoAck;			// Store value in object

		if (autoAck)
		{
			traceOut("Nrf: Enabled AutoACK");
			regValue |= 0x01;		// Set bit to enable AutoACK on data pipe 0
		}
		else
		{
			traceOut("Nrf: Disabled AutoACK");
			regValue &= ~0x01;		// Clear bit to disable AutoACK on data pipe 0
		}

		NrfSpi::writeRegister(NrfSpi::EN_AA, regValue);
		return true;
	}
	return false;
}

bool NrfTransceiver::setObserver(IPhyObserver * observer)
{
	if (!_pObserver)
	{
		_pObserver = observer;
		assert(_pObserver);
		return true;
	}
	return false;
}

bool NrfTransceiver::send(const Frame & frame)
{
	unsigned char status;

	assert(NrfSpi::isInitialized());	// Call initialize() first!

	if (!frame.payloadSize())
	{
		assert(false);	// Really wont to send frame with zero length?!
		return false;
	}

	// Set destination address
	setDestinationAddress(frame.address());

	traceOut("Nrf: TX Frame (size %d): %s", frame.payloadSize(), frame.toString());

	NrfSpi::disableCE();

	// Send payload - send any data
	status = NrfSpi::sendFrame(frame);

    enterTXMode();

	// Enable CE -> Starts the transmission
    NrfSpi::enableCE();
//	NrfSpi::disableCE();
// Do not pulse

	return true;
}
