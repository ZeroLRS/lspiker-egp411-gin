#include "AIHolder.h"
#include "Game.h"
#include "..\GameLib\EventSystem.h"
#include "..\GameLib\Event.h"
#include <assert.h>
#include <thread>

using namespace std;


AIHolder::AIHolder( const std::string& playerName, const std::string& dllPath, UINT playerNum )
	:mName(playerName)
	,mActive(false)
	,mPlayerNum(playerNum)
{
	mDllHandle = LoadLibrary(TEXT(dllPath.c_str()));
	
	//hardwired for now!
	if( mDllHandle != NULL )
	{
		EventCallbackFunction address = (EventCallbackFunction)GetProcAddress(mDllHandle, DRAW_CARD.c_str());
		setFunction( DRAW_CARD, address );
		EventSystem::getEventSystem()->registerCallback(DRAW_CARD_REQUEST, address, playerNum);
		
		address = (EventCallbackFunction)GetProcAddress(mDllHandle, DISCARD_CARD.c_str());
		setFunction( DISCARD_CARD, address );
		EventSystem::getEventSystem()->registerCallback(DISCARD_CARD_REQUEST, address, playerNum);

		address = (EventCallbackFunction)GetProcAddress(mDllHandle, PASS_CARD.c_str());
		setFunction( PASS_CARD, address );
		EventSystem::getEventSystem()->registerCallback(PASS_CARD_REQUEST, address, playerNum);

		address = (EventCallbackFunction)GetProcAddress(mDllHandle, CARD_PLAYED.c_str());
		setFunction( CARD_PLAYED, address );
		EventSystem::getEventSystem()->registerCallback(CARD_PLAYED_EVENT, address, playerNum);

		address = (EventCallbackFunction)GetProcAddress(mDllHandle, CARD_DRAWN.c_str());
		setFunction(CARD_DRAWN, address);
		EventSystem::getEventSystem()->registerCallback(CARD_DRAWN_EVENT, address, playerNum);

		address = (EventCallbackFunction)GetProcAddress(mDllHandle, CARD_PASSED.c_str());
		setFunction(CARD_PASSED, address);
		EventSystem::getEventSystem()->registerCallback(CARD_PASSED_EVENT, address, playerNum);

		address = (EventCallbackFunction)GetProcAddress(mDllHandle, GENERAL_NOTIFICATION.c_str());
		setFunction(GENERAL_NOTIFICATION, address);
		EventSystem::getEventSystem()->registerCallback(GAME_RESET_EVENT, address, UINT_MAX);
		EventSystem::getEventSystem()->registerCallback(INIT_EVENT, address, UINT_MAX);
		EventSystem::getEventSystem()->registerCallback(CLEANUP_EVENT, address, UINT_MAX);
	}
	else
	{
		cout << "DLL file not found!!!!  " << dllPath << endl;
		assert(false);
	}
}

AIHolder::~AIHolder()
{
	EventSystem::getEventSystem()->unregisterCallback(DRAW_CARD_REQUEST, getFunction(DRAW_CARD), mPlayerNum);
	EventSystem::getEventSystem()->unregisterCallback(DISCARD_CARD_REQUEST, getFunction(DISCARD_CARD), mPlayerNum);
	EventSystem::getEventSystem()->unregisterCallback(CARD_PLAYED_EVENT, getFunction(CARD_PLAYED), mPlayerNum);
	EventSystem::getEventSystem()->unregisterCallback(CARD_DRAWN_EVENT, getFunction(CARD_DRAWN), mPlayerNum);
	EventSystem::getEventSystem()->unregisterCallback(GAME_RESET_EVENT, getFunction(GENERAL_NOTIFICATION), UINT_MAX);
	EventSystem::getEventSystem()->unregisterCallback(INIT_EVENT, getFunction(GENERAL_NOTIFICATION), UINT_MAX);
	EventSystem::getEventSystem()->unregisterCallback(CLEANUP_EVENT, getFunction(GENERAL_NOTIFICATION), UINT_MAX);
	FreeLibrary(mDllHandle);
}

void AIHolder::setActive( bool flag )
{
	if( flag && !mActive )//activating
	{
		mActive = true;
	}
	else if( !flag && mActive )//deactivating
	{
		mActive = true;
	}
}

bool AIHolder::isActive() const
{
	return mActive;
}

void AIHolder::setFunction( const FuncID& id, EventCallbackFunction function )
{
	mFunctions[id] = function;
}

EventCallbackFunction AIHolder::getFunction( const FuncID& id ) const
{
	std::map<FuncID,EventCallbackFunction>::const_iterator iter = mFunctions.find( id );
	if( iter == mFunctions.cend() )
	{
		return NULL;
	}

	return iter->second;
}

