#include "Deck.h"
#include <assert.h>
#include <algorithm>

Deck::Deck(bool standard /*= true */)
{
	assert( standard );

	if( standard )
	{
		fillVectorWithStandardCards();
	}
}

Card Deck::dealTopCard()
{
	assert( mCards.size() > 0 );

	Card temp = mCards.back();
	mCards.pop_back();

	return temp;
}

void Deck::shuffle()
{
	random_shuffle( mCards.begin(), mCards.end() );
}

void Deck::reset(bool standard /*= true */)
{
	assert( standard );
	
	mCards.clear();
	fillVectorWithStandardCards();
}

void Deck::fillVectorWithStandardCards()
{
	for( int theSuit = Card::FIRST_SUIT; theSuit <= Card::LAST_SUIT; theSuit++ )
	{
		for( int value = Card::Value::ACE; value <= Card::Value::KING; value++ )
		{
			mCards.push_back( Card( (Card::Value)value, (Card::Suit)theSuit ) );
		}
	}
}

void Deck::addCard(const Card& card)
{
	mCards.push_back(card);
}

