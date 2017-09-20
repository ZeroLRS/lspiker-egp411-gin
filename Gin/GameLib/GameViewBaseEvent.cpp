#include "GameViewBaseEvent.h"


GameViewBaseEvent::GameViewBaseEvent(const GameView& theView, EventType type, UINT playerNum)
	:Event(type, playerNum)
	,mGameView(theView)
{
}
