#pragma once

#include <string>
#include <wtypes.h>
#include <cassert>
#include "Trackable.h"
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "DiscardPile.h"

class Game;
class Transaction;

const UINT MAX_PLAYERS = 4;
const UINT CARDS_PER_PLAYER = 10;
const UINT KNOCK_THRESHOLD = 10;

class GameState: public Trackable
{
public:
	friend class Game;

	enum Phase
	{
		PASS,
		DRAW,
		DISCARD,
		END_GAME
	};

	enum ResetReason
	{
		STARTING,
		KNOCK,
		GIN,
		DECK_EMPTY
	};

	enum PassDir
	{
		LEFT=0,
		RIGHT,
		NO_PASS
	};


	//default constructor
	GameState();
	//destructor
	~GameState();

	void init( UINT numPlayers = MAX_PLAYERS, UINT numCardsPerPlayer = CARDS_PER_PLAYER, UINT knockThreshold = KNOCK_THRESHOLD );
	void cleanup();

	void reset();

	//to be used when creating TransactionHandler's
	static void handleTransaction( Transaction* pTransaction, GameState* pGameState );

	inline Deck* getDeck() const { return mpDeck; };
	inline Hand* getHand( UINT playerNum ) const { assert( playerNum < mNumHands ); return mpHands[playerNum]; };
	inline DiscardPile* getDiscards() const { return mpPile; };
	inline UINT getNumHands() const { return mNumHands; };
	inline UINT getCurrentPlayerNum() const { return mCurrentPlayerNum; };
	inline void advancePlayerTurn() { mCurrentPlayerNum = (mCurrentPlayerNum + 1)%mNumHands; };
	inline void setCurrentPlayerTurn(UINT playerNum) { mCurrentPlayerNum = playerNum; };
	inline Phase getPhase() const { return mPhase; };
	inline PassDir getPassDir() const { return mPassDir; };
	inline ResetReason getResetReason() const { return mResetReason; };
	inline UINT getKnockThreshold() const { return mKnockThreshold; };

	void addCardsInDiscardPileToDeck();

	UINT getLowestScoringPlayer() const;
	UINT getWinningPlayerScore() const;

private:
	Deck* mpDeck;
	Hand** mpHands;
	UINT mNumHands;
	DiscardPile* mpPile;
	UINT mNumCardsPerPlayer;
	UINT mCurrentPlayerNum;
	Phase mPhase;
	GameState::PassDir mPassDir;
	UINT mNumPlayersPassed;
	Card mPassCards[MAX_PLAYERS];
	GameState::ResetReason mResetReason;
	UINT mKnockThreshold;
	bool mInitialized;

	//called from the static handleTransaction function
	void handleTransaction( Transaction* pTransaction );

	void doInitialActions();


};

