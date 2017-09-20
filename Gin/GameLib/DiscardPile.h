#pragma once

#include "trackable.h"
#include "Card.h"
#include <stack>
#include <wtypes.h>

class DiscardPile :	public Trackable
{
public:
	DiscardPile(){};
	~DiscardPile(){};

	void placeOnDiscard( const Card& card );
	Card removeFromDiscard();
	Card viewTopCard() const;

	void reset();

	inline UINT getSize() const { return mCards.size(); };

private:
	std::stack<Card> mCards;
};

