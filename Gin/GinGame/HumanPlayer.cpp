#include "HumanPlayer.h"
#include "Game.h"
#include "..\GameLib\GameState.h"
#include "..\GameLib\TransactionHandler.h"
#include <iostream>
#include <wtypes.h>

using namespace std;

HumanPlayer::HumanPlayer( const std::string& name, UINT playerNum, TransactionHandler* pHandler )
	:Player(name, playerNum)
	,mpHandler(pHandler)
{
}

HumanPlayer::~HumanPlayer()
{
}

void HumanPlayer::update()
{
}
