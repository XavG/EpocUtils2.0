/*  epocutils.cpp - implementation file */
/*  - UPDATE 2016 -
About Emotiv SDK Community :
1. Affectiv is now Performance Metrics;
2. Cognitiv is now Mental Commands;
3. Expressiv is now Facial Expressions;

*/

/* we include the corresponding header file */
#include "epocutils.h"

/* includes will be below */
#include <stdexcept>        //for exception

/* we define the functions declared in the corresponding header file in the current implementation file */

/* implement our dummy 'Hello Wolrd' fcn */
/* UPDATE
void epocutils::helloWorld()
{
  std::cout << "Hello World !" << std::endl;
}
*/
/* implement another dummy fcn that returns an int */
/*UPDATE
int epocutils::highFive()
{
  return 5;
}
*/

/* the REAL functions & stuff of the 'epocutils' helper */
/* ---------------------------------------------------- */

/* 0 - create an 'EmoEngineEventHandle' */
EmoEngineEventHandle epocutils::createEventHandle()
{
  return IEE_EmoEngineEventCreate(); //UPDATE : EE_EmoEngineEventCreate -> IEE_EmoEngineEventCreate //// simply return the original fcn, wrapped
}

/* 0 - create an 'EmoStateHandle' */
EmoStateHandle epocutils::createStateHandle()
{
  return IEE_EmoStateCreate(); //UPDATE : EE_EmoStateCreate -> IEE_EmoStateCreate //// simply return the original fcn, wrapped
}

/* 1 - initialize the struct members */
void epocutils::initializeEpocHeadsetStruct(unsigned int& userID, epocutils::EpocHeadset_t& epocheadset)
{
  // we init the bool that we'll use to know if we have unread data from the Epoc headset
  epocheadset.newDataToRead = false;

  // we initialize the EpocHeadset struct with all its member parameters to 0, except the userID wich equals the one passed as argument
  epocheadset.time = 0.0f;
  epocheadset.userID = userID;
  epocheadset.wirelessSignalStatus = 0;

  // FacialExpression suite
  epocheadset.isBlinking = 0;
  epocheadset.isWinkingLeft = 0;
  epocheadset.isWinkingRight = 0;
  epocheadset.isLookingLeft = 0;
  epocheadset.isLookingRight = 0;
  epocheadset.eyebrow = 0.0f;
  epocheadset.furrow = 0.0f;
  epocheadset.smile = 0.0f;
  epocheadset.clench = 0.0f;
  epocheadset.smirkLeft = 0.0f;
  epocheadset.smirkRight = 0.0f;
  epocheadset.laugh = 0.0f;

  // Emotiv suite
  epocheadset.shortTermExcitement = 0.0f;
  epocheadset.longTermExcitement = 0.0f;
  epocheadset.engagementBoredom = 0.0f;

  // MentalCommand suite
  epocheadset.mentalCommandAction = 0;
  epocheadset.mentalCommandActionPower = 0; // close, but not the same as 'power' ( YES, I DISAGREE with Emotiv's words on this ( ... )

  std::cout << "epocutils:: Epoc headset struct initialized." << std::endl; // inform that the EpocHeadset struct has been initialized
}

/* 2 - connect to the Epoc headset */
int epocutils::connect(bool& connected)
{
	/*UPDATE*/
	try{
		int option;
		std::cout << "===================================================================" << std::endl;
        std::cout << "  Emotiv EPOC+/Insght Tools & SDK helper :: epocUtils2.0 (UPDATE)  " << std::endl;
        std::cout << "===================================================================" << std::endl;
        std::cout << "Press '1' to start and connect to the EmoEngine                    " << std::endl;
        std::cout << "Press '2' to connect to the EmoComposer                            " << std::endl;
		std::cout << ">> ";
		std::cin >> option;

		switch (option) {
        case 1:
        {
            if (IEE_EngineConnect() != EDK_OK) {			//UPDATE : EE_EngineConnect -> IEE_EngineConnect
                throw std::runtime_error("epocutils:: Emotiv Driver start up failed.");
            }
            break;
        }
        case 2:
        {
            if (IEE_EngineRemoteConnect("127.0.0.1", 1726) != EDK_OK) {
                std::string errMsg = "epocutils:: Cannot connect to EmoComposer on [127.0.0.1]:1726";
                throw std::runtime_error(errMsg.c_str());
            }
            break;
        }
        default:
            throw std::runtime_error("epocutils:: Invalid option...");
            break;
		}
		connected = true;
		std::cout << "epocutils:: Now connected to the Epoc headset and start receiving EmoState !" << std::endl;
		return 0;
	}
	 catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
        std::cin.get();
		return 1;
	}
}

