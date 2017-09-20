#include "CardDrawnEvent.h"

CardDrawnEvent::CardDrawnEvent(DrawCardTransaction::DrawType drawType, const Card& theCard, UINT drawingPlayer, UINT playerToBeNotified)
	:Event(CARD_DRAWN_EVENT, playerToBeNotified, true)
	,mDrawType(drawType)
	,mCard(theCard)
	,mDrawingPlayer(drawingPlayer)
{
}
