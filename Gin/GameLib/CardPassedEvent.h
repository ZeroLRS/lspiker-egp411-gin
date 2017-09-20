#pragma once

#include "Event.h"
#include "Card.h"

class CardPassedEvent : public Event
{
public:
	CardPassedEvent( const Card& theCard, UINT passingPlayer, UINT receivingPlayer, UINT playerToBeNotified );
	~CardPassedEvent() {};

	inline const Card& getCard() const { return mCard; };
	inline UINT getPassingPlayer() const { return mPassingPlayer; };
	inline UINT getReceivingPlayer() const { return mReceivingPlayer; };

private:
	const Card& mCard;
	UINT mPassingPlayer;
	UINT mReceivingPlayer;
};