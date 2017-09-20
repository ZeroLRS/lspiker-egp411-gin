#pragma once

#include <string>
#include <map>
#include <wtypes.h>
#include "..\GameLib\Trackable.h"
#include "..\GameLib\EventSystem.h"

class GameState;
class TransactionHandler;

typedef std::string FuncID;

const FuncID DRAW_CARD = "drawCardRequest";
const FuncID DISCARD_CARD = "discardCardRequest";
const FuncID PASS_CARD = "passCardRequest";
const FuncID CARD_DRAWN = "cardDrawnNotification";
const FuncID CARD_PLAYED = "cardPlayedNotification";
const FuncID CARD_PASSED = "cardPassedNotification";
const FuncID GENERAL_NOTIFICATION = "generalGameNotification";

class AIHolder: public Trackable
{
public:
	AIHolder( const std::string& playerName, const std::string& dllPath, UINT playerNum );
	~AIHolder();

	inline const std::string& getName() const { return mName; };

	void setActive( bool flag );
	bool isActive() const;
	void setFunction( const FuncID& id, EventCallbackFunction function );
	EventCallbackFunction getFunction( const FuncID& id ) const;

private:
	std::map<FuncID,EventCallbackFunction> mFunctions;
	std::string mName;
	HINSTANCE mDllHandle;
	bool mActive;
	UINT mPlayerNum;

};