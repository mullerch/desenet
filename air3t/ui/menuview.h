#pragma once
#include "View"
class MonochromeDisplayPainter;
#include <vector>
#include <string>

namespace Air3T
{
	/**
	 * @ingroup ui
	 * @brief Base class for all menu based views.
	 *
	 * The view already implements the whole scrolling and selection tasks and the menu can be customized by derived classes.
	 */
	class MenuView : public View
	{
	public:
		/**
		 * @brief Creates a new menu view with the given title that uses the given painter to render himself.
		 *
		 * @param painter The painter to use for view rendering.
		 * @param title The title of the view.
		 */
		explicit MenuView( MonochromeDisplayPainter & painter , const std::string & title );

		/**
		 * @brief Destructor, does nothing.
		 */
		virtual ~MenuView();

		/**
		 * @brief Triggers a repaint of the view.
		 */
		void repaint();

		/**
		 * @name IJoystickObserver interface implementation.
		 * @{
		 */

		/**
		 * @brief Called by the IJoystick implementation upon the actual joystick position has been changed.
		 *
		 * @param position	The actual joystick position.
		 */
		void onPositionChange( IJoystick::Position position );

		//@}

	protected:
		/**
		 * @brief Direct access to the menu for derived classes using a reference to the menu vector.
		 *
		 * @return The reference to the menu that should be displayed.
		 */
		inline std::vector<std::string> & menu() { return _menu; }

	private:
		// Reference to the painter used to render the menu view.
		MonochromeDisplayPainter & _painter;

		// The actual title of the menu view.
		std::string _title;

		// The vector containing all menu points as strings.
		std::vector<std::string> _menu;

		// The first menu point to be shown.
		int _window;

		// The actual position inside the menu view.
		int _cursor;
	};
}
