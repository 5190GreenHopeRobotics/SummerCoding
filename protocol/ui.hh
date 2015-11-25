#ifndef UI_HH_INCLUDED
#define UI_HH_INCLUDED

#include "transmission.h"


std::string getStringFromStdin();
keepStateData getKeepStateDataStdin();
commandData getCommandDataStdin();
navXSensor getNavXStdin();
basicEncoder getEncoderStdin();
basicPotentiometer getPotStdin();
basicDistance getDistanceStdin();
switchSensor getSwitchStdin();

#endif // UI_HH_INCLUDED
