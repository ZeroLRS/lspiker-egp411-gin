#include <iostream>
#include <assert.h>
#include "GameState.h"
#include "TransactionHandler.h"
#include "Transaction.h"
#include "DrawCardTransaction.h"
#include "DiscardCardTransaction.h"
#include "PassCardTransaction.h"
#include "EventSystem.h"
#include "Event.h"
#include "CardDrawnEvent.h"
#include "CardPlayedEvent.h"
#include "CardPassedEvent.h"
#include "ResetEvent.h"

using namespace std;

GameState::GameState()
	:mpDeck(NULL)
	,mpHands(NULL)
	,mpPile(NULL)
	,mInitialized(false)
	,mNumHands(0)
	,mCurrentPlayerNum(0)
	,mNumCardsPerPlayer(0)
	,mPhase(DRAW)
	,mResetReason(STARTING)
	, mKnockThreshold(KNOCK_THRESHOLD)
{
}

GameState::~GameState()
{
	cleanup();
}

//static
void GameState::handleTransaction( Transaction* pTransaction, GameState* pGameState )
{
	pGameState->handleTransaction( pTransaction );
}

void GameState::handleTransaction( Transaction* pTransaction )
{
	if( pTransaction->getType() == DRAW_TRANSACTION )
	{
		DrawCardTransaction* pDraw = dynamic_cast<DrawCardTransaction*>(pTransaction);
		assert( pDraw != NULL );
		if( pDraw != NULL )
		{
			DrawCardTransaction::DrawType drawType = pDraw->getDrawType();
			UINT playerNum = pDraw->getPlayerNum();
			assert( playerNum >= 0 && playerNum < mNumHands );
			Card cardPlayed;

			if( drawType == DrawCardTransaction::DECK )
			{
				//no cards in deck?
				if (mpDeck->getNumCardsLeft() == 0)
				{
					//addCardsInDiscardPileToDeck();
					mPhase = END_GAME;
					mResetReason = DECK_EMPTY;
					TransactionHandler::getHandler()->postTransaction(new Transaction(END_GAME_TRANSACTION));
					//EventSystem::getEventSystem()->fireEvent(Event(GAME_RESET_EVENT, UINT_MAX, true));
				}
				else
				{
					//deal a card
					assert(mpDeck->getNumCardsLeft() > 0);
					Card theCard = mpDeck->dealTopCard();
					mpHands[playerNum]->addCard(theCard);
				}
			}
			else if( drawType == DrawCardTransaction::DISCARD )
			{
				assert(mpPile->getSize() > 0);
				Card theCard = mpPile->removeFromDiscard();
				mpHands[ playerNum ]->addCard( theCard );
				cardPlayed = theCard;//drawn from discard pile so we can tell players what the card is
			}
			mPhase = DISCARD;
			//notify all players
			for (UINT i = 0; i < mNumHands; i++)
			{
				EventSystem::getEventSystem()->fireEvent(CardDrawnEvent(drawType, cardPlayed, mCurrentPlayerNum, i));
			}
			EventSystem::getEventSystem()->fireEvent(CardDrawnEvent(drawType, cardPlayed, mCurrentPlayerNum, UINT_MAX));

			getHand( playerNum )->sortOnSuitAndValue();

		}
	}
	else if( pTransaction->getType() == DISCARD_TRANSACTION )
	{
		DiscardCardTransaction* pDiscard = dynamic_cast<DiscardCardTransaction*>(pTransaction);
		assert( pDiscard != NULL );
		if( pDiscard != NULL )
		{
			Card theCard = pDiscard->getCard();
			UINT playerNum = pDiscard->getPlayerNum();
			assert( playerNum >= 0 && playerNum < mNumHands );
			mpPile->placeOnDiscard( theCard );
			mpHands[ playerNum ]->removeCard( theCard );

			if( pDiscard->isKnocking() && mpHands[playerNum]->getHandScore() <= mKnockThreshold )//end game only if knocking player has knock threshold or less deadwood
			{
				mPhase = END_GAME;
				//EventSystem::getEventSystem()->fireEvent( Event(KNOCK_EVENT, playerNum) );
				EventSystem::getEventSystem()->fireEvent(Event(KNOCK_EVENT, UINT_MAX));
			}
			else //game continues
			{
				mPhase = DRAW;

				//notify all players
				for (UINT i = 0; i < mNumHands; i++)
				{
					EventSystem::getEventSystem()->fireEvent(CardPlayedEvent(theCard, mCurrentPlayerNum, i));
				}
				EventSystem::getEventSystem()->fireEvent( CardPlayedEvent(theCard, mCurrentPlayerNum, UINT_MAX) );
			}
		}
	}
	else if ( pTransaction->getType() == PASS_CARD_TRANSACTION )
	{
		PassCardTransaction* pPass = dynamic_cast<PassCardTransaction*>(pTransaction);
		assert(pPass != NULL);
		if ( pPass != NULL )
		{
			Card theCard = pPass->getCard();
			UINT passerNum = pPass->getPlayerNum();
			assert( passerNum >= 0 && passerNum < mNumHands );
			mpHands[passerNum]->removeCard(theCard);

			mPassCards[passerNum] = theCard;

			//cout << passerNum << " passed " << theCard.getValueName() << " of " << theCard.getSuitName() << " to the GameState" << endl;
			
			++mNumPlayersPassed;
			//check if all players have passed
			// if they haven't, wait for remaining players
			// if they have, proceed to actual game
			if (mNumPlayersPassed > (mNumHands - 1))
			{
				//actually pass cards to each player
				for (UINT i = 0; i < mNumHands; ++i)
				{
					theCard = mPassCards[i];
					passerNum = i;

					UINT receiverNum = passerNum; //default returns card to original passer's hand

					if (mPassDir == PassDir::LEFT)
					{
						receiverNum = ((int)passerNum - 1 >= 0) ? passerNum - 1 : mNumHands - 1;
					}
					else if (mPassDir == PassDir::RIGHT)
					{
						receiverNum = (passerNum + 1 < mNumHands) ? passerNum + 1 : 0;
					}

					assert(receiverNum >= 0 && receiverNum < mNumHands);
					mpHands[receiverNum]->addCard(theCard);

					//cout << passerNum << " passed " << theCard.getValueName() << " of " << theCard.getSuitName() << " to " << receiverNum << endl;

					//notify passer and receiver
					EventSystem::getEventSystem()->fireEvent(CardPassedEvent(theCard, passerNum, receiverNum, passerNum));
					EventSystem::getEventSystem()->fireEvent(CardPassedEvent(theCard, passerNum, receiverNum, receiverNum));
				}

				//then the game begins
				mPhase = DRAW;
				mNumPlayersPassed = 0;
				int tempDir = (int)mPassDir;
				mPassDir = (PassDir)(++tempDir % (int)PassDir::NO_PASS);
				//tell gpGame that all cards have been passed and the game can begin
				EventSystem::getEventSystem()->fireEvent(CardPassedEvent(theCard, passerNum, 0, UINT_MAX));
			}
			else
			{
				//notify gpGame that a card has been passed to the GameState
				//receiverNum is irrelevant here
				EventSystem::getEventSystem()->fireEvent(CardPassedEvent(theCard, passerNum, 0, UINT_MAX));
			}
		}
	}
	else if (pTransaction->getType() == END_GAME_TRANSACTION)
	{
		EventSystem::getEventSystem()->fireEvent(ResetGameEvent(getDiscards()->viewTopCard(), getNumHands(), UINT_MAX));
	}
	//cleanup transaction
	delete pTransaction;
}

