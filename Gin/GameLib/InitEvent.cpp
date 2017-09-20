#include "InitEvent.h"

InitEvent::InitEvent(UINT numPlayers, UINT playerNum)
	: Event(INIT_EVENT, playerNum, true)
	, mNumPlayers(numPlayers)
{

}

