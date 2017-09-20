#include <iostream>
#include <cstdlib>

#include "DLLCode.h"
#include "..\GameLib\GameView.h"
#include "..\GameLib\Transaction.h"
#include "..\GameLib\TransactionHandler.h"
#include "..\GameLib\Event.h"
#include "..\GameLib\GameViewBaseEvent.h"
#include "..\GameLib\CardPlayedEvent.h"
#include "..\GameLib\CardDrawnEvent.h"
#include "..\GameLib\CardPassedEvent.h"
#include "..\GameLib\InitEvent.h"
#include "..\GameLib\ResetEvent.h"
#include "..\GameLib\DrawCardTransaction.h"
#include "..\GameLib\DiscardCardTransaction.h"
#include "..\GameLib\PassCardTransaction.h"

void drawCardRequest( const Event& theEvent, TransactionHandler* pHandler )
{
	//place code here to respond to card draw requests - places appropriate transaction(s) in handler
	if( theEvent.getType() == DRAW_CARD_REQUEST )
	{
		//cout << "draw card request\n";
		const GameViewBaseEvent& gameviewEvent = dynamic_cast<const GameViewBaseEvent&>(theEvent);
		const GameView& gameView = gameviewEvent.getGameView();

		//always draw from the deck
		DrawCardTransaction::DrawType drawType = DrawCardTransaction::DECK;

		pHandler->postTransaction( new DrawCardTransaction(drawType, gameView.getCurrentPlayerNum() ) );
	}
}

void discardCardRequest( const Event& theEvent, TransactionHandler* pHandler )
{
	//place code here to respond to card discard requests - places appropriate transaction(s) in handler
	if( theEvent.getType() == DISCARD_CARD_REQUEST )
	{
		//cout << "discard card request\n";
		const GameViewBaseEvent& gameviewEvent = dynamic_cast<const GameViewBaseEvent&>(theEvent);
		const GameView& gameView = gameviewEvent.getGameView();

		//discard the highest value card
		Hand hand = gameView.getHand();
		hand.sortOnValue();

		bool knock = false;
		//get the card to be played
		Card theDiscard = hand[hand.getNumCards()-1];
		//remove it from the temp hand so it can be scored
		hand.removeCard( theDiscard );
		UINT score = hand.getHandScore();

		//note: you should knock if you have gin as well!!!
		if( score <=  gameView.getKnockThreshold())//if score is low enough to knock - then set knock flag to true
		{
			knock = true;
			//cout << "DLL knock\n";
		}
		pHandler->postTransaction( new DiscardCardTransaction( theDiscard , gameView.getCurrentPlayerNum(), knock ) );
	}
}

void passCardRequest( const Event& theEvent, TransactionHandler* pHandler )
{
	//place code here to respond to card pass requests - places appropriate transaction(s) in handler
	if ( theEvent.getType() == PASS_CARD_REQUEST )
	{
		cout << "Pass card request!" << endl;
		const GameViewBaseEvent& gameViewEvent = dynamic_cast<const GameViewBaseEvent&>(theEvent);
		const GameView& gameView = gameViewEvent.getGameView();

		cout << "Pass Direction:" << gameView.getPassDirection() << endl;

		//pass highest value card
		Hand hand = gameView.getHand();
		hand.sortOnValue();

		//get the card to be played
		Card theDiscard = hand[hand.getNumCards() - 1];

		pHandler->postTransaction( new PassCardTransaction( theDiscard, gameView.getCurrentPlayerNum() ) );
	}
}

void cardPlayedNotification( const Event& theEvent, TransactionHandler* pHandler )
{
	//pHandler should be NULL because we are not supposed to post any transactions - just track plays (if wanted)
	assert( pHandler == NULL );

	//place code here to track what cards are played
	if( theEvent.getType() == CARD_PLAYED_EVENT )
	{
		//cout << "Card played notification\n";
		const CardPlayedEvent& cardPlayedEvent = dynamic_cast<const CardPlayedEvent&>(theEvent);
		const Card& theCard = cardPlayedEvent.getCard();
		UINT playingPlayer = cardPlayedEvent.getPlayingPlayer();
		//cout << "Card played notification " << playingPlayer << endl;
	}
}

void cardDrawnNotification( const Event& theEvent, TransactionHandler* pHandler )
{
	//pHandler should be NULL because we are not supposed to post any transactions
	assert(pHandler == NULL);


	//place code here to track where cards are drawn from
	if (theEvent.getType() == CARD_DRAWN_EVENT)
	{
		//cout << "Card drawn notification\n";
		const CardDrawnEvent& cardDrawnEvent = dynamic_cast<const CardDrawnEvent&>(theEvent);
		DrawCardTransaction::DrawType type = cardDrawnEvent.getDrawType();
		const Card& theCard = cardDrawnEvent.getCard();
		UINT drawingPlayer = cardDrawnEvent.getDrawingPlayer();
		//cout << "Card drawn notification " << drawingPlayer << endl;
		if (theCard.isValid())//valid card means it was from the discard pile
		{
			assert(type == DrawCardTransaction::DISCARD);
		}
		else
		{
			//invalid card should be from the deck
			assert(type == DrawCardTransaction::DECK);
		}
	}
}

void cardPassedNotification(const Event& theEvent, TransactionHandler* pHandler)
{
	//pHandler should be NULL because we are not supposed to post any transactions
	assert(pHandler == NULL);

	//place code here to track where you received a card from & where your passed card went
	if (theEvent.getType() == CARD_PASSED_EVENT)
	{
		const CardPassedEvent& cardPassedEvent = dynamic_cast<const CardPassedEvent&>(theEvent);
		const Card& theCard = cardPassedEvent.getCard();
		UINT passingPlayer = cardPassedEvent.getPassingPlayer();
		UINT receivingPlayer = cardPassedEvent.getReceivingPlayer();
	}
}

void generalGameNotification(const Event& theEvent, TransactionHandler* pHandler)
{
	//pHandler should be NULL because we are not supposed to post any transactions
	assert(pHandler == NULL);

	//place code here to respond to general notifications from the game
	if (theEvent.getType() == INIT_EVENT)
	{
		const InitEvent& initEvent = dynamic_cast<const InitEvent&>(theEvent);
		UINT numPlayers = initEvent.getNumPlayers();
		//cout << "INIT_EVENT:" << numPlayers << " players\n";
	}
	else if (theEvent.getType() == GAME_RESET_EVENT)
	{
		cout << "RESET_EVENT\n";
		const ResetGameEvent& resetEvent = dynamic_cast<const ResetGameEvent&>(theEvent);
		Card initialDiscard = resetEvent.getInitialDiscard();
	}
	else if (theEvent.getType() == CLEANUP_EVENT)
	{
		//cout << "CLEANUP_EVENT\n";
	}
}
