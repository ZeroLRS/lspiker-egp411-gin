#pragma once

#include <string>
#include "..\gamelib\player.h"
#include "..\GameLib\Event.h"

class TransactionHandler;

class HumanPlayer:public Player
{
public:
	HumanPlayer(const std::string& name, UINT playerNum, TransactionHandler* pHandler);
	virtual ~HumanPlayer();

	virtual void update();
	inline void setActive( bool isActive ) { mActive = isActive; };
	inline bool isActive() const { return mActive; };
	//static void drawCardRequestCallback( const Event&, TransactionHandler* );

private:
	TransactionHandler* mpHandler;
	bool mActive;

};