void GameState::init(UINT numPlayers /*= NUM_PLAYERS*/, UINT numCardsPerPlayer /*= CARDS_PER_PLAYER */, UINT knockThreshold /*= KNOCK_THRESHOLD*/)
{
	if( !mInitialized )
	{
		mpDeck = new Deck;
		mpHands = new Hand*[numPlayers];
		for( UINT i=0; i<numPlayers; i++ )
		{
			mpHands[i] = new Hand;
		}
		mpPile = new DiscardPile;
		mNumCardsPerPlayer = numCardsPerPlayer;
		mInitialized = true;
		mNumHands = numPlayers;
		mKnockThreshold = knockThreshold;
		mPassDir = LEFT;
	}
	doInitialActions();
}

void GameState::cleanup()
{
	delete mpDeck;
	mpDeck = NULL;
	for( UINT i=0; i<mNumHands; i++ )
	{
		delete mpHands[i];
	}
	delete [] mpHands;
	mpHands = NULL;
	delete mpPile;
	mpPile = NULL;

	mInitialized = false;
	mNumHands = 0;
	mNumCardsPerPlayer = 0;
}

void GameState::reset()
{
	doInitialActions();
}

void GameState::doInitialActions()
{
	for (UINT i = 0; i < mNumHands; i++)
	{
		mpHands[i]->reset();
	}

	mpPile->reset();

	mpDeck->reset();
	mpDeck->shuffle();

	//deal cards
	for( UINT i=0; i<mNumCardsPerPlayer; i++ )
	{
		for (UINT playerNum = 0; playerNum < mNumHands; playerNum++)
		{
			mpHands[playerNum]->addCard(mpDeck->dealTopCard());
		}
	}

	mpPile->placeOnDiscard( mpDeck->dealTopCard() );

	mPhase = PASS;
	mNumPlayersPassed = 0;
	mResetReason = STARTING;
}

void GameState::addCardsInDiscardPileToDeck()
{
	while (mpPile->getSize() > 0)
	{
		mpDeck->addCard(mpPile->removeFromDiscard());
	}
	mpDeck->shuffle();
}

UINT GameState::getLowestScoringPlayer() const
{
	UINT lowestIndex = 0;
	UINT lowestScore = UINT_MAX;

	for (UINT i = 0; i < mNumHands; i++)
	{
		UINT score = mpHands[i]->getHandScore();
		if (score < lowestScore)
		{
			lowestScore = score;
			lowestIndex = i;
		}
	}

	return lowestIndex;
}

UINT GameState::getWinningPlayerScore() const
{
	UINT winner = getLowestScoringPlayer();

	UINT totalScore = 0;

	for (UINT i = 0; i < mNumHands; i++)
	{
		if (i != winner)
		{
			totalScore += mpHands[i]->getHandScore();
		}
	}
	totalScore -= mpHands[winner]->getHandScore();

	return totalScore;
}


