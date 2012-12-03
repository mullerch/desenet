#include "Model"
#include <assert.h>
#include "../ui/IView"

namespace Air3T
{
	Model::Model() : _gamefield( *this ) , _statistics( *this )
	{}

	Model::~Model()
	{}

	void Model::registerView( Air3T::IView * view )
	{
		assert( view );
		if ( view )
			_views.push_back( view );
	}

	void Model::unregisterView( Air3T::IView * view )
	{
		assert( view );
		_views.remove( view );
	}

	const std::list<Player> & Model::availablePlayers() const
	{
		return _players;
	}

	void Model::addAvailablePlayer( const Player & player )
	{
		_players.push_back( player );

		_update( AvailablePlayerSection );
	}

	void Model::removeAvailablePlayer( const Player & player )
	{
		for ( std::list<Player>::iterator i = _players.begin() ;
			  i != _players.end() ; ++i )
		if ( i->id() == player.id() )
		{
			i = _players.erase( i );
			break;
		}

		_update( AvailablePlayerSection );
	}

	void Model::clearAvailablePlayers()
	{
		_players.clear();

		_update( AvailablePlayerSection );
	}

	const GameField & Model::gamefield() const
	{
		return _gamefield;
	}

	GameField & Model::gamefield()
	{
		return _gamefield;
	}

	const Statistics & Model::statistics() const
	{
		return _statistics;
	}

	Statistics & Model::statistics()
	{
		return _statistics;
	}

	void Model::_update( int section )
	{
		for ( std::list<IView *>::const_iterator i = _views.begin() ; i != _views.end() ; ++i )
			(*i)->update( section );
	}
}
