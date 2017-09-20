#include <conio.h>
#include "Game.h"
#include "AIHolder.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"
#include "TaskCollection.h"
#include "WaitTask.h"
#include "FireEventTask.h"
#include "WaitAndFireEventTask.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsColor.h"
#include "GraphicsFont.h"
#include "GraphicsSprite.h"
#include "GraphicsGrid.h"
#include "CardSpriteManager.h"
#include "GameDisplay.h"
#include "InputSystem.h"
#include "DataRepository.h"
#include "StatTracker.h"
#include "GameSequencer.h"
#include "CardLocator.h"

#include "..\GameLib\EventSystem.h"
#include "..\GameLib\Event.h"
#include "..\GameLib\MouseEvent.h"
#include "..\GameLib\CardPlayedEvent.h"
#include "..\GameLib\CardDrawnEvent.h"
#include "..\GameLib\CardPassedEvent.h"
#include "..\GameLib\GameViewBaseEvent.h"
#include "..\GameLib\GameState.h"
#include "..\GameLib\GameView.h"
#include "..\GameLib\TransactionHandler.h"
#include "..\GameLib\Transaction.h"
#include "..\GameLib\DiscardCardTransaction.h"
#include "..\GameLib\Timer.h"

#include <assert.h>
#include <string>
#include <fstream>

using namespace std;

Game* gpGame = NULL;

Game::Game()
	:mpState(NULL)
	,mpTasks(NULL)
	,mpInputSystem(NULL)
	,mpSequencer(NULL)
	,mNumPlayers(0)
	,mNumAIPlayers(0)
	,mStartingPlayer(0)
	,mInitialized(false)
	,mKeepRunning(true)
{
	UINT numPlayers = gpDataRepository->getEntry(DataKeyEnum::NUM_PLAYERS).getUIntVal();
	mNumPlayers = numPlayers;
	mNumAIPlayers = numPlayers;

	for (UINT i = 0; i < numPlayers; i++)
	{
		mpPlayers[i] = NULL;
	}
}

Game::~Game()
{
	cleanup();
}

void Game::init()
{
	assert( mInitialized == false );
	mpState = new GameState;
	TransactionHandler::createTransactionHandler( GameState::handleTransaction, mpState );

	EventSystem::createEventSystem();
	registerCallbacks();

	loadAIHolders( gpDataRepository->getEntry(DataKeyEnum::DLL_FILENAME).getStringVal() );
	for( UINT i=0; i<mNumAIPlayers; i++ )
	{
		mpPlayers[i] = new AIPlayer( mpAIHolders[i], i );
	}
	/*if( mNumAIPlayers < mNumPlayers )
	{
		mpPlayers[1] = new HumanPlayer( "Dean", 1, TransactionHandler::getHandler() );
	}*/

	mpSequencer = new GameSequencer;
	//mpSequencer->setNextGameParams();

	UINT cardsPerPlayer = gpDataRepository->getEntry(DataKeyEnum::CARDS_PER_PLAYER).getUIntVal();
	UINT knockThreshold = gpDataRepository->getEntry(DataKeyEnum::KNOCK_THRESHOLD).getUIntVal();

	mpState->init(mNumPlayers,cardsPerPlayer,knockThreshold);
	mInitialized = true;
	mKeepRunning = true;

	mpTasks = new TaskCollection;

	UINT screenWidth = gpDataRepository->getEntry(DataKeyEnum::SCREEN_WIDTH).getUIntVal();
	UINT screenHeight = gpDataRepository->getEntry(DataKeyEnum::SCREEN_HEIGHT).getUIntVal();

	GraphicsSystem::createGraphicsSystem();
	GraphicsSystem::getGraphicsSystem()->init( screenWidth, screenHeight, false );

	mpInputSystem = new InputSystem;
	mpInputSystem->init();

	mStartingPlayer = 0;
}

void Game::cleanup()
{
	mpInputSystem->cleanup();
	delete mpInputSystem;
	mpInputSystem = NULL;

	GraphicsSystem::deleteGraphicsSystem();

	delete mpTasks;
	mpTasks = NULL;

	unregisterCallbacks();
	mInitialized = false;
	mKeepRunning = false;
	TransactionHandler::destroyTransactionHandler();

	delete mpSequencer;
	mpSequencer = NULL;

	for( size_t i=0; i<mpAIHolders.size(); i++ )
	{
		delete mpAIHolders[i];
	}
	mpAIHolders.clear();

	for( UINT i=0; i<mNumPlayers; i++ )
	{
		delete mpPlayers[i];
		mpPlayers[i] = NULL;
	}

	delete mpState;
	mpState = NULL;

	EventSystem::destroyEventSystem();
}

