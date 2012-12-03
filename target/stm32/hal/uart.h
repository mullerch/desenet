#pragma once


#include <stm32f10x.h>
#include <assert.h>


class Uart
{
	friend class Factory;

public:
	Uart(const unsigned char uartNbr);
	virtual ~Uart();

	bool initialize(const unsigned long baudRate);

	bool enable();
	bool disable();

	void write(const char * str, unsigned int length = 0);
	void onIsr();

	/**
	 * Static accessor to the instances of UART. Used by the factory.
	 * You should not use this method directly. Use the factory to
	 * access an UART instead.
	 */
	static inline Uart & uart(const unsigned char uartNbr)
	{
		assert(uartNbr >= 0 && uartNbr < UART_COUNT);
		return *_sUart[uartNbr];
	}

	/**
	 * Checks if UART is present and initialized
	 */
	static inline bool present(const unsigned char uartNbr)
	{
		return (uartNbr >= 0 &&
				uartNbr < UART_COUNT &&
				_sInitialized[uartNbr]) ? true : false;
	}

protected:
	static const unsigned char UART_COUNT = 3;	///< Constant indicating how many USART the MCU has

	unsigned char _uartNbr;						///< Number of UART. Index starting at 0
	USART_TypeDef * _pUart;						///< Reference to the USART structure

	static Uart * _sUart[UART_COUNT];			///< Array to check if USB device was created already
	static bool _sInitialized[UART_COUNT];		///< Indicates if the UART has been initialized
};

