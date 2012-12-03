#pragma once
#include <stdint.h>

namespace Air3T
{
	class Model;

	/**
	 * @ingroup data
	 * @brief Represents a TicTacToe game field.
	 *
	 * A tic tac toe gamefield has 9 fields which can have basically 3 states:
	 * - No player's symbol is placed : GameField::None.
	 * - Player one (circle) symbol is placed: GameField::Circle.
	 * - Player two (cross) symbol is placed: GameField::Cross.
	 *
	 * The states can be read or set using the accessor/mutator methods symbol() and setSymbol(). Note that to address a certain field
	 * you need to pass a Field object that contains the coordinates of the field in the 3x3 2 dimensional space.
	 *
	 * The GameField is part of the data model and if a value of the gamefield changes the observer of the model will be notified automatically.
	 */
	class GameField
	{
		friend class Model;

	public:
		/**
		 * @brief The value each field of the GameField can have plus a special value to indicate that no player has won and the field is full
		 * (a tie).
		 */
		enum Symbol
		{
			None	=  0 ,		///< @brief No symbol was placed onto the field.
			Circle	=  1 ,		///< @brief A circle was placed onto the field.
			Cross	= -1 ,		///< @brief A cross was placed onto the field.
			Tie		= 2			///< @brief Not a symbol, but winner() returns this if there was a tie.
		};

		/**
		 * @brief Identifies a Field inside the GameField.
		 *
		 * Basically a field object is nothing other than two dimensional coordinates to address a field inside the GameField.
		 */
		class Field
		{
		public:
			/**
			 * @brief Creates a Field coordinates object with the given x and y components.
			 *
			 * @param x	X coordinate. Defaults to 0.
			 * @param y	Y coordinate. Defaults to 0.
			 */
			inline Field( unsigned int x = 0 , unsigned int y = 0 ) : x( x ) , y( y ) {};

			/**
			 * @brief X coordinate.
			 */
			unsigned int x;

			/**
			 * @brief Y coordinate.
			 */
			unsigned int y;
		};

		/**
		 * @brief Clears the GameField by setting all fields to None and notifies a model observer about the change.
		 */
		void clear();

		/**
		 * @brief Returns the field type Circle or Cross that has won the game, None if nobody already won and Tie if all fields
		 * 		  are full and no player has three tiles in a row.
		 *
		 * @return The winner tile, None in case nobody already won or Tie for a tie.
		 */
		Symbol winner() const;

		/**
		 * @brief Returns the symbol of the field identified by the given field object.
		 *
		 * @param field The field coordinates.
		 * @return 		The actual symbol on the field.
		 */
		Symbol symbol( const Field & field ) const;

		/**
		 * @brief Changes the symbol on the given field to the given symbol and notifies the model observer.
		 *
		 * @param field 	The field coordinates.
		 * @param symbol 	The symbol to place on the field identified by field.
		 */
		void setSymbol( const Field & field , Symbol symbol );

		/**
		 * @brief Returns the raw data of the gamefield, an array of signed 8-bit integers, where 0 means None,
		 * 		  1 means Circle and -1 means Cross.
		 *
		 * Note that the size clearly is 9 and at index 0 is the first element at top left.
		 *
		 * @return Pointer to the raw data of the game field.
		 */
		int8_t * rawData();

	private:
		// Only the Model can create a game field.
		explicit GameField( Model & model );

		// The game field data.
		int8_t _data[9];

		// The model that owns the game field.
		Model & _model;
	};
}
