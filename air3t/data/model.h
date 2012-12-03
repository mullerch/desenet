#pragma once
#include <list>
#include "GameField"
#include "Player"
#include "Statistics"

/**
 * @defgroup air3t Air3T
 * @brief The Air3T Application.
 *
 * A simple Tic Tac Toe game that can be played wireless between all modules in RF range.
 *
 * @section air3t_cd Class Diagram
 * The following illustration shows the class diagram of the Air3T solution:
 * @image html Air3T.png
 *
 * The solution is based around the MVC pattern with a Air3T::Model section, a Air3T::UI section for the views and
 * the Air3T::Logic package contains the controller with the FSM that controls the game behavior.
 */

/**
 * @ingroup air3t Air3T
 * @defgroup data Data
 * @brief The data (model) part of the Air3T solution.
 *
 * The data model contains all information about the actually played Tic Tac Toe game.
 */

namespace Air3T
{
	class IView;

	/**
	 * @ingroup data
	 * @brief The model contains all game related data needed for the TicTacToe game over the air.
	 *
	 * The model offers a method to register a view implementing the IView interface in order to notify the view once
	 * the model data has been changed and the view can redraw the data changes.
	 */
	class Model
	{
		friend class GameField;
		friend class Statistics;

	public:
		/**
		 * @brief The different sections inside the Air3T model that will be used by the view's update methods.
		 */
		enum ModelSection
		{
			UnknownSection			= 0 ,	///< @brief The section where the data has been changes is not known.
			AvailablePlayerSection	= 1 ,	///< @brief The list of available players has changed.
			GameFieldSection		= 2 ,	///< @brief The game field has been changed.
			StatisticsSection		= 3		///< @brief The statistics have been changed.
		};

		/**
		 * @brief Constructs a new Tic Tac Toe model.
		 *
		 * The model is immediately initialized and directly ready to use.
		 */
		Model();

		/**
		 * @brief Destructor, does nothing particular.
		 */
		virtual ~Model();

		/**
		 * @brief Registers a view within the model.
		 *
		 * Views will be notified using the IView interface about changes in the model.
		 *
		 * @param view The view to add to the list of views registered for model updates.
		 */
		void registerView( IView * view );

		/**
		 * @brief Removes a view from the model's list of views to be notified about changes in the model.
		 *
		 * @param view The view to remove from the list of views registered for model updates.
		 */
		void unregisterView( IView * view );

		/**
		 * @brief Returns the read-only reference to the list of players available to play with.
		 *
		 * @return Reference to the list of players.
		 */
		const std::list<Player> & availablePlayers() const;

		/**
		 * @brief Adds a player to the model and notifies all views.
		 *
		 * @param player The player to add, see Player for details.
		 */
		void addAvailablePlayer( const Player & player );

		/**
		 * @brief Removes the player from the model and notifies all registered views.
		 *
		 * @param player Player to remove, see Player for details.
		 */
		void removeAvailablePlayer( const Player & player );

		/**
		 * @brief Clears the player list.
		 */
		void clearAvailablePlayers();

		/**
		 * @brief Returns a read-only reference to the model's game field.
		 *
		 * @return Read only game field.
		 */
		const GameField & gamefield() const;

		/**
		 * @brief Returns a mutable reference to the model's game field.
		 *
		 * @return Mutable game field.
		 */
		GameField & gamefield();

		/**
		 * @brief Returns a read-only reference to the model's statistics.
		 *
		 * @return Read only statistics.
		 */
		const Statistics & statistics() const;

		/**
		 * @brief Returns a mutable reference to the model's statistics.
		 *
		 * @return Mutable statistics.
		 */
		Statistics & statistics();

	private:
		// Calls the update method on all registered views.
		void _update( int section );

		// List of all registered views to update if the model changes.
		std::list<IView *> _views;

		// List of all available players.
		std::list<Player> _players;

		// The Tic Tac Toe gamefield.
		GameField _gamefield;

		// The statistics of the last games played.
		Statistics _statistics;
	};
}
