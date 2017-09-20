#pragma once

#include "event.h"

class GameView;

class GameViewBaseEvent : public Event
{
public:
public:
	GameViewBaseEvent( const GameView& theView, EventType type, UINT playerNum );
	~GameViewBaseEvent(){};

	inline const GameView& getGameView() const { return mGameView; };
private:
	const GameView& mGameView;
};

