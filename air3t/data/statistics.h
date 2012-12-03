#pragma once

namespace Air3T
{
	class Model;

	/**
	 * @ingroup data
	 * @brief Holds the statistics about how many games were won or lost by the local player.
	 */
	class Statistics
	{
		friend class Model;

	public:
		/**
		 * @brief Returns the number of games the local player has played in total.
		 *
		 * @return The number of games the local player has played.
		 */
		int gamesPlayed() const;

		/**
		 * @brief Returns the number of games the local player has won.
		 *
		 * @return The number of games the local player has won.
		 */
		int gamesWon() const;

		/**
		 * @brief Increments the number of games the local player has won.
		 */
		void incrementGamesWon();

		/**
		 * @brief Returns the number of games the local player has played tie.
		 *
		 * @return The number of games the local player has played tie.
		 */
		int gamesTie() const;

		/**
		 * @brief Increments the number of games the local player has played tie.
		 */
		void incrementGamesTie();

		/**
		 * @brief Returns the number of games the local player has lost.
		 *
		 * @return The number of games the local player has lost.
		 */
		int gamesLost() const;

		/**
		 * @brief Increments the number of games the local player has lost.
		 */
		void incrementGamesLost();

		/**
		 * @brief Resets the game statistics.
		 */
		void reset();

	private:
		// Only the Model can instanciate this class.
		explicit Statistics( Model & model );
		virtual ~Statistics();

		// The statistic values.
		int _won , _tie , _lost;

		// Pointer to the model to inform about the changes.
		Model & _model;
	};
}
