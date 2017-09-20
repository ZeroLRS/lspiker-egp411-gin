#pragma once

#include "transaction.h"
#include "Card.h"

class DiscardCardTransaction : public Transaction
{
public:
	DiscardCardTransaction( const Card& card, UINT playerNum, bool knock = false );
	~DiscardCardTransaction(){};

	inline Card getCard() const { return mCard; };
	inline bool isKnocking() const { return mKnocking; };
	inline UINT getPlayerNum() const { return mPlayerNum; };

private:
	Card mCard;
	bool mKnocking;
	UINT mPlayerNum;
};