void Game::doLoop()
{
	//setNextPlayerActive();
	mpState->mResetReason = GameState::STARTING;
	gpGame->mpTasks->addTask(new FireEventTask(INIT_EVENT));
	gpGame->mpTasks->addTask(new FireEventTask(GAME_RESET_EVENT));
	gpGame->mpTasks->addTask(new WaitAndFireEventTask(gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), PASS_CARD_REQUEST));
	//gpGame->mpTasks->addTask(new WaitAndFireEventTask(gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), DRAW_CARD_REQUEST));


	Timer loopTimer;

	while(mKeepRunning)
	{
		// Poll a dummy event to allow us to move the window.
		SDL_Event dummyEvent;
		while (SDL_PollEvent(&dummyEvent)) {}

		loopTimer.start();
	
		TransactionHandler::getHandler()->processAllTransactions();
	
		for( UINT i=0; i<mNumPlayers; i++ )
		{
			mpPlayers[i]->update();
		}
	
		mpTasks->update();
		mpInputSystem->update();
	
		GraphicsSystem::clearBackBuffer( COLOR_GREEN );
	
		GameDisplay::draw( mpState );
	
		GraphicsSystem::flipDisplay();
	
	
		//cout << loopTimer.getElapsedTime() << endl;
		double target = gpDataRepository->getEntry(DataKeyEnum::TARGET_FRAME_TIME).getDoubleVal();
		loopTimer.sleepUntilElapsed( target );
		//cout << loopTimer.getElapsedTime() << endl;
	
	}
}

void Game::processTransactions()
{
	TransactionHandler::getHandler()->processAllTransactions();
}

void Game::loadAIHolders(const string& controllerFileName)
{
	ifstream controllerFile(controllerFileName);
	if( controllerFile.good() )
	{
		UINT playerCount = 0;
		while( !controllerFile.eof() && playerCount < mNumAIPlayers  )
		{
			playerCount++;
			string playerName;
			getline( controllerFile, playerName );
			string dllPath;
			getline( controllerFile, dllPath );

			AIHolder* pHolder = new AIHolder( playerName, dllPath, mpAIHolders.size() );
			mpAIHolders.push_back( pHolder );
		}
	}
}

AIHolder* Game::getAIHolder(UINT index) const
{
	assert( index < mpAIHolders.size() );

	if( index >= mpAIHolders.size() )
		return NULL;

	return mpAIHolders[index];
}

void Game::registerCallbacks()
{
	EventSystem::getEventSystem()->registerCallback( CARD_DRAWN_EVENT, cardDrawnCallback );
	EventSystem::getEventSystem()->registerCallback( CARD_PLAYED_EVENT, cardPlayedCallback);
	EventSystem::getEventSystem()->registerCallback( CARD_PASSED_EVENT, cardPassedCallback );
	EventSystem::getEventSystem()->registerCallback( GAME_ENDED_EVENT, gameEndedCallback );
	EventSystem::getEventSystem()->registerCallback( GAME_QUIT_EVENT, gameQuitCallback );
	EventSystem::getEventSystem()->registerCallback( MOUSE_CLICKED_EVENT, mouseClickedCallback );
	EventSystem::getEventSystem()->registerCallback( GAME_RESET_EVENT, gameResetCallback );
	EventSystem::getEventSystem()->registerCallback( KNOCK_EVENT, knockCallback );
}

void Game::unregisterCallbacks()
{
	EventSystem::getEventSystem()->unregisterCallback( CARD_DRAWN_EVENT, cardDrawnCallback );
	EventSystem::getEventSystem()->unregisterCallback( CARD_PLAYED_EVENT, cardPlayedCallback );
	EventSystem::getEventSystem()->unregisterCallback( CARD_PASSED_EVENT, cardPassedCallback );
	EventSystem::getEventSystem()->unregisterCallback( GAME_ENDED_EVENT, gameEndedCallback );
	EventSystem::getEventSystem()->unregisterCallback( GAME_QUIT_EVENT, gameQuitCallback );
	EventSystem::getEventSystem()->unregisterCallback( MOUSE_CLICKED_EVENT, mouseClickedCallback );
	EventSystem::getEventSystem()->unregisterCallback( GAME_RESET_EVENT, gameResetCallback );
	EventSystem::getEventSystem()->unregisterCallback( KNOCK_EVENT, knockCallback );
}

