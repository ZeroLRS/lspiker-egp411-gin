#include "AIPlayer.h"
#include "AIHolder.h"
#include "Game.h"
#include <string>
#include <assert.h>

using namespace std;

AIPlayer::AIPlayer(AIHolder* pHolder, UINT playerNum)
	:Player("", playerNum)
	,mpHolder(pHolder)
{
	assert( pHolder != NULL );
	if( pHolder != NULL )
	{
		mName = pHolder->getName();
	}
}


AIPlayer::~AIPlayer()
{
}

void AIPlayer::setActive(bool isActive)
{
	mpHolder->setActive( isActive );
}

bool AIPlayer::isActive() const
{
	return mpHolder->isActive();
}
