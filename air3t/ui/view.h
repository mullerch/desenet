#pragma once
#include "IView"
#include <cstring>

namespace Air3T
{
	class IViewObserver;

	/**
	 * @ingroup ui
	 * @brief Base class of all view implementations. Handles the pointers to the input event observer.
	 */
	class View : public IView
	{
	protected:
		// Constructor is protected, only derived classes can be instanciated.
		explicit View();

		// Returns the pointer to the observer to derived classes.
		inline IViewObserver * observer() { return _observer; }

	public:
		/**
		 * @brief Destructor, does nothing.
		 */
		virtual ~View();

		/**
		 * @brief Sets the observer to the observer pointed by observer.
		 *
		 * @param observer Pointer to the observer implementing the IViewObserver interface.
		 */
		bool setObserver( IViewObserver * observer );

	private:
		// The observer.
		IViewObserver * _observer;
	};
}
