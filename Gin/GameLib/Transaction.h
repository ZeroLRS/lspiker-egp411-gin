#pragma once

#include <wtypes.h>
#include "Trackable.h"

enum TransactionType
{
	INVALID_TRANSACTION_TYPE = -1,
	DRAW_TRANSACTION,
	DISCARD_TRANSACTION,
	PASS_CARD_TRANSACTION,
	END_GAME_TRANSACTION
};

class Transaction: public Trackable
{
public:
	Transaction();
	Transaction( TransactionType type );
	virtual ~Transaction();

	TransactionType getType() const { return mType; };

private:
	TransactionType mType;
	int mID;

	static int msNextID;
};

