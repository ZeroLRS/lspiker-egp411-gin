#include <iostream>
#include <thread>
#include <wtypes.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "Game.h"
#include "AIHolder.h"
#include "DataRepository.h"
#include "DataLoader.h"
#include "StatTracker.h"
#include "..\GameLib\GameState.h"
#include "..\GameLib\Transaction.h"
#include "..\GameLib\TransactionHandler.h"
#include "..\GameLib\MemoryTracker.h"
#include "..\GinDLL\DLLCode.h"

using namespace std;

int main(int argc, char **argv)
{
	gpDataRepository = new DataRepository;
	srand( (UINT)time(NULL) );
	DataLoader loader( "driver.txt" );

	gpStatTracker = new StatTracker( "stats.txt" );

	gpGame = new Game;
	gpGame->init();

	gpGame->doLoop();

	//gpGame->processTransactions();

	system("pause");
	delete gpGame;

	delete gpStatTracker;
	delete gpDataRepository;
	gMemoryTracker.reportAllocations( cout );
	system("pause");

	return 0;
}