void Game::gameQuitCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	cout << "You Quitter!\n";
	gpGame->mKeepRunning = false;
}

void Game::mouseClickedCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	const MouseEvent& mouseEvent = static_cast<const MouseEvent&>(theEvent);
	cout << "Mouse clicked " << mouseEvent.getX() << " " << mouseEvent.getY() << endl;

	if( gpGame->getGameState()->getPhase() == GameState::DISCARD )
	{
		Card clickedCard = CardLocator::getCard( mouseEvent.getX(),  mouseEvent.getY(), gpGame->getGameState()->getCurrentPlayerNum() );
		cout << "Card clicked=" << clickedCard << endl;
		if( clickedCard.getSuit() < Card::NUM_SUITS )
		{
			bool knocking = false;
			if( mouseEvent.getButton() == RIGHT_MOUSE_BUTTON )
			{
				knocking = true;
			}
			pHandler->postTransaction( new DiscardCardTransaction( clickedCard, gpGame->getGameState()->getCurrentPlayerNum(), knocking ) );
		}
	}
	else if( gpGame->getGameState()->getPhase() == GameState::DRAW )
	{
		DrawCardTransaction::DrawType drawType = CardLocator::getDrawType( mouseEvent.getX(),  mouseEvent.getY() );
		cout << "pile clicked=" << drawType << endl;
		if( drawType < DrawCardTransaction::NUM_TYPES )
		{
			pHandler->postTransaction( new DrawCardTransaction( drawType, gpGame->getGameState()->getCurrentPlayerNum() ) );
		}
	}
}

const UINT WON = 0;
const UINT LOST = 1;
const UINT SCORE = 2;

const UINT GIN_BONUS = 25;
const UINT UNDERCUT_BONUS = 25;

void Game::gameResetCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	gpGame->mpTasks->markAllTasksDone();
	TransactionHandler::getHandler()->deleteAllTransactions();
	cout << "resetting new level...\n";

	//temp for speed
	GameState* pGameState = gpGame->getGameState();

	if (pGameState->getResetReason() == GameState::KNOCK || pGameState->getResetReason() == GameState::GIN)
	{
		//track stats
		gpStatTracker->addToStat(StatEnum::GAMES_SIMULATED, 1);
		UINT winner = pGameState->getLowestScoringPlayer();
		UINT knocker = pGameState->getCurrentPlayerNum();

		//check for bonuses
		//gin?
		if (pGameState->getResetReason() == GameState::GIN)
		{
			assert(winner == knocker);
			gpStatTracker->addToStat(PLAYER_CATAGORIES[knocker][SCORE], GIN_BONUS);
		}
		else if (pGameState->getResetReason() == GameState::KNOCK) 
		{
			//undercut?
			if (winner != knocker)
			{
				gpStatTracker->addToStat(PLAYER_CATAGORIES[winner][SCORE], UNDERCUT_BONUS);
			}
		}

		//mark winner and losers
		for (UINT i = 0; i < pGameState->getNumHands(); i++)
		{
			if (i == winner) //winner
			{
				gpStatTracker->addToStat(PLAYER_CATAGORIES[i][WON], 1);
				gpStatTracker->addToStat(PLAYER_CATAGORIES[i][SCORE], pGameState->getWinningPlayerScore());
			}
			else //loser
			{
				gpStatTracker->addToStat(PLAYER_CATAGORIES[i][LOST], 1);
			}
			//move scores into player object for display
			gpGame->getPlayer(i)->setTotalScore(gpStatTracker->getStat(PLAYER_CATAGORIES[i][SCORE]));
		}
	}

	bool shouldContinue = true;
	if (pGameState->getResetReason() != GameState::STARTING)
	{
		shouldContinue = gpGame->mpSequencer->setNextGameParams();
	}

	if( shouldContinue )
	{
		//EventSystem::getEventSystem()->fireEvent(Event(GAME_RESET_EVENT, UINT_MAX));
		pGameState->reset();
		if (pGameState->getResetReason() != GameState::STARTING)
		{
			gpGame->advanceStartingPlayer();
		}
		
		//queue up next task
		gpGame->mpTasks->addTask(new WaitAndFireEventTask(gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), PASS_CARD_REQUEST));
		//gpGame->mpTasks->addTask(new WaitAndFireEventTask(gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), DRAW_CARD_REQUEST));
		
	}
	else
	{
		EventSystem::getEventSystem()->fireEvent(Event(CLEANUP_EVENT, UINT_MAX, true));
		EventSystem::getEventSystem()->fireEvent(Event(GAME_QUIT_EVENT, UINT_MAX));
	}
}

