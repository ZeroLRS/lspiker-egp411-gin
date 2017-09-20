#include "Card.h"

Card::Card(Value val, Suit suit)
	:mValue(val)
	,mSuit(suit)
{

}

Card::Card()
	:Card(NUM_VALUES,NUM_SUITS)
{

}

bool Card::sortOrderSuitAndValue( const Card& lhs, const Card& rhs )
{
	if( lhs.mSuit < rhs.mSuit )
	{
		return true;
	}
	else if( lhs.mSuit > rhs.mSuit )
	{
		return false;
	}
	else
	{
		return lhs.mValue < rhs.mValue;
	}
}

const std::string& Card::getSuitName() const
{
	static std::string names[Card::NUM_SUITS+1] = {"H", "D", "C", "S", "Not a suit" };
	return names[mSuit];
}

const std::string& Card::getValueName() const
{
	static std::string names[Card::NUM_VALUES] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
	return names[mValue];
}

bool Card::isValid() const
{
	if (mSuit == NUM_SUITS || mValue == NUM_VALUES)
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::ostream& operator<<( std::ostream& out, Card theCard )
{
	out << theCard.getValueName() << theCard.getSuitName();
	return out;
}

const UINT CARD_POINTS[Card::NUM_VALUES] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };

UINT Card::getPointValue() const 
{ 
	return CARD_POINTS[mValue]; 
};
