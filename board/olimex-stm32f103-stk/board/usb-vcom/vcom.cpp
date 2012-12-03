#include <string.h>
#include <assert.h>
#include <vcom/usb_vcom.h>
#include "vcom.h"


Vcom Vcom::_Instance;
bool Vcom::_bInitialized(false);


int vcomWrite(const char * const ptr, unsigned int len)
{
	Vcom & instance = Vcom::instance();

	if (!Vcom::isInitialized())
	{
		instance.initialize();
	}

	instance.write(ptr, len);

	return len;
}

Vcom::Vcom()
{

#if (USE_WRITE_BUFFER != 0)
	_currentState = STATE_INITIAL;
#endif // (USE_WRITE_BUFFER != 0)

}

Vcom::~Vcom()
{

}

Vcom & Vcom::instance()
{
	return _Instance;
}

bool Vcom::isInitialized()
{
	return _bInitialized;
}

void Vcom::initialize()
{
	if (!isInitialized())
	{
		_bInitialized = true;

#if (USE_WRITE_BUFFER != 0)
		startBehavior();
#endif // (USE_WRITE_BUFFER != 0)
	}
}

void Vcom::write(const char * const pData, unsigned int len)
{
	assert(isInitialized());		// Error: Call initialize() first

	if (len)
	{
#if (USE_WRITE_BUFFER != 0)
		// Get memory
		char * pCopiedData = new char[len + 1];

		// Make a copy
		memcpy(pCopiedData, pData, len);
		pCopiedData[len] = '\0';

		// Enqueue the copy
		_queue.enqueue(pCopiedData);
#else
		Virtual_Com_Port_Transmit((u8*)pData, len);
#endif // (USE_WRITE_BUFFER != 0)
	}
}

void Vcom::write(const char * const pData)
{
	const size_t len = strlen(pData);

	write(pData, len);
}

#if (USE_WRITE_BUFFER != 0)
EventStatus Vcom::processEvent()
{
	eEventStatus eventStatus = EventStatus::Unknown;

	switch (_currentState)
	{
	case STATE_INITIAL:
		{
			if ( getCurrentEvent()->getEventType() == IXFEvent::Initial )
			{
				GEN(XFNullTransition());

				_currentState = STATE_POLL;

				eventStatus = EventStatus::Consumed;
			}
		}
		break;
	case STATE_POLL:
		{
			if (getCurrentEvent()->getEventType() == IXFEvent::NullTransition ||
				(getCurrentEvent()->getEventType() == IXFEvent::Timeout &&
				 getCurrentTimeout()->getId() == Timeout_POLL_id))
			{
				{
					while (!_queue.isEmpty())
					{
						const char * pData = _queue.dequeue();

						Virtual_Com_Port_Transmit((u8*)pData, strlen(pData));

						delete [] pData;
					}
				}

				getThread()->scheduleTimeout(Timeout_POLL_id, 100, this);

				_currentState = STATE_POLL;

				eventStatus = EventStatus::Consumed;
			}
		}
		break;
	default:
		break;
	}

	return eventStatus;
}
#endif // (USE_WRITE_BUFFER != 0)
