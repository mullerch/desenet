#pragma once
#include "View"
class MonochromeDisplayPainter;
#include "../data/GameField"

namespace Air3T
{
	class Model;

	/**
	 * @ingroup ui
	 * @brief Renders a Tic Tac Toe field on the display with a cursor to select a field.
	 */
	class GameView : public View
	{
	public:
		/**
		 * @brief Creates and initializes the view with the data of the given model.
		 *
		 * @param painter 	The painter used to render the model's game field.
		 * @param model 	The model to use for rendering.
		 */
		explicit GameView( MonochromeDisplayPainter & painter , const Model & model );

		/**
		 * @brief Destructor, does nothing.
		 */
		virtual ~GameView();

		/**
		 * @brief Updates the game view if the section is the game field section of the model.
		 *
		 * @param section The section of the model that has changes.
		 */
		void update( int section );

		/**
		 * @brief Changes the visibility of the cursor on the view.
		 *
		 * @param cursorEnabled True to show the cursor, false to hide it.
		 */
		void setCursorEnabled( bool cursorEnabled );

		/**
		 * @brief Returns the current cursor position.
		 *
		 * @return The current cursor position.
		 */
		GameField::Field cursorPosition() const;

		/**
		 * @brief Changes the cursor position.
		 *
		 * @param field The new cursor position.
		 */
		void setCursorPosition( const GameField::Field & field );

		/**
		 * @name IJoystickObserver interface implementation.
		 * @{
		 */

		/**
		 * @brief Called by the IJoystick interface implementation if the position of the joystick has been changed.
		 *
		 * The GameView object will use this information to move the insertion cursor on the rendered game field.
		 *
		 * @param position The actual position of the joystick.
		 */
		void onPositionChange( IJoystick::Position position );

		//@}

	private:
		// Reference to the painter used to render the view.
		MonochromeDisplayPainter & _painter;

		// Reference to the data model to use.
		const Model & _model;

		// The actual position of the cursor on the game field.
		GameField::Field _cursor;

		// True if the cursor is enabled, false otherwise.
		bool _cursorEnabled;
	};
}
