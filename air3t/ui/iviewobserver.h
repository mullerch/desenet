#pragma once
#include "../data/GameField"

namespace Air3T
{
	/**
	 * @ingroup ui
	 * @brief Interface to retrieve events from the any view.
	 */
	class IViewObserver
	{
	protected:
		// It is an interface, only subclasses can be instantiated.
		explicit IViewObserver() {}
		virtual ~IViewObserver() {}

	public:
		/**
		 * @brief A menu point on a MenuView was selected.
		 *
		 * This means that the cursor was one one menu entry and the user pressed the joystick down.
		 *
		 * @param menuPointIndex The index of the selected menu point.
		 */
		virtual void onMenuPointSelection( int menuPointIndex ) {}

		/**
		 * @brief The user has selected a field on the gamefield.
		 *
		 * This means he choose a field with the cursor and pressed the joystick down to select that field.
		 *
		 * @param field The selected field.
		 */
		virtual void onGameFieldSelection( const GameField::Field & field ) {}

		/**
		 * @brief The user has pressed or moved the joystick into any direction on the text view.
		 */
		virtual void onTextViewProceed() {}
	};
}
