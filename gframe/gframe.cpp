#include "config.h"
#include "game.h"
#include <event2/thread.h>
#include "data_manager.h"
#include "deck_manager.h"
#include "netserver.h"

#ifndef WIN32

#define Sleep(x) usleep(1000*x)
#endif
int enable_log = 0;
bool exit_on_return = false;

bool runasserver = true;
const unsigned short PRO_VERSION = 0x12f0;

namespace ygo{
unsigned short aServerPort;
unsigned int lflist;
unsigned char rule;
unsigned char mode;
bool enable_priority;
bool no_check_deck;
bool no_shuffle_deck;
unsigned int start_lp;
unsigned char start_hand;
unsigned char draw_count;
unsigned int game_timer;
}
int main(int argc, char* argv[]) {
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif //_WIN32
	//ygo::Game _game;

	if (runasserver){
	    ygo::aServerPort=7911;
		ygo::aServerPort=atoi(argv[1]);
		ygo::lflist=atoi(argv[2]);
		ygo::start_hand=0;
		if (argc>3) {
			ygo::rule=atoi(argv[3]);
			ygo::mode=atoi(argv[4]);
			if (argv[5][0]=='T')
				ygo::enable_priority=true;
			else
				ygo::enable_priority=false;
			if (argv[6][0]=='T')
				ygo::no_check_deck=true;
			else
				ygo::no_check_deck=false;
			if (argv[7][0]=='T')
				ygo::no_shuffle_deck=true;
			else
				ygo::no_shuffle_deck=false;
			ygo::start_lp=atoi(argv[8]);
			ygo::start_hand=atoi(argv[9]);
			ygo::draw_count=atoi(argv[10]);
			ygo::game_timer=atoi(argv[11]);
		}
		ygo::deckManager.LoadLFList();
    ygo::dataManager.LoadDB("cards.cdb");
    ygo::NetServer::StartServer(ygo::aServerPort);
    ygo::NetServer::Initduel(ygo::mode);
    while(ygo::NetServer::net_evbase)
    {
        Sleep(200);
    }
		return 0;
	}


	return EXIT_SUCCESS;
}
