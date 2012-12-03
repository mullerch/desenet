#include "Statistics"
#include "Model"

namespace Air3T
{
	Statistics::Statistics( Model & model ) : _model( model )
	{
		reset();
	}

	Statistics::~Statistics()
	{}

	int Statistics::gamesPlayed() const
	{
		return _won + _tie + _lost;
	}

	int Statistics::gamesWon() const
	{
		return _won;
	}

	void Statistics::incrementGamesWon()
	{
		++_won;
		_model._update( Model::StatisticsSection );
	}

	int Statistics::gamesTie() const
	{
		return _tie;
	}

	void Statistics::incrementGamesTie()
	{
		++_tie;
		_model._update( Model::StatisticsSection );
	}

	int Statistics::gamesLost() const
	{
		return _lost;
	}

	void Statistics::incrementGamesLost()
	{
		++_lost;
		_model._update( Model::StatisticsSection );
	}

	void Statistics::reset()
	{
		_won = 0;
		_tie = 0;
		_lost = 0;

		_model._update( Model::StatisticsSection );
	}
}
