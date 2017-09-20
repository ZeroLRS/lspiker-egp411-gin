#include <wtypes.h>
#include <cassert>
#include "CardLocator.h"
#include "Game.h"
#include "..\GameLib\GameState.h"
#include "..\GameLib\Hand.h"


const UINT PLAYER_HAND_X[MAX_PLAYERS] = { 10, 10, 10, 10 };
const UINT PLAYER_HAND_Y[MAX_PLAYERS] = { 10, 110, 210, 310 };
const UINT CARD_PIXELS_X = 72;
const UINT CARD_PIXELS_Y = 96;
const UINT DRAW_PILE_X = 10;
const UINT DRAW_PILE_Y = 500;
const UINT CARD_FAN_INCREMENT = 8;
const UINT DISCARD_PILE_X = 400;
const UINT DISCARD_PILE_Y = 500;


UPoint2D CardLocator::getCardLocation(UINT playerNum, UINT cardNum)
{
	UINT x = PLAYER_HAND_X[playerNum] + cardNum*CARD_PIXELS_X;
	UINT y = PLAYER_HAND_Y[playerNum];
	return UPoint2D( x, y );
}

UPoint2D CardLocator::getCardPixelSize()
{
	return UPoint2D( CARD_PIXELS_X, CARD_PIXELS_Y );
}

Card CardLocator::getCard(UINT x, UINT y, UINT playerNum)
{
	//check each card of the given player's hand

	//first subtract the starting x location
	x -= PLAYER_HAND_X[playerNum];

	//divide by the pixel width
	x /= CARD_PIXELS_X;

	if( x < gpGame->getGameState()->getHand( playerNum )->getNumCards() )
	{
		if( y >= PLAYER_HAND_Y[playerNum] && y < PLAYER_HAND_Y[playerNum] + CARD_PIXELS_Y )
		{
			return gpGame->getGameState()->getHand( playerNum )->getCard( x );
		}
	}

	return Card(Card::NUM_VALUES, Card::NUM_SUITS);

}

DrawCardTransaction::DrawType CardLocator::getDrawType(UINT x, UINT y)
{
	//check draw deck first
	UINT xlow = DRAW_PILE_X;
	UINT ylow = DRAW_PILE_Y;
	UINT xhigh = xlow;
	UINT yhigh = ylow + CARD_PIXELS_Y;

	UINT numCards = gpGame->getGameState()->getDeck()->getNumCardsLeft();
	if( numCards > 0 )
	{
		xhigh = xlow + CARD_PIXELS_X + ( ( numCards - 1) * CARD_FAN_INCREMENT );
	}

	if( x >= xlow && x < xhigh && y >= ylow && y < yhigh )
	{
		return DrawCardTransaction::DECK;
	}

	//check discard pile
	xlow = DISCARD_PILE_X;
	ylow = DISCARD_PILE_Y;
	xhigh = xlow + CARD_PIXELS_X;
	yhigh = ylow + CARD_PIXELS_Y;

	if(  gpGame->getGameState()->getDiscards()->getSize() > 0 )
	{
		if( x >= xlow && x < xhigh && y >= ylow && y < yhigh )
		{
			return DrawCardTransaction::DISCARD;
		}
	}

	return DrawCardTransaction::NUM_TYPES;
}
