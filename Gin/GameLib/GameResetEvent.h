#pragma once

#include "event.h"



class GameResetEvent : public Event
{
public:
	enum ResetReason
	{
		KNOCK,
		GIN,
		DECK_EMPTY
	};

	GameResetEvent(GameResetEvent::ResetReason reason, UINT playerNumEndingGame, UINT targetPlayer);
	~GameResetEvent(){};

	inline GameResetEvent::ResetReason getReason() const { return mReason; };
	inline UINT getPlayerEndingGame() const { return mPlayerEndingGame; };
private:
	GameResetEvent::ResetReason mReason;
	UINT mPlayerEndingGame;
};

