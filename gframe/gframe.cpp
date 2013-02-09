#include "config.h"
#include "game.h"
#include <event2/thread.h>

int enable_log = 0;
bool exit_on_return = false;

bool runasserver = true;



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
	ygo::Game _game;

	if (runasserver){
	    ygo::aServerPort=7911;
		ygo::aServerPort=atoi(argv[1]);
		ygo::lflist=atoi(argv[2]);
		ygo::start_hand=0;
		if (argc>2) {
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
		}
		ygo::mainGame = &_game;
		ygo::mainGame->MainServerLoop(ygo::mode);
		
		return 0;
	}


	ygo::mainGame = &_game;
	if(!ygo::mainGame->Initialize())
		return 0;

	if(argc >= 2) {
		/*command line args:
		 * -j: join host (host info from system.conf)
		 * -d: deck edit
		 * -r: replay */
		if(!strcmp(argv[1], "-j") || !strcmp(argv[1], "-d") || !strcmp(argv[1], "-r")) {
			exit_on_return = true;
			irr::SEvent event;
			event.EventType = irr::EET_GUI_EVENT;
			event.GUIEvent.EventType = irr::gui::EGET_BUTTON_CLICKED;
			if(!strcmp(argv[1], "-j")) {
				event.GUIEvent.Caller = ygo::mainGame->btnLanMode;
				ygo::mainGame->device->postEventFromUser(event);
				//TODO: wait for wLanWindow show. if network connection faster than wLanWindow, wLanWindow will still show on duel scene.
				event.GUIEvent.Caller = ygo::mainGame->btnJoinHost;
				ygo::mainGame->device->postEventFromUser(event);
			} else if(!strcmp(argv[1], "-d")) {
				event.GUIEvent.Caller = ygo::mainGame->btnDeckEdit;
				ygo::mainGame->device->postEventFromUser(event);
			} else if(!strcmp(argv[1], "-r")) {
				event.GUIEvent.Caller = ygo::mainGame->btnReplayMode;
				ygo::mainGame->device->postEventFromUser(event);
				ygo::mainGame->lstReplayList->setSelected(0);
				event.GUIEvent.Caller = ygo::mainGame->btnLoadReplay;
				ygo::mainGame->device->postEventFromUser(event);
			}
		}
	}
	ygo::mainGame->MainLoop();
#ifdef _WIN32
	WSACleanup();
#else

#endif //_WIN32
	return EXIT_SUCCESS;
}
