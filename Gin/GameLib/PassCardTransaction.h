#pragma once

#include "Transaction.h"
#include "Card.h"

class PassCardTransaction : public Transaction
{
public:
	PassCardTransaction( const Card& card, UINT playerNum );
	~PassCardTransaction() {};

	inline Card getCard() const { return mCard; };
	inline UINT getPlayerNum() const { return mPlayerNum; };

private:
	Card mCard;
	UINT mPlayerNum;


};