/* 3 - handle fresh data from the Epoc headset, if connected, & update the passed 'EpocHeadset_struct' structure with that data */
void epocutils::handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, epocutils::EpocHeadset_t& epocheadset)
{
  if ( connected )
  {
    epoc_state = IEE_EngineGetNextEvent(eEvent); //UPDATE : EE_EngineGet.. -> IEE_EngineGet..// get the latest EmoEngine events ( aka 'stuff from Epoc' )

    if (epoc_state == EDK_OK)
    {
      IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);        //UPDATE : EE_ -> IEE_
      IEE_EmoEngineEventGetUserId(eEvent, &userID);                     //UPDATE : EE_ -> IEE_

      // Log the EmoState if it has been updated
      if (eventType == IEE_EmoStateUpdated)                            //UPDATE : EE_ -> IEE_

      {

            IEE_EmoEngineEventGetEmoState(eEvent, eState);          //UPDATE : EE_ -> IEE_
            const float timestamp = IS_GetTimeFromStart(eState);    //UPDATE : ES_GetTimeFromStart -> IS_GetTimeFromStart
            std::cout<<"epocutils:: Timestamp: " << timestamp <<" New EmoState from user: " << userID << std::endl;

			epocheadset.mentalCommandAction = static_cast<int>(IS_MentalCommandGetCurrentAction(eState));
			if (epocheadset.mentalCommandAction < 0){
				std::cout << "en c++ je recois bien l'ordre" << std::endl;
			}



            // write data from the EmoEngine to the EpocHeadset struct ... or just log it to stdout for the moment ?
            epocheadset.time = IS_GetTimeFromStart(eState);         //UPDATE : ES_GetTimeFromStart -> IS_Get...
            epocheadset.userID = userID;
            epocheadset.wirelessSignalStatus = static_cast<int>(IS_GetWirelessSignalStatus(eState));        //UPDATE : ES_GetWirelessSignal.. -> IS_Get...

            // FacialExpression suite
            epocheadset.isBlinking = IS_FacialExpressionIsBlink(eState);                    // UPDATE : ES_Expressiv[...] -> IS_FacialExpression[...]
            epocheadset.isWinkingLeft = IS_FacialExpressionIsLeftWink(eState);
            epocheadset.isWinkingRight = IS_FacialExpressionIsRightWink(eState);
            epocheadset.isLookingLeft = IS_FacialExpressionIsLookingLeft(eState);
            epocheadset.isLookingRight = IS_FacialExpressionIsLookingRight(eState);

            std::map<IEE_FacialExpressionAlgo_t, float> expressivStates;                    //UPDATE : EE_ExpressivAlgo_t -> IEE_FacialExpressionAlgo_t
            IEE_FacialExpressionAlgo_t upperFaceAction = IS_FacialExpressionGetUpperFaceAction(eState);
            float upperFacePower = IS_FacialExpressionGetUpperFaceActionPower(eState);
            IEE_FacialExpressionAlgo_t lowerFaceAction = IS_FacialExpressionGetLowerFaceAction(eState);
            float lowerFacePower = IS_FacialExpressionGetLowerFaceActionPower(eState);

            expressivStates[ upperFaceAction ] = upperFacePower;
            expressivStates[ lowerFaceAction ] = lowerFacePower;

            epocheadset.eyebrow = expressivStates[ FE_SURPRISE ];       //UPDATE : EXP_EYEBROW -> FE_SURPRISE
            epocheadset.furrow = expressivStates[ FE_FROWN ];         //UPDATE : EXP_FURROW -> FE_FROWN
            epocheadset.smile = expressivStates[ FE_SMILE ];           //UPDATE : EXP_SMILE -> FE_SMILE
            epocheadset.clench = expressivStates[ FE_CLENCH ];         //UPDATE : EXP_CLENCH -> FE_CLENCH
            epocheadset.smirkLeft = expressivStates[ FE_SMIRK_LEFT  ];  //UPDATE : EXP_SMIRK_LEFT -> FE_SMIRK_LEFT
            epocheadset.smirkRight = expressivStates[ FE_SMIRK_RIGHT  ];   //UPDATE : EXP_SMIRK_RIGHT -> FE_SMIRK_RIGHT
            epocheadset.laugh = expressivStates[ FE_LAUGH  ];           //UPDATE : EXP_LAUGH -> FE_LAUGH

            // Affective suite
            /*UPDATE
            epocheadset.shortTermExcitement = ES_AffectivGetExcitementShortTermScore(eState);       // UPDATE : Can't find the new function
            epocheadset.longTermExcitement = ES_AffectivGetExcitementLongTermScore(eState);
            epocheadset.engagementBoredom = ES_AffectivGetEngagementBoredomScore(eState);
            */
            // Cognitiv suite
            epocheadset.mentalCommandAction = static_cast<int>(IS_MentalCommandGetCurrentAction(eState)); // UPDATE : IS_Cognitiv[...] -> IS_MentalCommand[...]
            epocheadset.mentalCommandActionPower = IS_MentalCommandGetCurrentActionPower(eState);

            epocheadset.newDataToRead = true; // we update our boolean to indicate that data is yet to be read

            /* Print somes infrmations sent by the headset (or composer) */
            std::cout << "Time : " << epocheadset.time << std::endl;
            std::cout << "UserID : " << epocheadset.userID << std::endl;
            std::cout << "IsBlinking : " << epocheadset.isBlinking << std::endl;
            std::cout << "MentalCommand Action : " << epocheadset.mentalCommandAction << std::endl;
            std::cout << "MentalCommand Action Power : " << epocheadset.mentalCommandActionPower << std::endl;
      }
    } else if (epoc_state != EDK_NO_EVENT)
    {
      std::cout << "epocutils:: No new data from the Epoc headset:" << std::endl; // inform that new data is available
      std::cout << "epocutils:: Internal error in Emotiv Engine!" << std::endl;
      // handle that :/
    }
  }
}

/* 4 - disconnect from the Epoc headset AND clean up */
void epocutils::disconnect(bool& connected, EmoStateHandle& eState, EmoEngineEventHandle& eEvent)
{
  IEE_EngineDisconnect(); //UPDATE : EE_EngineDisconnect -> IEE_EngineDisconnect// we disconnect from the EmoEngine
  connected = false; // we update the headset connection status ( a 'connected = false' will prevent new readings from the EmoEngine using 'epocutils::handleEvents()' )
  std::cout << "\nepocutils:: Now disconnected from the Epoc headset." << std::endl; // inform that an error occured
  IEE_EmoStateFree(eState); //UPDATE : EE_...-> IEE_...// free the 'EmoStateHandle' instance
  std::cout << "epocutils:: EmoStateHandle resources freed." << std::endl; // inform that the EmoStateHandle instance has been freed
  IEE_EmoEngineEventFree(eEvent); //UPDATE : EE_...-> IEE_...//// free the 'EmoEngineEventHandle' instance
  std::cout << "epocutils:: EmoEngineEventHandle resources freed." << std::endl; // inform that the EmoEngineEventHandle instance has been freed
}


/* UPDATE DONE - 2016 - By XavG <xav.guerin@hotmail.fr> */
