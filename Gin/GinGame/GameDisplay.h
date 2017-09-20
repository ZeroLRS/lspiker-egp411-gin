#pragma once

#include "..\gamelib\trackable.h"

class GameState;

class GameDisplay :	public Trackable
{
public:

	static void draw( GameState* pGameState );

private:
	GameDisplay();
	~GameDisplay();

};

