#include "GameField"
#include "Model"
#include <assert.h>
#include <algorithm>

namespace Air3T
{
	GameField::GameField( Model & model ) : _model( model )
	{
		for ( int i = 0 ; i < 9 ; ++i )
			_data[i] = None;
	}

	void GameField::clear()
	{
		for ( int i = 0 ; i < 9 ; ++i )
			_data[i] = None;

		_model._update( Model::GameFieldSection );
	}

	GameField::Symbol GameField::winner() const
	{
		for ( int sequenceIncrement = 1 ; sequenceIncrement <= 4 ; ++sequenceIncrement )
		{
			int startField = sequenceIncrement == 2 ? 2 : 0;
			int posibilityIncrement = 4 - sequenceIncrement;
			for ( int j = 0 ; j < ( sequenceIncrement % 2 ? 3 : 1 ) ; ++j )
			{
				int sum = 0;
				for ( int i = 0 ; i < 3 ; ++i ) sum += _data[startField + j * posibilityIncrement + i * sequenceIncrement];
				if ( sum == 3 ) return Circle;
				if ( sum == -3 ) return Cross;
			}
		}

		// Is there a draft?
		for ( int i = 0 ; i < 9 ; i++ )
			if ( _data[i] == None ) return None;
		return Tie;
	}

	GameField::Symbol GameField::symbol( const Field & field ) const
	{
		assert( field.x < 3 && field.y < 3 );
		return static_cast<Symbol>( _data[std::min( field.x + 3 * field.y , 9U )] );
	}

	void GameField::setSymbol( const Field & field , Symbol symbol )
	{
		assert( field.x < 3 && field.y < 3 );
		_data[std::min( field.x + 3 * field.y , 9U )] = symbol;

		_model._update( Model::GameFieldSection );
	}

	int8_t * GameField::rawData()
	{
		return _data;
	}
}
