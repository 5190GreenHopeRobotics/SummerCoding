#ifndef UI_HH_INCLUDED
#define UI_HH_INCLUDED

#include "transmission.h"

long getLongStdin();
float getFloatStdin();
int getIntStdin();
std::string getStringFromStdin();
unsigned char getUnsignedCharStdin();
keepStateData getKeepStateDataStdin();
commandData getCommandDataStdin();
navXSensor getNavXStdin();
basicEncoder getEncoderStdin();
basicPotentiometer getPotStdin();
basicDistance getDistanceStdin();
switchSensor getSwitchStdin();
void processCommand(const std::string& command);
void getStatID(sensorInfo& data);
#endif // UI_HH_INCLUDED
