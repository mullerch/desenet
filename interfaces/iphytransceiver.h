#pragma once
#include <utils/Address>
#include <stdint.h>
class IPhyObserver;
class Frame;

/**
 * @ingroup interfaces
 * @brief Abstracts the PHY+ hardware and driver needed to send frames using a shared medium or over the air.
 *
 * Note that the features the interface offers go beyond the standard PHY features, since the used NRF chip offers hardware
 * frame address decoding and automatic frame acknowledgment and retransmit and since these are realized in hardware,
 * it will be way more efficient and faster.
 */
class IPhyTransceiver
{
protected:
	// It is just an interface, so constructor and destructor should nor be public.
	IPhyTransceiver() {}
	virtual ~IPhyTransceiver() {}

public:
	/**
	 * @brief Radio channel data type used to change the channel on the PHY for those models that support
	 * 		  that feature.
	 */
	typedef uint8_t RadioChannel;

	/**
	 * @brief The PHY disposes an address filter.
	 *
	 * Although the NRF chip supports 3 to 5 bytes addresses, the address size is fixed to 4 bytes for this
	 * interface for compatibility reasons. This data type will be used to setup this address.
	 */
	typedef Address<4> FilterAddress;

	/**
	 * @brief Initializes all PHY subsystems and the object itself needed for proper operation.
	 *
	 * Note that after initialization, the PHY will be in Active mode, but the radio channel, the auto
	 * ack feature mode and filter address are undefined and depends the underlying implementation. Remember
	 * this if you have problems across platforms. This method must be called before any other one is called.
	 *
	 * @return True if the PHY could be initialized and is ready to be used, false otherwise.
	 */
	virtual bool initialize() = 0;

	/**
	 * @brief Defines the operating modes a PHY transceiver.
	 *
	 * The PHY should be in the Active state after having been initialized and you need to put it on
	 * Inactive state manually.
	 */
	enum Mode
	{
		Undefined 	= 0 ,	///< @brief Undefined mode.
		Inactive	= 1 ,	///< @brief The transceiver is inactive (off).
		Active		= 2		///< @brief The transceiver is active (on).
	};

	/**
	 * @brief Returns the actual mode of the transceiver.
	 *
	 * @return The actual mode of the transceiver.
	 */
	virtual Mode mode() const = 0;

	/**
	 * @brief Changes the actual mode of the transceiver to the given one and returns true on success.
	 *
	 * @param mode 	The mode the transceiver should change to.
	 * @return 		True on success, false otherwise.
	 */
	virtual bool setMode( Mode mode ) = 0;

	/**
	 * @brief Returns the radio channel the transceiver is currently using.
	 *
	 * @return Actual radio channel.
	 */
	virtual RadioChannel radioChannel() const = 0;

	/**
	 * @brief Changes the radio channel to the given one.
	 *
	 * @param channel 	The radio channel to use.
	 * @return 			True if the channel could be changed or false otherwise.
	 */
	virtual bool setRadioChannel( RadioChannel channel ) = 0;

	/**
	 * @brief Returns the actual reception address used by the hardware address decoder.
	 *
	 * @return The actual reception address.
	 */
	virtual FilterAddress receptionFilterAddress() const = 0;

	/**
	 * @brief Changes the address filter decoder of the hardware to the given address.
	 *
	 * @param address 	The address to use for hardware address filtering.
	 * @return 			True if the address filter changed successfully.
	 */
	virtual bool setReceptionFilterAddress( const FilterAddress & address ) = 0;

	/**
	 * @brief Returns true if the auto acknowledge feature of the underlying PHY hardware is used and false if not.
	 *
	 * @return True if the auto acknowledge feature is used, false if not.
	 */
	virtual bool autoAck() const = 0;

	/**
	 * @brief Enables or disables the PHY's automatic acknowledge feature.
	 *
	 * @param autoAck 	If true, the auto acknowledge feature is enabled, if false it will be disabled.
	 * @return 			True if the operation succeed, false otherwise.
	 */
	virtual bool setAutoAck( bool autoAck ) = 0;

	/**
	 * @brief Sets the observer that will receive events upon frame arrivals and status reports after frame transmissions.
	 *
	 * Note that if already an observer was set before, the old one will be discarded without any notification since
	 * only one observer is possible at the same time. Note that depending on the used platform, the context the
	 * methods of the observer can be another thread or even an interrupt service routine. So new() and malloc()
	 * should not be used inside these methods.
	 *
	 * @param observer 	Pointer to the observer object which should receive the PHY events.
	 * @return 			True if the observer was accepted, false if no observer was set (observer == NULL) or on error.
	 */
	virtual bool setObserver( IPhyObserver * observer ) = 0;

	/**
	 * @brief Sends the given frame to the medium using the PHY hardware. Note that the PHY supports only sending one frame
	 * 		  at once, the next frame can only be sent after having received the status from the previous one, otherwise the
	 * 		  send method will fail and returning false.
	 *
	 * @param frame 	Frame to send, this includes the frame payload and the destination address.
	 * @return 			True if the PHY has taken the frame for emission, false otherwise (transceiver busy).
	 */
	virtual bool send( const Frame & frame ) = 0;
};
