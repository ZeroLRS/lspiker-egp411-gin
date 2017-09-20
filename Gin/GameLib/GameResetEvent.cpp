#include "GameResetEvent.h"


GameResetEvent::GameResetEvent(ResetReason reason, UINT playerNumEndingGame, UINT targetPlayer)
	:Event(GAME_RESET_EVENT, targetPlayer, true)
	, mReason(reason)
	, mPlayerEndingGame(playerNumEndingGame)
{
}
