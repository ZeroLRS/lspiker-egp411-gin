#include "Event.h"
#include <functional>//non-standard for c++98, but is for c++11
#include <sstream>

Event::Event(EventType type, UINT playerNum, bool isNotification )
:BaseEvent( (BaseEventType)type )
,mKey( type, playerNum )
,mIsNotification( isNotification )
{
}

Event::Event()
	:Event(NUM_EVENT_TYPES, UINT_MAX, true)
{
}

Event::~Event()
{
}

const string& Event::getEventName() const
{
	//static const string nullString;
	//return nullString;

	if (mType > 0 && mType < NUM_EVENT_TYPES)
	{
		return EVENT_NAMES[mType];
	}
	else
	{
		static const string nullString;
		return nullString;
	}
}

bool operator<(const EventKey& lhs, const EventKey& rhs ) 
{ 
	return (lhs.getHashVal() < rhs.getHashVal() ); 
}

EventKey::EventKey( EventType theType, UINT playerNum )
	:mType(theType)
	,mTargetPlayerNum(playerNum)
{
	setHashVal();
}

EventKey::EventKey()
	:mType((EventType)BaseEventType::INVALID_EVENT_TYPE)
	,mTargetPlayerNum(UINT_MAX)
{
	setHashVal();
}

void EventKey::setHashVal()
{
	std::stringstream theStream;
	theStream << mType << " " << mTargetPlayerNum;
	std::string theString = theStream.str();
	std::hash<std::string> hashFunc;
	mHashVal = hashFunc( theStream.str() );
}
