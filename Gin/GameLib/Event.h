#pragma once

#include <string>
#include <wtypes.h>
#include "../GameLib/BaseEvent.h"

using namespace std;

enum EventType
{
	DRAW_CARD_REQUEST = BaseEventType::INVALID_EVENT_TYPE + 1,
	DISCARD_CARD_REQUEST,
	PASS_CARD_REQUEST,
	CARD_PLAYED_EVENT,
	CARD_DRAWN_EVENT,
	CARD_PASSED_EVENT,
	KNOCK_EVENT,
	GAME_ENDED_EVENT,
	GAME_QUIT_EVENT,
	GAME_RESET_EVENT,
	INIT_EVENT,
	CLEANUP_EVENT,
	MOUSE_CLICKED_EVENT,
	NUM_EVENT_TYPES
};

const string EVENT_NAMES[NUM_EVENT_TYPES] =	{	"",
												"Draw Card Request",
												"Discard Card Request",
												"Card Played Event",
												"Card Drawn Event",
												"Knock Event",
												"Game Ended Event",
												"Game Quit Event",
												"Game Reset Event",
												"Init Event",
												"Cleanup Event",
												"Mouse clicked Event"
											};

class EventKey: public Trackable
{
public:
	EventKey( EventType theType, UINT playerNum );
	EventKey();
	~EventKey(){};

	inline size_t getHashVal() const { return mHashVal; };

private:
	EventType mType;
	UINT mTargetPlayerNum;
	size_t mHashVal;

	void setHashVal();
};

bool operator<(const EventKey& lhs, const EventKey& rhs );

class Event:public BaseEvent
{
public:
	Event(EventType type, UINT playerNum, bool isNotification = false);
	Event();
	virtual ~Event();

	inline EventType getType() const { return (EventType)getEventType(); };
	const string& getEventName() const;
	inline EventKey getEventKey() const { return mKey; };
	inline bool isNotification() const { return mIsNotification; };
private:
	EventKey mKey;
	bool mIsNotification;
};