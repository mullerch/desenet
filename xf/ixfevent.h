#ifndef IXFEVENT_H
#define IXFEVENT_H


class IXFReactive;


/**
 * @ingroup xf
 * Interface for an event. The abstract methods defined in this interface
 * need to be reimplemented in its derived classes. These methods are
 * used by other classes in the framework (ex. XFThread, XFReactive).
 *
 * Requirements:
 * - Needs to hold a property which allows to distinguish between different
 *   types of events.
 * - Use an enumeration to define the type for the type property.
 * - The type property needs to be set at initialization time and cannot be
 *   changed afterwards.
 */
class IXFEvent
{
public:
	/**
	 * Defines the types of events
	 */
	typedef enum
	{
		Unknown			= 0,		///< Unknown state (not initialized)
		Initial			= 1,		///< Initial pseudo-state
		NullTransition	= 2,		///< Event generated by the state machine to define a null transition
		Event			= 3,		///< Normal event
		Timeout			= 4,		///< Defines a timeout event.
		Terminate		= -1		///< Defines a terminate state. Will end the state machine.
	} eEventType;

public :
	/**
	 * Default class constructor
	 */
	IXFEvent(eEventType eventType, int id, IXFReactive * pBehavior) : _eventType(eventType), _id(id), _pBehavior(pBehavior) { _eventStatus.shouldDelete = true; }
	virtual ~IXFEvent() {}		///< Class destructor

	/** \brief Returns the type of the event.
	 *
	 * Can be used to distinguish between an event or a timeout.
	 */
	inline eEventType getEventType() const { return _eventType; }

	/** \brief Returns pointer to behavioral class.
	 *
	 * Returns a pointer to the behavioral class processing the event (see #_pBehavior).
	 */
	inline IXFReactive * getBehavior() const { return _pBehavior; }

	/** \brief Sets pointer to behavioral class (see #_pBehavior).
	 * Sets the behavior in which the event should be executed.
	 */
	inline void setBehavior(IXFReactive * pBehavior) { _pBehavior = pBehavior; }

	inline int getId() const { return _id; }		///< Returns #_id identifying the event in the behaviors context.

	/**
	 * Marks the event as non-deletable. Used for static events.
	 */
	inline void setShouldDelete(bool bShouldDelete = true) { _eventStatus.shouldDelete = bShouldDelete; }

	/** \brief Returns true in case the event should be deleted
	 * Used by XFThread to check if the event should be delete after consume
	 */
	inline bool shouldDelete() const { return _eventStatus.shouldDelete; }

protected:
	typedef struct _tEventStatus
	{
		unsigned char shouldDelete : 1;	///< True in case the event should be deleted after consume.

		_tEventStatus()
		{
			shouldDelete = true;
		}
	} tEventStatus;

protected:
	const eEventType _eventType;		///< Holds the type of the event.
	int _id;							///< Event id
	tEventStatus _eventStatus;			///< Holds information about the current event.

	IXFReactive * _pBehavior;			///< Pointer to behavioral class processing the event.
};

#endif // IXFEVENT_H
