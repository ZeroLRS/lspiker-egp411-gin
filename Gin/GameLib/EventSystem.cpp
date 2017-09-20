#include "EventSystem.h"
#include "Event.h"
#include "TransactionHandler.h"

EventSystem* EventSystem::mspInstance = NULL;

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::registerCallback(EventType type, EventCallbackFunction callbackFunction, UINT playerNum)
{
	EventKey theKey(type,playerNum);
	size_t hashVal = theKey.getHashVal();
	mCallbacks.insert( pair< size_t, EventCallbackFunction >(hashVal, callbackFunction) );
}

void EventSystem::unregisterCallback(EventType type, EventCallbackFunction callbackFunction, UINT playerNum)
{
	pair<multimap<size_t,EventCallbackFunction>::iterator,multimap<size_t,EventCallbackFunction>::iterator> ret;

	ret = mCallbacks.equal_range( EventKey(type, playerNum ).getHashVal() );
	multimap<size_t,EventCallbackFunction>::iterator iter;

	for( iter = ret.first; iter != ret.second; ++iter )
	{
		if( iter->second == callbackFunction )
		{
			mCallbacks.erase( iter );
			break;//to prevent using invalidated iterator
		}
	}
}

void EventSystem::fireEvent( const Event& theEvent )
{
	//cout << "Event Fired:" << theEvent.getEventName() << endl;
	dispatchAllEvents( static_cast<const Event&>(theEvent) );
}

void EventSystem::dispatchAllEvents( const Event& theEvent )
{
	TransactionHandler* pHandler = NULL;
	if( !theEvent.isNotification() )
	{
		pHandler = TransactionHandler::getHandler();
	}

	pair<multimap<size_t,EventCallbackFunction>::iterator,multimap<size_t,EventCallbackFunction>::iterator> ret;
	ret = mCallbacks.equal_range( theEvent.getEventKey().getHashVal() );

	multimap<size_t,EventCallbackFunction>::iterator iter;
	for( iter = ret.first; iter != ret.second; ++iter )
	{
		EventCallbackFunction theFunction = iter->second;
		theFunction( theEvent, pHandler );//call the callback function!
	}

}