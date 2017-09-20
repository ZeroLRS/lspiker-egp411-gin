#include "FireEventTask.h"
#include "Game.h"
#include "..\GameLib\EventSystem.h"
#include "..\GameLib\GameView.h"
#include "..\GameLib\GameState.h"
#include "..\GameLib\GameViewBaseEvent.h"
#include "..\GameLib\InitEvent.h"
#include "..\GameLib\ResetEvent.h"


FireEventTask::FireEventTask(EventType type)
	:mEventType( type )
	, mTheEvent()
{
}

bool FireEventTask::update()
{
	switch( mEventType )
	{
	case DRAW_CARD_REQUEST:
		{
			EventSystem::getEventSystem()->fireEvent( GameViewBaseEvent( GameView( gpGame->getGameState(), gpGame->getGameState()->getCurrentPlayerNum() ), DRAW_CARD_REQUEST, gpGame->getGameState()->getCurrentPlayerNum() ) );
			flagAsDone();
			return true;
		}
	case DISCARD_CARD_REQUEST:
		{
			EventSystem::getEventSystem()->fireEvent( GameViewBaseEvent( GameView( gpGame->getGameState(), gpGame->getGameState()->getCurrentPlayerNum() ), DISCARD_CARD_REQUEST, gpGame->getGameState()->getCurrentPlayerNum() ) );
			flagAsDone();
			return true;
		}
	case PASS_CARD_REQUEST:
		{
			EventSystem::getEventSystem()->fireEvent( GameViewBaseEvent( GameView( gpGame->getGameState(), gpGame->getGameState()->getCurrentPlayerNum()), PASS_CARD_REQUEST, gpGame->getGameState()->getCurrentPlayerNum() ) );
			flagAsDone();
			return true;
		}
	case INIT_EVENT:
	{
		EventSystem::getEventSystem()->fireEvent(InitEvent(gpGame->getNumPlayers(), UINT_MAX));
		flagAsDone();
		return true;
	}
	case GAME_RESET_EVENT:
	{
		EventSystem::getEventSystem()->fireEvent(ResetGameEvent(gpGame->getGameState()->getDiscards()->viewTopCard(), gpGame->getNumPlayers(),UINT_MAX));
		flagAsDone();
		return true;
	}
	case NUM_EVENT_TYPES:
	{
		assert(false);//shouldn't happen
		return true;
	}
	default:
	{
		EventSystem::getEventSystem()->fireEvent(Event(mEventType, UINT_MAX, true));
		flagAsDone();
		return true;
	}
	}
	return false;
}

