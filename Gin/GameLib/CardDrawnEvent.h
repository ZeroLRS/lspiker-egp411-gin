#pragma once

#include "event.h"
#include "DrawCardTransaction.h"
#include "Card.h"

class CardDrawnEvent : public Event
{
public:
	CardDrawnEvent( DrawCardTransaction::DrawType drawType, const Card& theCard, UINT drawingPlayer, UINT playerToBeNotified );
	~CardDrawnEvent(){};

	inline DrawCardTransaction::DrawType getDrawType() const { return mDrawType; };
	inline const Card& getCard() const { return mCard; };
	inline UINT getDrawingPlayer() const { return mDrawingPlayer; };
private:
	DrawCardTransaction::DrawType mDrawType;
	Card mCard;
	UINT mDrawingPlayer;
};

