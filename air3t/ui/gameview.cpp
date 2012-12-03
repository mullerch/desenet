#include "GameView"
#include <cstring>
#include <algorithm>
#include "../data/Model"
#include <utils/MonochromeDisplayPainter>
#include "IViewObserver"

namespace Air3T
{
	GameView::GameView( MonochromeDisplayPainter & painter , const Model & model )
		: _painter( painter ) , _model( model ) , _cursorEnabled( true )
	{
		_painter.setAutoUpdate( false );
	}

	GameView::~GameView()
	{}

	void GameView::update( int section )
	{
		if ( section == Model::GameFieldSection || section == Model::UnknownSection )
		{
			// Clear display.
			_painter.clear();

			// Draw TTT grid.
			for ( unsigned int i = 1 ; i <= 2 ; ++i ) _painter.drawLine( IMDPoint( 18 + i * 16 , 0 ) ,
																		 IMDPoint( 18 + i * 16 , 47 ) ,
																		 IMonochromeDisplay::Black );
			for ( unsigned int i = 1 ; i <= 2 ; ++i ) _painter.drawLine( IMDPoint( 18 , i * 16 ) ,
																		 IMDPoint( 66 , i * 16 ) ,
																		 IMonochromeDisplay::Black );

			for ( unsigned int x = 0 ; x < 3 ; ++x )
				for ( unsigned int y = 0 ; y < 3 ; ++y )
				{
					IMDPixelColor color = IMonochromeDisplay::Undefined;
					if ( _cursorEnabled && x == _cursor.x && y == _cursor.y )
					{
						_painter.drawRectangle( IMDPoint( 18 + 2 + 16 * x  , 2 + 16 * y ) , IMDSize( 13 , 13 ) ,
												IMonochromeDisplay::Black , IMonochromeDisplay::Black );
						color = IMonochromeDisplay::White;
					}
					else color = IMonochromeDisplay::Black;

					switch( _model.gamefield().symbol( GameField::Field( x , y ) ) )
					{
						case GameField::Circle:
							_painter.drawEllipse( IMDPoint( 18 + 4 + 16 * x , 4 + 16 * y ) ,
												  IMDSize( 9 , 9 ) , color );
							break;

						case GameField::Cross:
							_painter.drawLine( IMDPoint( 18 + 4 + 16 * x , 4 + 16 * y ) ,
											   IMDPoint( 18 + 12 + 16 * x , 12 + 16 * y ) , color );
							_painter.drawLine( IMDPoint( 18 + 4 + 16 * x , 12 + 16 * y ) ,
											   IMDPoint( 18 + 12 + 16 * x , 4 + 16 * y ) , color );
							break;

						default:
							break;
					}
				}

			_painter.update();
		}
	}

	void GameView::setCursorEnabled( bool cursorEnabled )
	{
		if ( _cursorEnabled != cursorEnabled )
		{
			_cursorEnabled = cursorEnabled;
			update( Model::GameFieldSection );
		}
	}

	GameField::Field GameView::cursorPosition() const
	{
		return _cursor;
	}

	void GameView::setCursorPosition( const GameField::Field & field )
	{
		_cursor = field;
		update( Model::GameFieldSection );
	}

	void GameView::onPositionChange( IJoystick::Position position )
	{
		switch ( position )
		{
			case IJoystick::Up:
				if ( _cursor.y > 0 ) _cursor.y--;
				update( Model::GameFieldSection );
				break;

			case IJoystick::Down:
				if ( _cursor.y < 2 ) _cursor.y++;
				update( Model::GameFieldSection );
				break;

			case IJoystick::Left:
				if ( _cursor.x > 0 ) _cursor.x--;
				update( Model::GameFieldSection );
				break;

			case IJoystick::Right:
				if ( _cursor.x < 2 ) _cursor.x++;
				update( Model::GameFieldSection );
				break;

			case IJoystick::Pressed:
				if ( observer() && _model.gamefield().symbol( _cursor ) == GameField::None )
					observer()->onGameFieldSelection( _cursor );
				break;
			default:
				// Do nothing...
				break;
		}
	}
}
