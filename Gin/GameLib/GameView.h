#pragma once
#include "trackable.h"
#include "GameState.h"
#include <assert.h>

class GameState;
class Cell;

class GameView : public Trackable
{
public:
	GameView( GameState* pState, UINT currentPlayerNum );
	~GameView(){};

	inline Hand getHand() const { assert(mCurrentPlayerNum<mpState->getNumHands()); return mpState->getHand( mCurrentPlayerNum)->clone(); };
	inline UINT getNumCardsInDeck() const { return mpState->getDeck()->getNumCardsLeft(); };
	inline Card viewTopDiscard() const { return mpState->getDiscards()->viewTopCard(); };
	inline UINT getCurrentPlayerNum() const { return mCurrentPlayerNum; };
	inline UINT getKnockThreshold() const { return mpState->getKnockThreshold(); };
	inline GameState::PassDir getPassDirection() const { return mpState->getPassDir(); };

private:
	GameState* mpState;
	UINT mCurrentPlayerNum;
};

