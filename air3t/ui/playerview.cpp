#include "PlayerView"
#include <air3t/data/Model>

namespace Air3T
{
	PlayerView::PlayerView( MonochromeDisplayPainter & painter , const Model & model )
		: MenuView( painter , "Opponent..." ) , _model( model )
	{}

	PlayerView::~PlayerView()
	{}

	void PlayerView::update( int section )
	{
		if ( section == Model::AvailablePlayerSection || section == Model::UnknownSection )
		{
			menu().clear();
			for ( std::list<Player>::const_iterator i = _model.availablePlayers().begin() ;
				  i != _model.availablePlayers().end() ; ++i )
				menu().push_back( i->nickname() );

			repaint();
		}
	}
}
