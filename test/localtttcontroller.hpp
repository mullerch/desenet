#pragma once
#include <xf/xfreactive.h>
#include <air3t/ui/IViewObserver>
#include <air3t/data/Model>
#include <air3t/ui/GameView>
#include <IAir3TFactory>
#include <utils/MonochromeDisplayPainter>
#include <IJoystick>


/**
 * @ingroup test
 * Lets the user play a local TicTacToe game and tests the XF, the joystick, the display and the MVC pattern of the
 * game.
 *
 * The FSM of the LocalTTTController looks like this:
 * @image html LocalTTTController.png
 */
class LocalTTTController : public Air3T::IViewObserver , public XFReactive
{
public:
	/**
	 * Event pushed into the event queue if the user has selected a field to place his symbol.
	 */
	class FieldSelectionEvent : public IXFEvent
	{
	public:
		static const int Id = 1;
		FieldSelectionEvent( const Air3T::GameField::Field & field )
			: IXFEvent( Event , Id , NULL ) , _field( field ) {}
		inline const Air3T::GameField::Field field() const { return _field; }
	private:
		Air3T::GameField::Field _field;
	};

	/**
	 * Initilizes the local game controller, the paint system, the model and the used game view.
	 */
	LocalTTTController() : _state( InitialState ) , _view( NULL )
	{
		// Initialize the game.
		_painter.begin( &IAir3TFactory::factory().monochromeDisplay() );

		// Initialize the game view.
		_view = new Air3T::GameView( _painter , _model );
		_view->setObserver( this );
		_model.registerView( _view );

		// Initialize the joystick with myself as observer.
		IAir3TFactory::factory().joystick().setObserver( _view );

		// Start FSM.
		startBehavior();
	}

	/**
	 * Free dynamically allocated memory.
	 */
	virtual ~LocalTTTController()
	{
		delete _view;
	}

	/**
	 * FSM implementation.
	 */
	EventStatus processEvent()
	{
		EventStatus status = EventStatus::NotConsumed;

		// Get event.
		IXFEvent * event = getCurrentEvent();

		// Calculate transitions.
		State _oldState = _state;
		switch( _state )
		{
			case InitialState:
				if ( event->getEventType() == IXFEvent::Initial )
				{
					_state = StartGameState;
					status = EventStatus::Consumed;
				}
				break;

			case StartGameState:
				if ( event->getEventType() == IXFEvent::NullTransition )
				{
					_state = WaitForPlayer1State;
					status = EventStatus::Consumed;
				}
				break;

			case WaitForPlayer1State:
				if ( event->getEventType() == IXFEvent::Event &&
					 event->getId() == FieldSelectionEvent::Id )
					{
						_state = PlacePlayer1SymbolState;
						status = EventStatus::Consumed;
					}
				break;

			case PlacePlayer1SymbolState:
				if ( event->getEventType() == IXFEvent::NullTransition )
				{
					if ( _model.gamefield().winner() != Air3T::GameField::None )
						_state = ShowWinnerState;
					else
						_state = WaitForPlayer2State;
					status = EventStatus::Consumed;
				}
				break;

			case WaitForPlayer2State:
				if ( event->getEventType() == IXFEvent::Event &&
					 event->getId() == FieldSelectionEvent::Id )
				{
					_state = PlacePlayer2SymbolState;
					status = EventStatus::Consumed;
				}
				break;

			case PlacePlayer2SymbolState:
				if ( event->getEventType() == IXFEvent::NullTransition )
				{
					if ( _model.gamefield().winner() != Air3T::GameField::None )
						_state = ShowWinnerState;
					else
						_state = WaitForPlayer1State;
					status = EventStatus::Consumed;
				}
				break;

			case ShowWinnerState:
				if ( event->getEventType() == IXFEvent::Timeout )
				{
					_state = StartGameState;
					status = EventStatus::Consumed;
				}
				break;
		}

		// Execute on entry code.
		if ( _state != _oldState )
		{
			switch( _state )
			{
				case InitialState:
					GEN( XFNullTransition() );
					break;

				case StartGameState:

					_model.gamefield().clear();
					_view->setCursorPosition( Air3T::GameField::Field( 0 , 0 ) );
					_view->setCursorEnabled( true );

					GEN( XFNullTransition() );
					break;

				case WaitForPlayer1State:
					break;

				case PlacePlayer1SymbolState:
					{
						FieldSelectionEvent *se = static_cast<FieldSelectionEvent *>( event );

						_model.gamefield().setSymbol( se->field() , Air3T::GameField::Circle );

						GEN( XFNullTransition() );
					}
					break;

				case WaitForPlayer2State:
					break;

				case PlacePlayer2SymbolState:
					{
						FieldSelectionEvent *se = static_cast<FieldSelectionEvent *>( event );

						_model.gamefield().setSymbol( se->field() , Air3T::GameField::Cross );

						GEN( XFNullTransition() );
					}
					break;

				case ShowWinnerState:
					_view->setCursorEnabled( false );
					getThread()->scheduleTimeout( 0 , 2500 , this );
					break;
			}
		}

		return status;
	}

	/**
	 * Handle the event when the user has selected a field to place his symbol.
	 */
	void onGameFieldSelection( const Air3T::GameField::Field & field )
	{
		// Convert the method call into an event...
		GEN( FieldSelectionEvent( field ) );
	}

private:
	enum State
	{
		InitialState ,
		StartGameState ,
		WaitForPlayer1State ,
		PlacePlayer1SymbolState ,
		WaitForPlayer2State ,
		PlacePlayer2SymbolState ,
		ShowWinnerState
	} _state;

	Air3T::Model _model;
	Air3T::GameView * _view;

	MonochromeDisplayPainter _painter;
};
