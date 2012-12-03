#include <xf/xf.h>
#include <xf/xfevent.h>
#include <xf/xfstaticevent.h>
#include <interfaces/IJoystickObserver>
#include <IAir3TFactory>
#include <trace/Trace>
using namespace std;

class CdPlayer : public XFReactive
{
/*** Define all events *******************************************************************************************************************/
private:
	class TurnOnEvent : 	public XFStaticEvent { public: static const int Id = 1; TurnOnEvent() : 	XFStaticEvent( Id ) {} };
	class TurnOffEvent : 	public XFStaticEvent { public: static const int Id = 2; TurnOffEvent() : 	XFStaticEvent( Id ) {} };
	class PlayPauseEvent : 	public XFStaticEvent { public: static const int Id = 3; PlayPauseEvent() : 	XFStaticEvent( Id ) {} };
	class TimeIncTimeout						 { public: static const int Id = 4; 										   };
	class WaitNullTransition					 { public: static const int Id = 5;											   };

/*** All methods accessible from external ************************************************************************************************/
public:
	CdPlayer() : _state( InitialState ) , _onSubstate( OnFinalState ) , _onPlaySubstate( OnPlayFinalState ) ,
				 _painter( &IAir3TFactory::factory().monochromeDisplay() )
	{ startBehavior(); }
	void turnOn() 		{ static TurnOnEvent event; 	pushEvent( &event ); }
	void turnOff() 		{ static TurnOffEvent event; 	pushEvent( &event ); }
	void playPause()	{ static PlayPauseEvent event;	pushEvent( &event ); }

/*** FSM implementation ******************************************************************************************************************/
private:
	enum { InitialState , Off , On } _state , _oldState;
	enum { Play , Pause , OnFinalState } _onSubstate , _oldOnSubstate;
	enum { Wait , IncrementTime , OnPlayFinalState } _onPlaySubstate , _oldOnPlaySubstate;

	EventStatus processEvent()
	{
		IXFEvent * e = getCurrentEvent();
		_oldState = _state; _oldOnSubstate = _onSubstate; _oldOnPlaySubstate = _onPlaySubstate;

		/*** State calculation ***********************************************************************************************************/
		switch( _state )
		{
			case InitialState:
				if ( e->getEventType() == XFEvent::Initial ) _state = Off;
				break;

			case Off:
				if ( e->getEventType() == XFEvent::Event && e->getId() == TurnOnEvent::Id )
				{
					_state = On;
					_onSubstate = Pause;
				}
				break;

			case On:
				if ( e->getEventType() == XFEvent::Event && e->getId() == TurnOffEvent::Id )
				{
					_state = Off;
					_onSubstate = OnFinalState;
					_onPlaySubstate = OnPlayFinalState;
				}
				else switch( _onSubstate )
				{
					case Play :
						if ( e->getEventType() == XFEvent::Event && e->getId() == PlayPauseEvent::Id )
						{
							_onSubstate = Pause;
							_onPlaySubstate = OnPlayFinalState;
						}
					    else switch( _onPlaySubstate )
						{
					    	case Wait :
					    		if ( e->getEventType() == XFEvent::Timeout && e->getId() == TimeIncTimeout::Id )
					    			_onPlaySubstate = IncrementTime;
					    		break;

					    	case IncrementTime :
					    		if ( e->getEventType() == XFEvent::NullTransition && e->getId() == WaitNullTransition::Id )
					    			_onPlaySubstate = Wait;
					    		break;

					    	default: break;
						}
						break;

					    case Pause:
					    	if ( e->getEventType() == XFEvent::Event && e->getId() == PlayPauseEvent::Id )
					    	{
					    		_onSubstate = Play;
					    		_onPlaySubstate = Wait;
					    	}
					    	break;

					    default: break;
				}
				break;
		}

		/*** Actions on exit *************************************************************************************************************/
		if ( _oldOnPlaySubstate != _onPlaySubstate ) switch( _oldOnPlaySubstate )
		{
			case Wait : Trace::out( "      [On:Play:Wait]->" );
				getThread()->unscheduleTimeout( TimeIncTimeout::Id , this );
				break;

			case IncrementTime : Trace::out( "      [On::Play:IncrementTime]->" );
				break;

			default: break;
		}

		if ( _oldOnSubstate != _onSubstate ) switch( _oldOnSubstate )
		{
			case Play : Trace::out( "    [On::Play]->" );
				break;

			case Pause : Trace::out( "    [On::Pause]->" );
				break;

			default: break;
		}

		if ( _oldState != _state ) switch( _oldState )
		{
			case On : Trace::out( "  [On]->" );
				IAir3TFactory::factory().led() = ILed::Off;
				_painter.clear();
				break;

			case Off : Trace::out( "  [Off]->" );
				break;

			default: break;
		}

		/*** Actions on entry ************************************************************************************************************/
		if ( _state != _oldState ) switch( _state )
		{
			case On : Trace::out( "->[On]" );
				_time = 0;
				IAir3TFactory::factory().led() = ILed::On;
				_painter.drawText( IMDPoint( 28 , 20 ) , "00:00" );
				break;

			case Off : Trace::out( "->[Off]" );
				break;

			default: break;
		}

		if ( _onSubstate != _oldOnSubstate ) switch( _onSubstate )
		{
			case Play : Trace::out( "  ->[On::Play]" );
				break;

			case Pause : Trace::out( "  ->[On::Pause]" );
				break;

			default: break;
		}

		if ( _onPlaySubstate != _oldOnPlaySubstate ) switch( _onPlaySubstate )
		{
			case Wait : Trace::out( "    ->[On:Play:Wait]" );
				getThread()->scheduleTimeout( TimeIncTimeout::Id , 1000 , this );
				break;

			case IncrementTime : Trace::out( "    ->[On::Play:IncrementTime]" );
				++_time;
				Trace::outToString( _timeString , 6 , "%02i:%02i" , _time / 60 , _time % 60 );
				_painter.clear();
				_painter.drawText( IMDPoint( 28 , 20 ) , _timeString );
				pushEvent( new XFNullTransition( WaitNullTransition::Id ) );
				break;

			default: break;
		}

		return EventStatus::Consumed;
	}

	/*** Normal object attributes ********************************************************************************************************/
	int _time;
	char _timeString[6];
	MonochromeDisplayPainter _painter;
};


class CdPlayerHandler : public IJoystickObserver
{
public:
	CdPlayerHandler()
	{
		IAir3TFactory::factory().joystick().setObserver( this );
	}

private:
	void onPositionChange( IJoystick::Position position )
	{
		switch( position )
		{
			case IJoystick::Right:
			case IJoystick::Up: _player.turnOn(); break;
			case IJoystick::Left:
			case IJoystick::Down: _player.turnOff(); break;
			case IJoystick::Pressed: _player.playPause(); break;
			default: break;
		}
	}

	CdPlayer _player;
};