void Game::setNextPlayerActive()
{
	mpState->advancePlayerTurn();
	UINT currentPlayerNum = mpState->getCurrentPlayerNum();

	//set all players inactive except for the current player
	for (UINT i = 0; i < mNumPlayers; i++)
	{
		if (currentPlayerNum == i)
		{
			mpPlayers[i]->setActive(true);
		}
		else
		{
			mpPlayers[i]->setActive(false);
		}
	}
}

void Game::cardDrawnCallback( const Event& theEvent, TransactionHandler* pHandler )
{
	if( theEvent.getType() == CARD_DRAWN_EVENT )
	{
		//cout << "card drawn callback\n";
		const CardDrawnEvent& drawnEvent = static_cast<const CardDrawnEvent&>(theEvent);
		//queue up next task
		gpGame->mpTasks->addTask( new WaitAndFireEventTask( gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), DISCARD_CARD_REQUEST) );
	}

}

void Game::cardPlayedCallback( const Event& theEvent, TransactionHandler* pHandler )
{
	if( theEvent.getType() == CARD_PLAYED_EVENT && gpGame->getGameState()->getPhase() == GameState::DRAW )
	{
		//cout << "card played callback\n";
		const CardPlayedEvent& playedEvent = static_cast<const CardPlayedEvent&>(theEvent);
		gpGame->setNextPlayerActive();
		//queue up next task
		gpGame->mpTasks->addTask( new WaitAndFireEventTask( gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), DRAW_CARD_REQUEST) );
	}

	//track stats
	//gpStatTracker->addToStat(StatEnum::GAMES_LOST, 1 );

}

void Game::cardPassedCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	if (theEvent.getType() == CARD_PASSED_EVENT)
	{
		const CardPassedEvent& passedEvent = static_cast<const CardPassedEvent&>(theEvent);
		gpGame->setNextPlayerActive();
		if (gpGame->getGameState()->getPhase() == GameState::DRAW)
		{
			gpGame->mpTasks->addTask(new WaitAndFireEventTask(gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), DRAW_CARD_REQUEST));
		}
		else if (gpGame->getGameState()->getPhase() == GameState::PASS)
		{
			gpGame->mpTasks->addTask(new WaitAndFireEventTask(gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), PASS_CARD_REQUEST));
		}
	}
}

void Game::gameEndedCallback( const Event& theEvent, TransactionHandler* pHandler )
{
	if (theEvent.getType() == GAME_ENDED_EVENT)
	{
		//cout << "Game Ended callback\n";
		gpGame->mpTasks->markAllTasksDone();

		gpGame->mpTasks->addTask(new WaitAndFireEventTask(gpDataRepository->getEntry(DataKeyEnum::GAME_RESET_PAUSE).getUIntVal(), GAME_RESET_EVENT));
	}

}

void Game::knockCallback( const Event& theEvent, TransactionHandler* pHandler )
{
	if( theEvent.getType() == KNOCK_EVENT )
	{
		//cout << "Knock Knock!!! callback\n";
		gpGame->mpTasks->markAllTasksDone();
		gpGame->mpState->mResetReason = GameState::KNOCK;
		if (gpGame->getGameState()->getHand(gpGame->getCurrentPlayer()->getPlayerNum())->getHandScore() == 0)//gin?
		{
			gpGame->mpState->mResetReason = GameState::GIN;
		}
		gpGame->mpTasks->addTask(new WaitAndFireEventTask(gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), GAME_ENDED_EVENT));

	}
}

Player* Game::getCurrentPlayer() const
{ 
	return mpPlayers[mpState->getCurrentPlayerNum()]; 
}

Player* Game::getPlayer( UINT playerNum ) const
{ 
	return mpPlayers[playerNum]; 
}

bool Game::humanPlaying() const
{
	return ( mNumAIPlayers < mNumPlayers);
}

void Game::advanceStartingPlayer()
{
	mStartingPlayer = (mStartingPlayer + 1) % mNumPlayers;
	gpGame->getGameState()->setCurrentPlayerTurn(mStartingPlayer);
}
