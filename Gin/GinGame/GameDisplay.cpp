#include "GameDisplay.h"
#include "../GameLib/Hand.h"
#include "../GameLib/GameState.h"
#include "../GameLib/Deck.h"
#include "../GameLib/DiscardPile.h"
#include "../GameLib/Card.h"
#include "../GameLib/Player.h"
#include "GraphicsSystem.h"
#include "GraphicsSprite.h"
#include "GraphicsColor.h"
#include "CardSpriteManager.h"
#include "CardLocator.h"
#include "Game.h"
#include "HumanPlayer.h"


#include <wtypes.h>
#include <cassert>
#include <sstream>

using namespace std;

const UINT PLAYER_HAND_SCORE_X[MAX_PLAYERS] = { 850, 850, 850, 850 };
const UINT PLAYER_HAND_SCORE_Y[MAX_PLAYERS] = { 40, 140, 240, 340 };
const UINT PLAYER_TOTAL_SCORE_X[MAX_PLAYERS] = { 850, 850, 850, 850 };
const UINT PLAYER_TOTAL_SCORE_Y[MAX_PLAYERS] = { 70, 170, 270, 370 };
const UINT PLAYER_NAME_X[MAX_PLAYERS] = { 850, 850, 850, 850 };
const UINT PLAYER_NAME_Y[MAX_PLAYERS] = { 10, 110, 210, 310 };
const UINT MESSAGE_X = 300;
const UINT MESSAGE_Y = 350;
const UINT DRAW_PILE_X = 10;
const UINT DRAW_PILE_Y = 500;
const UINT CARD_FAN_INCREMENT = 8;
const UINT DISCARD_PILE_X = 400;
const UINT DISCARD_PILE_Y = 500;

void GameDisplay::draw( GameState* pGameState )
{
	bool humanPlayerPlaying = gpGame->humanPlaying();

	//draw hands
	for (UINT i = 0; i<gpGame->getNumPlayers(); i++)
	{
		Hand* pHand = pGameState->getHand( i );
		for( UINT j=0; j<pHand->getNumCards(); j++ )
		{
			const GraphicsSprite& sprite = GraphicsSystem::getCardSpriteManager()->getSprite( (*pHand)[j] );
			UPoint2D point = CardLocator::getCardLocation( i, j );
			GraphicsSystem::drawSprite( sprite, point.x, point.y );
		}
	}

	const GraphicsSprite& cardBack = GraphicsSystem::getCardSpriteManager()->getCardBackSprite();

	//draw deck
	Deck* pDeck = pGameState->getDeck();
	for( UINT i=0; i<pDeck->getNumCardsLeft(); i++ )
	{
		UINT x = DRAW_PILE_X + ( CARD_FAN_INCREMENT * i );
		UINT y = DRAW_PILE_Y;
		GraphicsSystem::drawSprite( cardBack, x, y );
	}

	//draw discard pile
	DiscardPile* pPile = pGameState->getDiscards();

	//draw top card
	if( pPile->getSize() > 0 )
	{
		const GraphicsSprite& sprite = GraphicsSystem::getCardSpriteManager()->getSprite( pPile->viewTopCard() );
		UINT x = DISCARD_PILE_X;
		UINT y = DISCARD_PILE_Y;
		GraphicsSystem::drawSprite( sprite, x, y );
	}

	//draw scores
	for (UINT i = 0; i<gpGame->getNumPlayers(); i++)
	{
		//hand scores
		Hand* pHand = pGameState->getHand( i );
		UINT score = pHand->getHandScore();
		stringstream theStream;
		theStream << "Score: " << score;
		GraphicsSystem::drawText( GraphicsFont::getFont(gpGame->getFontID()), PLAYER_HAND_SCORE_X[i], PLAYER_HAND_SCORE_Y[i], COLOR_BLACK, theStream.str() );

		//total scores
		theStream.str("");
		theStream.clear();
		theStream << "Total Score: " << gpGame->getPlayer(i)->getTotalScore();
		GraphicsSystem::drawText(GraphicsFont::getFont(gpGame->getFontID()), PLAYER_TOTAL_SCORE_X[i], PLAYER_TOTAL_SCORE_Y[i], COLOR_BLACK, theStream.str());

	}

	//current player is human?
	HumanPlayer* pPlayer = dynamic_cast<HumanPlayer*>(gpGame->getCurrentPlayer());
	if( pPlayer != NULL )
	{
		GameState::Phase phase = pGameState->getPhase();
		switch( phase )
		{
		case GameState::DRAW:
			{
				GraphicsSystem::drawText(GraphicsFont::getFont(gpGame->getFontID()), MESSAGE_X, MESSAGE_Y, COLOR_BLACK, "Click Deck to draw from" );
				break;
			}
		case GameState::DISCARD:
			{
				GraphicsSystem::drawText(GraphicsFont::getFont(gpGame->getFontID()), MESSAGE_X, MESSAGE_Y, COLOR_BLACK, "Click Card to Discard - Right click to Knock" );
				break;
			}
		}
	}

	//draw player names
	for (UINT i = 0; i<gpGame->getNumPlayers(); i++)
	{
		Player* pPlayer = gpGame->getPlayer(i);
		GraphicsSystem::drawText(GraphicsFont::getFont(gpGame->getFontID()), PLAYER_NAME_X[i], PLAYER_NAME_Y[i], COLOR_BLACK, pPlayer->getName() );
	}

}
