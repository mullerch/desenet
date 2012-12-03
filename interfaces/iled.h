#pragma once

/**
 * @ingroup interfaces
 * @brief Interface for a single LED. Abstracts all operations possible (turn off, turn on, toggle)
 * 		  with a simple interface.
 */
class ILed
{
public:
	/**
	 * @brief All possible and impossible states a LED can have.
	 */
	enum State
	{
		Invalid	=  0 ,	///< @brief Invalid LED state.
		Off 	= -1 ,	///< @brief The LED is off.
		On 		=  1	///< @brief The LED is on.
	};

protected:
	// It is just an interface, so constructor and destructor should be not public.
	explicit ILed() {}
	virtual ~ILed() {}

public:
	/**
	 * @brief Initializes all subsystems and the object itself needed for proper LED operation.
	 *
	 * @return True if the LED could be initialized and is ready to be used, false otherwise.
	 */
	virtual bool initialize() = 0;

	/**
	 * @brief Returns the actual state of the LED.
	 *
	 * @return State of the LED.
	 */
	virtual State state() const = 0;

	/**
	 * @brief Changes the actual state of the LED.
	 *
	 * @param state State to set for the LED.
	 */
	virtual void setState( State state ) = 0;

	/**
	 * @brief Convenient method that makes myLed = ILed::On statements possible.
	 *
	 * @param state State to set for the LED.
	 * @return Reference to the ILed object.
	 */
	inline const ILed & operator=( State state )
	{
		setState( state );
		return *this;
	}

	/**
	 * @brief Convenient method that makes if ( myLed = ILed::Off ) statements possible.
	 *
	 * @return Actual state of the LED.
	 */
	inline operator State() const
	{
		return state();
	}

	/**
	 * @brief Convenient method that toggles the LED if this functionality is not provided
	 * 		  by the ILed implementation.
	 */
	virtual void toggle()
	{
		setState( static_cast<State>( -state() ) );
	}
};
