#include "CardPassedEvent.h"

CardPassedEvent::CardPassedEvent(const Card& theCard, UINT passingPlayer, UINT receivingPlayer, UINT playerToBeNotified)
	:Event(CARD_PASSED_EVENT, playerToBeNotified, true)
	,mCard(theCard)
	,mPassingPlayer(passingPlayer)
	,mReceivingPlayer(receivingPlayer)
{

}