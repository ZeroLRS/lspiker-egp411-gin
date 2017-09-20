#pragma once

#include "..\gamelib\trackable.h"
#include <wtypes.h>

class GameSequencer : public Trackable
{
public:
	GameSequencer();
	~GameSequencer(){};

	bool setNextGameParams();//returns false when simulation should end

private:

	UINT mNumStillToPlay;
};

