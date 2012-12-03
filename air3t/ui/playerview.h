#pragma once
#include "MenuView"
class MonochromeDisplayPainter;

namespace Air3T
{
	class Model;


	/**
	 * @ingroup ui
	 * @brief Displays all detected remote players that are available to play the Air3T game with.
	 *
	 * Displays the list of all available players to play the Ait3T Tic Tac Toe game with in a standard menu view
	 * using their nicknames (friendly name) to identify the players.
	 */
	class PlayerView : public MenuView
	{
	public:
		/**
		 * @brief Creates the view that uses the given display painter and the model.
		 *
		 * @param painter Painter to use to render the view.
		 * @param model Model to use to get the data for the view.
		 */
		explicit PlayerView( MonochromeDisplayPainter & painter , const Model & model );

		/**
		 * @brief Destructor, does nothing.
		 */
		virtual ~PlayerView();

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

	private:
		// Pointer to the model to use for data retrieval.
		const Model & _model;
	};
}
