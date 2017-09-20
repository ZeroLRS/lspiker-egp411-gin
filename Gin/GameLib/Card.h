#pragma once

#include "trackable.h"
#include <string>
#include <iostream>
#include <wtypes.h>


class Card : public Trackable
{
public:

	friend std::ostream& operator<<( std::ostream& out, Card theCard );

	enum Suit
	{
		HEARTS,
		DIAMONDS,
		CLUBS,
		SPADES,
		NUM_SUITS
	};

	const static Suit FIRST_SUIT = HEARTS;
	const static Suit LAST_SUIT = SPADES;

	enum Value
	{
		ACE = 0,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		TEN,
		JACK,
		QUEEN,
		KING,
		NUM_VALUES
	};

	Card( Card::Value val, Card::Suit suit);
	Card();
	~Card(){};

	inline Card::Suit getSuit() const { return mSuit; };
	inline Card::Value getValue() const { return mValue; };
	UINT getPointValue() const;
	const std::string& getSuitName() const;
	const std::string& getValueName() const;

	static bool sortOrderSuitAndValue( const Card& lhs, const Card& rhs );
	static bool sortOrderValue(  const Card& lhs, const Card& rhs ) { return lhs.mValue < rhs.mValue; };

	inline bool operator==( const Card& rhs ) const { return (mSuit == rhs.mSuit && mValue == rhs.mValue); };

	bool isValid() const;

private:
	Card::Suit mSuit;
	Card::Value mValue;
};

std::ostream& operator<<( std::ostream& out, Card theCard );
