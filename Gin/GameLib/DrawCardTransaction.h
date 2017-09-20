#pragma once

#include "transaction.h"
#include <wtypes.h>

class DrawCardTransaction :	public Transaction
{
public:
	enum DrawType
	{
		DECK,
		DISCARD,
		NUM_TYPES
	};

	DrawCardTransaction( DrawType drawType, UINT playerNum );
	~DrawCardTransaction(){};

	inline DrawType getDrawType() const { return mDrawType; };
	inline UINT getPlayerNum() const { return mPlayerNum; };

private:
	DrawType mDrawType;
	UINT mPlayerNum;
};

