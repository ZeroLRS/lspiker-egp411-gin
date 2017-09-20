#pragma once

#include "event.h"
#include "Card.h"

class ResetGameEvent : public Event
{
public:
	ResetGameEvent(const Card& initialDiscard, UINT numPlayers, UINT playerNum);
	~ResetGameEvent(){};

	inline UINT getNumPlayers() const { return mNumPlayers; };
	inline const Card& getInitialDiscard() const { return mInitialDiscard; };
private:
	UINT mNumPlayers;
	Card mInitialDiscard;
};

