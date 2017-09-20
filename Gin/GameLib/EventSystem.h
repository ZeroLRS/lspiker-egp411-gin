#pragma once

#include <map>
#include <cassert>
#include <wtypes.h>
#include "Trackable.h"

class Event;
class TransactionHandler;
class EventKey;
enum EventType;

typedef void (__cdecl *EventCallbackFunction)( const Event&, TransactionHandler* );


class EventSystem:public Trackable
{
public:
	void fireEvent( const Event& theEvent );
	void registerCallback( EventType type, EventCallbackFunction callbackFunction, UINT playerNum = UINT_MAX );
	void unregisterCallback( EventType type, EventCallbackFunction callbackFunction, UINT playerNum = UINT_MAX );

	static inline EventSystem* getEventSystem() { return mspInstance; };
	static inline void createEventSystem(){ assert( mspInstance == NULL ); mspInstance = new EventSystem; };
	static inline void destroyEventSystem(){ delete mspInstance; mspInstance = NULL; };
private:
	static EventSystem* mspInstance;

	std::multimap< size_t, EventCallbackFunction > mCallbacks;

	void dispatchAllEvents( const Event& theEvent );
	EventSystem();
	~EventSystem();

};

