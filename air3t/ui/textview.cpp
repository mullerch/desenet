#include "TextView"
#include <utils/MonochromeDisplayPainter>
#include <cstring>
#include <algorithm>
#include "IViewObserver"
#include <air3t/data/Model>

namespace Air3T
{

	TextView::TextView( MonochromeDisplayPainter & painter , const std::string & text )
		: _painter( painter ) , _text( text )
	{
		_painter.setAutoUpdate( false );
		repaint();
	}

	TextView::~TextView()
	{}

	void TextView::repaint()
	{
		// Clear display.
		_painter.clear();

		// Draw box.
		_painter.drawRectangle( IMDPoint( 0 , 0 ) , IMDSize( 84 , 48 ) , IMonochromeDisplay::Black );

		// Draw text.
		_painter.drawText( IMDPoint( 42 - _text.size() * 6 / 2 , 20 ) ,
									_text , IMonochromeDisplay::Black );

		_painter.update();
	}

	void TextView::update( int section )
	{
		if ( section == Model::UnknownSection )
			repaint();
	}

	void TextView::onPositionChange( IJoystick::Position position )
	{
		if ( position != IJoystick::Center && observer() )
		{
			observer()->onTextViewProceed();
		}
	}
}
