#include "GameSequencer.h"
#include "DataRepository.h"


GameSequencer::GameSequencer()
{
	mNumStillToPlay = gpDataRepository->getEntry(DataKeyEnum::NUM_GAMES).getUIntVal();
}

bool GameSequencer::setNextGameParams()
{
	mNumStillToPlay--;

	if (mNumStillToPlay == 0)
	{
		return false;
	}

	return true;
}

