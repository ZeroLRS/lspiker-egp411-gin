#pragma once

#include "trackable.h"
#include "Card.h"
#include <vector>
#include <wtypes.h>

class Hand : public Trackable
{
public:
	Hand(){};
	~Hand(){};

	Hand clone() const;

	void addCard( const Card& card );
	bool removeCard( const Card& card );//returns false if not found

	void sortOnSuitAndValue();
	void sortOnValue();

	void reset();

	inline UINT getNumCards() const { return mCards.size(); };
	const Card& operator[](UINT index);

	UINT getHandScore() const;
	UINT scoreExistingCards() const;

	Card getCard( UINT index ) const;

	void logContents() const;

	std::vector<Card> removeMatches(bool shouldSortOnValue);//return the actual matches

private:
	std::vector<Card> mCards;
};

