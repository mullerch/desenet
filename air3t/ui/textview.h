#pragma once
#include "View"
#include <string>
class MonochromeDisplayPainter;

namespace Air3T
{
	/**
	 * @ingroup ui
	 * @brief View to display just a text on the display.
	 *
	 * Notifies the observer implementing the IViewObserver interface upon the joystick was moved into any direction
	 * by calling the onTextViewProceed() method.
	 */
	class TextView : public View
	{
	public:
		/**
		 * @brief Creates the text view and triggers directly an update that will paint the view into the display.
		 *
		 * @param painter Reference to the painter to use for view rendering.
		 * @param text The text to display.
		 */
		explicit TextView( MonochromeDisplayPainter & painter , const std::string & text );

		/**
		 * @brief Destructor, releases allocated ressources.
		 */
		virtual ~TextView();

		/**
		 * @brief Paints the view into the display.
		 */
		void repaint();

		/**
		 * @name IView interface implementation.
		 * @{
		 */

		/**
		 * @brief Called by the data model upon a change in the model has been detected.
		 *
		 * @param section The section of the model that has changed.
		 */
		void update( int section );

		//@}

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

	private:
		// Painter used to render the view.
		MonochromeDisplayPainter & _painter;

		// Text to display.
		std::string _text;
	};
}
