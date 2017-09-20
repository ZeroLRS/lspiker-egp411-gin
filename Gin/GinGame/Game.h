#pragma once

#include <string>
#include <vector>
#include <wtypes.h>
#include "..\GameLib\Trackable.h"
#include "..\GameLib\GameState.h"
#include "TaskCollection.h"
#include "GraphicsFont.h"

class TransactionHandler;
class Transaction;
class AIHolder;
class GraphicsGrid;
class Player;
class Event;
class InputSystem;
class GameSequencer;

class Game: public Trackable
{
public:
	Game();
	~Game();

	void init();
	void cleanup();

	void doLoop();

	inline GameState* getGameState() const { return mpState; };
	
	void processTransactions();

	AIHolder* getAIHolder( UINT index ) const;
	inline size_t getNumAIHolders() const { return mpAIHolders.size(); };
	Player* getCurrentPlayer() const;
	Player* getPlayer(UINT playerNum) const;
	inline FontID getFontID() const { return mFontID; };
	inline void setFontID(const FontID& id) { mFontID = id; };
	bool humanPlaying() const;
	UINT getNumPlayers() const { return mNumPlayers; };
	void removeAllTasks() { mpTasks->markAllTasksDone(); };
	void advanceStartingPlayer();

private:
	GameState* mpState;
	std::vector<AIHolder*> mpAIHolders;
	Player* mpPlayers[MAX_PLAYERS];
	TaskCollection* mpTasks;
	InputSystem* mpInputSystem;
	GameSequencer* mpSequencer;
	UINT mNumPlayers;
	UINT mNumAIPlayers;
	UINT mStartingPlayer;
	FontID mFontID;
	bool mInitialized;
	bool mKeepRunning;

	void loadAIHolders( const std::string& controllerFileName );
	void loadPlayers();
	static void cardDrawnCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void cardPlayedCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void cardPassedCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void gameEndedCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void knockCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void gameQuitCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void mouseClickedCallback(const Event& theEvent, TransactionHandler* pHandler);
	static void gameResetCallback(const Event& theEvent, TransactionHandler* pHandler);

	void registerCallbacks();
	void unregisterCallbacks();

	void setNextPlayerActive();
};

extern Game* gpGame;