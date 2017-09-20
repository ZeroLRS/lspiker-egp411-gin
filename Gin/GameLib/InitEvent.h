#pragma once

#include "event.h"

class InitEvent : public Event
{
public:
	InitEvent(UINT numPlayers, UINT playerNum);
	~InitEvent(){};

	inline UINT getNumPlayers() const { return mNumPlayers; };
private:
	UINT mNumPlayers;
};

