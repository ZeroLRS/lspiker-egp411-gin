#include "StatTracker.h"
#include <fstream>
#include <cassert>

using namespace std;

StatTracker* gpStatTracker = NULL;

StatTracker::StatTracker( const std::string& filename )
	:mFilename( filename )
{
	for( UINT i=0; i<StatEnum::NUM_STAT_CATAGORIES; i++ )
	{
		mMap[ (StatEnum::StatCatagory)i ] = 0;
	}
}

StatTracker::~StatTracker()
{
	ofstream output( mFilename );

	if( output.good() )
	{
		reportStats( output );
	}
	output.close();
}

void StatTracker::addToStat(StatEnum::StatCatagory catagory, int addition)
{
	int val = mMap[ catagory];
	val += addition;
	mMap[ catagory ] = val;
}

void StatTracker::resetStat(StatEnum::StatCatagory catagory)
{
	mMap[ catagory ] = 0;
}

void StatTracker::reportStats(std::ostream& theStream) const
{
	for( UINT i=0; i<StatEnum::NUM_STAT_CATAGORIES; i++ )
	{
		StatEnum::StatCatagory catagory =  (StatEnum::StatCatagory)i;
		theStream << STAT_DESCRIPTIONS[ i ] << " = " << mMap.find( catagory )->second << endl;
	}
}

int StatTracker::getStat(StatEnum::StatCatagory catagory) const
{
	auto it = mMap.find(catagory);
	assert(it != mMap.end());

	if (it != mMap.end())
	{
		return mMap.find(catagory)->second;
	}
	else
	{
		return 0;
	}
}

