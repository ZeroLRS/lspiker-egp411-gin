#include "ResetEvent.h"

ResetGameEvent::ResetGameEvent(const Card& initialDiscard, UINT numPlayers, UINT playerNum)
	: Event(GAME_RESET_EVENT, playerNum, true)
	, mNumPlayers(numPlayers)
	, mInitialDiscard(initialDiscard)

{
}

