#pragma once
#include "..\gamelib\trackable.h"
#include "..\gamelib\card.h"
#include "..\gamelib\DrawCardTransaction.h"

struct UPoint2D: public Trackable
{
	UPoint2D( UINT theX, UINT theY ):x(theX), y(theY){};
	UINT x;
	UINT y;
};

class CardLocator :	public Trackable
{
public:
	//CardLocator(){};
	//~CardLocator(){};

	static UPoint2D getCardLocation( UINT playerNum, UINT cardNum );
	static Card getCard( UINT x, UINT y, UINT playerNum );//returns bogus card (suit=NUM_SUITS (4) if not clicked on a valid hand card
	static DrawCardTransaction::DrawType getDrawType( UINT x, UINT y );//returns draw pile clicked on - NUM_TYPES id nothing
	static UPoint2D getCardPixelSize();
};

