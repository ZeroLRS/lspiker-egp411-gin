#include "DataLoader.h"
#include "DataRepository.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <wtypes.h>
#include <cassert>

using namespace std;

DataLoader::DataLoader(const string& filename)
{
	ifstream input( filename );
	if( input.good() )
	{
		while( !input.eof() )
		{
			string line;
			getline( input, line);
			stringstream sstream( line );
			string id;
			sstream >> id;

			if( id == "screen_size" )
			{
				UINT width, height;
				sstream >> width >> height;
				gpDataRepository->addEntry( DataKeyEnum::SCREEN_WIDTH, width );
				gpDataRepository->addEntry( DataKeyEnum::SCREEN_HEIGHT, height );
			}
			else if (id == "knock_threshold")
			{
				UINT num;
				sstream >> num;
				gpDataRepository->addEntry(DataKeyEnum::KNOCK_THRESHOLD, num);
				assert(num <= 100);
			}
			else if (id == "cards_per_player")
			{
				UINT num;
				sstream >> num;
				gpDataRepository->addEntry(DataKeyEnum::CARDS_PER_PLAYER, num);
				assert(num <= 52);
			}
			else if (id == "num_players")
			{
				UINT num;
				sstream >> num;
				gpDataRepository->addEntry(DataKeyEnum::NUM_PLAYERS, num);
				assert(num <= MAX_PLAYERS);
			}
			else if (id == "num_games")
			{
				UINT num;
				sstream >> num;
				gpDataRepository->addEntry(DataKeyEnum::NUM_GAMES, num);
			}
			else if (id == "dll_filename")
			{
				string name;
				sstream >> name;
				gpDataRepository->addEntry( DataKeyEnum::DLL_FILENAME, name );
			}
			else if( id == "target_frame_time" )
			{
				double time;
				sstream >> time;
				gpDataRepository->addEntry( DataKeyEnum::TARGET_FRAME_TIME, time );
			}
			else if( id == "fire_event_pause" )
			{
				UINT time;
				sstream >> time;
				gpDataRepository->addEntry( DataKeyEnum::FIRE_EVENT_PAUSE, time );
			}
			else if( id == "game_reset_pause" )
			{
				UINT time;
				sstream >> time;
				gpDataRepository->addEntry( DataKeyEnum::GAME_RESET_PAUSE, time );
			}
		}
	}

}


