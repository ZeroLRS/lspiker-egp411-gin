#pragma once
#include "..\gamelib\trackable.h"

#include <string>
#include <wtypes.h>
#include <cassert>
#include <map>

const UINT MAX_STRING_VAL_SIZE = 48;

union DataUnion
{
	int intVal;
	float floatVal;
	double doubleVal;
	char stringVal[MAX_STRING_VAL_SIZE];
	UINT uintVal;

};

enum DataType
{
	INT_VAL,
	FLOAT_VAL,
	DOUBLE_VAL,
	STRING_VAL,
	UINT_VAL
};

class DataEntry: public Trackable
{
public:
	DataEntry( int val );
	DataEntry( float val );
	DataEntry( double val );
	DataEntry( const std::string& val );
	DataEntry( UINT val );
	DataEntry():mType(UINT_VAL){};

	~DataEntry(){};

	inline int getIntVal() const { assert( mType == INT_VAL ); return mData.intVal; };
	inline float getFloatVal() const { assert( mType == FLOAT_VAL ); return mData.floatVal; };
	inline double getDoubleVal() const { assert( mType == DOUBLE_VAL ); return mData.doubleVal; };
	inline std::string getStringVal() const { assert( mType == STRING_VAL ); return std::string(mData.stringVal); };
	inline UINT getUIntVal() const { assert( mType == UINT_VAL ); return mData.uintVal; };
private:
	DataType mType;
	DataUnion mData;

};

typedef int DataKey;

class DataRepository : public Trackable
{

public:
	DataRepository(){};
	~DataRepository(){};

	void addEntry( const DataKey& key, int val );
	void addEntry( const DataKey& key, float val );
	void addEntry( const DataKey& key, double val );
	void addEntry( const DataKey& key, const std::string& val );
	void addEntry( const DataKey& key, UINT val );

	const DataEntry& getEntry( const DataKey& key );

private:
	std::map<DataKey, DataEntry> mMap;
};

extern DataRepository* gpDataRepository;

//class only exists to hold a class level enum
class DataKeyEnum
{
public:
	enum DataKeyVals
	{
		NUM_GAMES,
		NUM_PLAYERS,
		KNOCK_THRESHOLD,
		CARDS_PER_PLAYER,
		DLL_FILENAME,
		TARGET_FRAME_TIME,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		GAME_RESET_PAUSE,
		FIRE_EVENT_PAUSE
	};

};


