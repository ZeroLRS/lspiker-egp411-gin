#pragma once

#include "trackable.h"
#include <vector>
#include <wtypes.h>
#include "Card.h"

class Deck : public Trackable
{
public:
	Deck( bool standard = true );
	~Deck(){};

	inline UINT getNumCardsLeft() const { return mCards.size(); };
	Card dealTopCard();
	void shuffle();
	void reset( bool standard = true );
	void addCard(const Card& card);

private:
	std::vector<Card> mCards;

	void fillVectorWithStandardCards();
};

