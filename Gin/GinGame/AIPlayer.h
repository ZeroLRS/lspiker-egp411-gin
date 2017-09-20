#pragma once

#include "../GameLib/Player.h"

class AIHolder;

class AIPlayer : public Player
{
public:
	AIPlayer( AIHolder* pHolder, UINT playerNum );
	virtual ~AIPlayer();

	void update(){};

	void setActive( bool isActive );
	bool isActive() const;
private:
	AIHolder* mpHolder;
};

