/*  epocutils.h - header file
*
*   UPDATE node epocutils for Emotiv EPOC SDK Community - XavG <xav.guerin@hotmail.fr> 2016
*
*   supposed to be used:  1 -> as a utility class ( include header file in necessary .cpp file )
*                         2 -> as a utility class, embedded in a library that is available system wide ( .so file )
*
*/

/* includes the necessary standard stuff */
#include <iostream>
#include <map>

/* include the Emotiv-proprietary stuff */
#include "Includes/Iedk.h"				//UPDATE : edk.h -> Iedk.h
#include "Includes/IedkErrorCode.h"		//UPDATE : edkErrorCode.h -> IedkErrorCode.h
#include "Includes/IEmoStateDLL.h"		//UPDATE : EmoStateDLL.h -> IEmoStateDLL.h

/* we define what this header file makes available to file that include it */

#ifndef epocutils_h
#define epocutils_h

namespace epocutils
{



  //UPDATE//void helloWorld(); // a dummy 'Hello World' fcn
  //UPDATE//int highFive(); // anothe dummy fcn

  /* the REAL functions & stuff of the 'epocuutils2.0' helper */


  /* a 'EpocHeadset_struct' structure to hold all the stuff we're interested in */
  //typedef struct EpocHeadset_struct
  struct EpocHeadset_t
  {
    // bool to prevent unnecessary polling
    bool newDataToRead;

    // -- Epoc 'original' values -- // R: " #include <typeinfo> & then  std::cout << typeid( <the-var-name> ).name() << std::endl; ", but still not that easy to use ( ...)
    float time; // aka, returned a 'f' from the comment above in comments
    unsigned int userID;
    int wirelessSignalStatus; // R: do a 'static_cast<int>(ES_GetWIrelessSignalStatus(eState))' as else the type will be 'EE_SignalStrength_t' ( enum )

	// Expressiv suite
    int isBlinking;
    int isWinkingLeft;
    int isWinkingRight;
    int isLookingLeft;
    int isLookingRight;
        // Digg the doc & ask Emotiv for the 'IsEyesOpen', 'IsLookingUp' & 'IsLookingDown' fcn ( ... )
    float eyebrow; // R: " expressivStates[ EXP_EYEBROW ] "
    float furrow;
    float smile;
    float clench;
    float smirkLeft;
    float smirkRight;
    float laugh;
    // Affectiv suite
    float shortTermExcitement;
    float longTermExcitement;
    float engagementBoredom;
          // Digg the doc & ask Emotiv for the 'Meditation' & 'Frustration'
    // Cognitiv suite
    int mentalCommandAction;
    float mentalCommandActionPower;

  //} EpocHeadset;
  };

  /* 0 - create an 'EmoEngineEventHandle' */
  EmoEngineEventHandle createEventHandle();

  /* 0 - create an 'EmoStateHandle' */
  EmoStateHandle createStateHandle();

  /* 1 - initialize the struct members ( I actually don't know if it's actually required, but it's always nice to init parameters before setting values to them anyway ( ... ) */
  void initializeEpocHeadsetStruct(unsigned int& userID, EpocHeadset_t& epocheadset);

  /* 2 - connect to the Epoc headset */
  int connect(bool& connected);

  /* 3 - handle fresh data from the Epoc headset, if connected, & update the passed 'EpocHeadset_struct' structure with that data */
  void handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, EpocHeadset_t& epocheadset);

  /* 4 - disconnect from the Epoc headset AND clean up */
  void disconnect(bool& connected, EmoStateHandle& eState, EmoEngineEventHandle& eEvent);

}

#endif // epocutils_h
