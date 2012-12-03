#include "MenuView"
#include <utils/MonochromeDisplayPainter>
#include <cstring>
#include <algorithm>
#include "IViewObserver"

namespace Air3T
{

	MenuView::MenuView( MonochromeDisplayPainter & painter , const std::string & title )
		: _painter( painter ) , _title( title ) , _window( 0 ) , _cursor( 0 )
	{
		_painter.setAutoUpdate( false );
	}

	MenuView::~MenuView()
	{}

	void MenuView::repaint()
	{
		// Check that _cursor and _window are within the size.
		_window = std::max( 0 , std::min( _window , static_cast<int>( _menu.size() ) - 4 ) );
		_cursor = std::max( 0 , std::min( _cursor , static_cast<int>( _menu.size() ) - 1 ) );

		// Clear display.
		_painter.clear();

		// Draw title and box.
		_painter.drawRectangle( IMDPoint( 0 , 0 ) , IMDSize( 84 , 48 ) , IMonochromeDisplay::Black );
		_painter.drawRectangle( IMDPoint( 0 , 0 ) , IMDSize( 84 , 9 ) , IMonochromeDisplay::Black ,
								IMonochromeDisplay::Black );
		_painter.drawText( IMDPoint( 42 - _title.size() * 6 / 2 , 1 ) , _title ,
						   IMonochromeDisplay::White );

		int displayedSize = std::min( static_cast<unsigned int>( _menu.size() ) , 4U );

		for ( int i = 0 ; i < displayedSize ; ++i )
		{
			if ( _cursor == i + _window )
			{
				_painter.drawRectangle( IMDPoint( 42 - _menu[i + _window].size() * 6 / 2 - 2 ,
												  28 - displayedSize * 4 + i * 8 ) ,
										IMDSize( _menu[i + _window].size() * 6 + 3 , 8 ) ,
										IMonochromeDisplay::Black , IMonochromeDisplay::Black );

				_painter.drawText( IMDPoint( 42 - _menu[i + _window].size() * 6 / 2 ,
											 28 - displayedSize * 4 + i * 8 ) ,
											 _menu[i + _window] , IMonochromeDisplay::White );
			}
			else
			{
				_painter.drawText( IMDPoint( 42 - _menu[i + _window].size() * 6 / 2 ,
											 28 - displayedSize * 4 + i * 8 ) ,
											 _menu[i + _window] , IMonochromeDisplay::Black );
			}
		}

		_painter.update();
	}

	void MenuView::onPositionChange( IJoystick::Position position )
	{
		switch ( position )
		{
			case IJoystick::Up:
				if ( _cursor > 0 )
				{
					_cursor--;
					if ( _window > _cursor ) _window = _cursor;
					repaint();
				}
				break;

			case IJoystick::Down:
				if ( _cursor < static_cast<int>( _menu.size() ) - 1 )
				{
					_cursor++;
					if ( _cursor - _window > 3 ) _window = _cursor - 3;
					repaint();
				}
				break;

			case IJoystick::Pressed:
				if ( observer() )
					observer()->onMenuPointSelection( _cursor );

			default:
				// Do nothing...
				break;
		}
	}
}
