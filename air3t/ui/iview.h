#pragma once
#include <IJoystickObserver>

/**
 * @ingroup air3t
 * @defgroup ui UI
 * @brief The user interface package of the Air3T game.
 *
 * A view is an object that renders a certain view to the display using the MonochromeDisplayPainter utility class. Calling the
 * update method of such a view will re-render the view. A view will basically send events to the controller if anything changes
 * and the controller can replace a view with another on the screen to have a different functionality available at a certain
 * moment in time. A view is always rendered fullscreen and there is only one view active at the moment.
 */

namespace Air3T
{
	/**
	 * @ingroup ui
	 * @brief View interface used by the model to notify the view about changes in the data model.
	 */
	class IView : public IJoystickObserver
	{
	protected:
		// Constructor and destructor are protected, since it is an interface and not meant the be instantiated.
		explicit IView() {}
		virtual ~IView() {}

	public:
		/**
		 * @brief This method is called by the model to notify the view about changes in the model.
		 *
		 * To be more effective, the model adds the section parameter to the update method, this helps that views do not have
		 * to deal with changed data in the model that are not of interest for the view. The model defines the sections.
		 *
		 * @param section The section of the model where the change happened.
		 */
		virtual void update( int section ) = 0;
	};
}
