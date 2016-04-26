#include <iostream>
#include <windows.h>
#include <conio.h>

#include "epocutils.h"

using namespace std;

int main()
{
    /* init code for the Epoc headset */
  EmoEngineEventHandle eEvent = epocutils::createEventHandle(); // create an 'EmoEngineEventHandle' using the 'epocutils' helper
  EmoStateHandle       eState = epocutils::createStateHandle(); // create an 'EmoStateHandle' using the 'epocutils' helper
  unsigned int         userID = 0; // create a user ID
  int                  epoc_state = 0; // create a int variable that'll hold the return codes from the EmoEngine ( EDK_OK, etc .. )
  bool                 connected = false; // create a boolean that'll be used to gather data from the Epoc headset if true ( must be set to 'true' to handle new events with 'epocutils::handleEvents()' )
  epocutils::EpocHeadset_t user;

  // Data Initialization
  epocutils::initializeEpocHeadsetStruct( userID, user);

  // connection to the Epoc headset
  int epoc_conn_retval = epocutils::connect(connected); // try to connect to the Epoc headset

  if ( epoc_conn_retval != 0 ) {
    std::cout << "Failed to connect to the Epoc headset." << std::endl;
    std::cout << "Program says: I'm of none use if no Epoc headset if connected ! -> Exiting." << std::endl;
    //exit 1; // for certain programs that are pretty useless without the Epoc headset ( for games, we can still use the mouse & keyboard or some Arduino-made joystick ;p )
  } else {
    std::cout << "Program : Connected to the Epoc headset." << std::endl;
  }

  while (!_kbhit()) {
    epocutils::handleEvents(connected, epoc_state, eEvent, eState, userID, user); // the latest data from the Epoc headset 'll update the 'epocheadset' struct content
    Sleep(10);
  }

  // free all the resources used by the Epoc headset
  epocutils::disconnect(connected, eState, eEvent); // disconnect from the Epoc headset & free all the resources involved (bool to 'false', EmoEngine resources freed ( gone ^^! ) )
}
