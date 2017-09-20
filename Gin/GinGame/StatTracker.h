#pragma once

#include "..\gamelib\trackable.h"
#include <string>
#include <map>
#include <wtypes.h>
#include <fstream>

//class only exists to hold a class level enum

const UINT STAT_INCREMENT = 3;

class StatEnum
{
public:
	enum StatCatagory
	{
		GAMES_SIMULATED,
		GAMES_WON_PLAYER_0,
		GAMES_LOST_PLAYER_0,
		TOTAL_SCORE_PLAYER_0,
		GAMES_WON_PLAYER_1 = GAMES_WON_PLAYER_0 + STAT_INCREMENT,
		GAMES_LOST_PLAYER_1 = GAMES_LOST_PLAYER_0 + STAT_INCREMENT,
		TOTAL_SCORE_PLAYER_1 = TOTAL_SCORE_PLAYER_0 + STAT_INCREMENT,
		GAMES_WON_PLAYER_2 = GAMES_WON_PLAYER_1 + STAT_INCREMENT,
		GAMES_LOST_PLAYER_2 = GAMES_LOST_PLAYER_1 + STAT_INCREMENT,
		TOTAL_SCORE_PLAYER_2 = TOTAL_SCORE_PLAYER_1 + STAT_INCREMENT,
		GAMES_WON_PLAYER_3 = GAMES_WON_PLAYER_2 + STAT_INCREMENT,
		GAMES_LOST_PLAYER_3 = GAMES_LOST_PLAYER_2 + STAT_INCREMENT,
		TOTAL_SCORE_PLAYER_3 = TOTAL_SCORE_PLAYER_2 + STAT_INCREMENT,
		NUM_STAT_CATAGORIES
	};


};

const StatEnum::StatCatagory PLAYER_CATAGORIES[4][3] =
{
	{ StatEnum::GAMES_WON_PLAYER_0, StatEnum::GAMES_LOST_PLAYER_0, StatEnum::TOTAL_SCORE_PLAYER_0 },
	{ StatEnum::GAMES_WON_PLAYER_1, StatEnum::GAMES_LOST_PLAYER_1, StatEnum::TOTAL_SCORE_PLAYER_1 },
	{ StatEnum::GAMES_WON_PLAYER_2, StatEnum::GAMES_LOST_PLAYER_2, StatEnum::TOTAL_SCORE_PLAYER_2 },
	{ StatEnum::GAMES_WON_PLAYER_3, StatEnum::GAMES_LOST_PLAYER_3, StatEnum::TOTAL_SCORE_PLAYER_3 }
};


const std::string STAT_DESCRIPTIONS[StatEnum::NUM_STAT_CATAGORIES] = 
{
	"Number of Games Simulated",
	"Number of Games Won Player 0",
	"Number of Games Lost Player 0",
	"Total score Player 0",
	"Number of Games Won Player 1",
	"Number of Games Lost Player 1",
	"Total score Player 1",
	"Number of Games Won Player 2",
	"Number of Games Lost Player 2",
	"Total score Player 2",
	"Number of Games Won Player 3",
	"Number of Games Lost Player 3",
	"Total score Player 3",
};



class StatTracker :	public Trackable
{
public:
	StatTracker( const std::string& filename );
	~StatTracker();
	void addToStat( StatEnum::StatCatagory catagory, int addition );
	void resetStat( StatEnum::StatCatagory catagory);
	void reportStats( std::ostream& theStream ) const ;
	int getStat(StatEnum::StatCatagory catagory) const;

private:
	std::string mFilename;
	std::map<StatEnum::StatCatagory,int> mMap;
};

extern StatTracker* gpStatTracker;