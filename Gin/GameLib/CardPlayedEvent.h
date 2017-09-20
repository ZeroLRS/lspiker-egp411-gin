#pragma once

#include "event.h"
#include "Card.h"

class CardPlayedEvent :	public Event
{
public:
	CardPlayedEvent( const Card& theCard, UINT playingPlayer, UINT playerToBeNotified );
	~CardPlayedEvent(){};

	inline const Card& getCard() const { return mCard; };
	inline UINT getPlayingPlayer() const { return mPlayingPlayer; };
private:
	const Card& mCard;
	UINT mPlayingPlayer;
};

