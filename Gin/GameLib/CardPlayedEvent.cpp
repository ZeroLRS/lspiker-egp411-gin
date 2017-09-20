#include "CardPlayedEvent.h"



CardPlayedEvent::CardPlayedEvent(const Card& theCard, UINT playingPlayer, UINT playerToBeNotified)
	:Event(CARD_PLAYED_EVENT, playerToBeNotified, true)
	,mCard(theCard)
	, mPlayingPlayer(playingPlayer)
{
}
