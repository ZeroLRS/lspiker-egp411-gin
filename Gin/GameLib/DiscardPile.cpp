#include "DiscardPile.h"
#include <assert.h>

void DiscardPile::placeOnDiscard(const Card& card)
{
	mCards.push( card );
}

Card DiscardPile::removeFromDiscard()
{
	assert( mCards.size() > 0 );
	Card temp = mCards.top();
	mCards.pop();

	return temp;
}

Card DiscardPile::viewTopCard() const
{
	assert( mCards.size() > 0 );
	return mCards.top();
}

void DiscardPile::reset()
{
	while( !mCards.empty() )
	{
		mCards.pop();
	}
}
