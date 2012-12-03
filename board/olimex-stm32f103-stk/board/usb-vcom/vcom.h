#pragma once

/**
 * Set the define USE_WRITE_BUFFER to '1' if you want the Vcom class to buffer
 * the messages to output (write). This may be useful when tracing things
 * out of an ISR.
 */
#define USE_WRITE_BUFFER		1

#ifdef __cplusplus
#if (USE_WRITE_BUFFER != 0)
	#include <xf/xfreactive.h>
	#include <containers/IsQueue>
#endif // (USE_WRITE_BUFFER != 0)
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C"
{
#endif

	// Global functions used in c files
	int vcomWrite(const char * const ptr, unsigned int len);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

class Vcom
#if (USE_WRITE_BUFFER != 0)
 : public XFReactive
#endif // (USE_WRITE_BUFFER != 0)
{
public:
	Vcom();
	virtual ~Vcom();

	static Vcom & instance();
	static bool isInitialized();

	void initialize();

	void write(const char * const pData);
	void write(const char * const pData, unsigned int len);


#if (USE_WRITE_BUFFER != 0)
protected:
	virtual EventStatus processEvent();

protected:
	typedef IsQueue<const char * const> tQueue;

	typedef enum
	{
		Timeout_POLL_id = 1
	} eTimeoutId;

	typedef enum
	{
		STATE_UNKOWN = 0,
		STATE_INITIAL = 1,
		STATE_POLL = 2
	} eMainState;

protected:
	tQueue _queue;
	eMainState _currentState;		///< Attribute indicating currently active state

#endif // (USE_WRITE_BUFFER != 0)

protected:
	static Vcom _Instance;
	static bool _bInitialized;
};

#endif /* __cplusplus */
