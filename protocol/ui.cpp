/*
 * test.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: sdai
 */

#include <iostream>
#include "ui.hh"
#include "utils.hh"
#include <cstdlib>



std::string getStringFromStdin() {
    std::string result;
    char buf;
    while(true) {
        std::cin >> std::noskipws >> buf;
        if(!std::cin) {
            std::cerr << "encoutered error while reading from stdin" << std::endl;
            std::cin.clear();
            return std::string();
        }
        if(buf == '\n') {
            break;
        }
        result += buf;
    }
    return result;
}

float getFloatStdin() {
    float result = 0;
    std::string buffer = getStringFromStdin();
    result = atof(buffer.c_str());
    return result;
}

long getLongStdin() {
    long result = 0;
    std::string buffer = getStringFromStdin();
    result = atol(buffer.c_str());
    return result;
}

int getIntStdin() {
    int result = 0;
    std::string buffer = getStringFromStdin();
    result = atoi(buffer.c_str());
    return result;
}

unsigned char getUnsignedCharStdin() {
    unsigned char result;
    int buffer = getIntStdin();
    if(buffer > 255) {
        return 0;
    }
    result = buffer;
    return result;
}

void getStatID(sensorInfo& data) {
    unsigned char buf;
    std::cout << "input id:";
    buf = getUnsignedCharStdin();
    data.setId(buf);
    std::cout << "input status:";
    buf = getUnsignedCharStdin();
    data.setStat(buf);
}

keepStateData getKeepStateDataStdin() {
    keepStateData result;
    unsigned char state = 0;
    std::cout << "input the state value:";
    state = getUnsignedCharStdin();
    result.setState(state);
    std::cout << "input the sequence number:";
    int sequenceData = getIntStdin();
    result.setSequence(sequenceData);
    return result;
}

commandData getCommandDataStdin() {
    commandData result;
    unsigned char command = 0;
    std::cout << "input the command id:";
    command = getUnsignedCharStdin();
    result.setCommand(command);
    std::cout << "input the command parameter:";
    int param = getIntStdin();
    result.setParameter(param);
    return result;
}

navXSensor getNavXStdin() {
    float buffer;
    navXSensor result;
    getStatID(result);
    std::cout << "input the altitude:";
    buffer = getFloatStdin();
    result.setAltitude(buffer);
    std::cout << "input the temperature:";
    buffer = getFloatStdin();
    result.setTemperature(buffer);
    std::cout << "input the fused heading:";
    buffer = getFloatStdin();
    result.setFusedHeading(buffer);
    std::cout << "input the barometric pressure:";
    buffer = getFloatStdin();
    result.setBarometricPressure(buffer);
    std::cout << "input the linear acceleration x:";
    buffer = getFloatStdin();
    result.setLinearAccelX(buffer);
    std::cout << "input the linear acceleration y:";
    buffer = getFloatStdin();
    result.setLinearAccelY(buffer);
    std::cout << "input the linear acceleration z:";
    buffer = getFloatStdin();
    result.setLinearAccelZ(buffer);
    std::cout << "input the magnetometer x:";
    buffer = getFloatStdin();
    result.setMagnetometerX(buffer);
    std::cout << "input the magnetometer y:";
    buffer = getFloatStdin();
    result.setMagnetometerY(buffer);
    return result;
}

basicEncoder getEncoderStdin() {
    basicEncoder enc;
    getStatID(enc);
    unsigned char direction = 0;
    long counts = 0;
    std::cout << "input the encoder direction:";
    direction = getUnsignedCharStdin();
    std::cout << "input the encoder count:";
    counts = getLongStdin();
    enc.setCounts(counts);
    enc.setDirection(direction);
    return enc;
}

basicPotentiometer getPotStdin() {
    basicPotentiometer pot;
    getStatID(pot);
    float angle = 0;
    std::cout << "input the potentiometer angle:";
    angle = getFloatStdin();
    pot.setAngle(angle);
    return pot;
}

basicDistance getDistanceStdin() {
    basicDistance dis;
    getStatID(dis);
    float distance = 0;
    std::cout << "input the distance:";
    distance = getFloatStdin();
    dis.setDistance(distance);
    return dis;
}

switchSensor getSwitchStdin() {
    switchSensor sws;
    getStatID(sws);
    unsigned char state;
    std::cout << "input the switch value:";
    state = getUnsignedCharStdin();
    sws.setSwitchValue(state);
    return sws;
}

void processCommand(const std::string& command) {
    transmissionPacket buffer;
    binaryFile bin;
    bool commandExecuted = false;
    std::string fileName;
    if(command == "keepStatePacket") {
        buffer.setMessageType(1);
        keepStateData ksd = getKeepStateDataStdin();
        buffer.setData(ksd);
        commandExecuted = true;
    }
    if(command == "commandDataPacket") {
        buffer.setMessageType(2);
        commandData cmd = getCommandDataStdin();
        buffer.setData(cmd);
        commandExecuted = true;
    }
    if(command == "navXPacket") {
        buffer.setMessageType(3);
        navXSensor nvx = getNavXStdin();
        buffer.setData(nvx);
        commandExecuted = true;
    }
    if(command == "encoderPacket") {
        buffer.setMessageType(3);
        basicEncoder enc = getEncoderStdin();
        buffer.setData(enc);
        commandExecuted = true;
    }
    if(command == "potPacket") {
        buffer.setMessageType(3);
        basicPotentiometer pot = getPotStdin();
        buffer.setData(pot);
        commandExecuted = true;
    }
    if(command == "distanceSensorPacket") {
        buffer.setMessageType(3);
        basicDistance dis = getDistanceStdin();
        buffer.setData(dis);
        commandExecuted = true;
    }
    if(command == "switchPacket") {
        buffer.setMessageType(3);
        switchSensor switchData = getSwitchStdin();
        buffer.setData(switchData);
        commandExecuted = true;
    }
    if(commandExecuted) {
        std::cout << "input a filename to save:";
        fileName = getStringFromStdin();
        bin.setFilename(fileName);
        unsigned char* output = buffer.toPacket();
        bin.write(output,buffer.getLength() + 2);
        std::cout << "saved!" << std::endl;
        delete[] output;
    }
}


int main() {
    while(true) {
        std::string command;
        std::cout << "command(? for help):";
        command = getStringFromStdin();
        if(command == "quit") {
            return 0;
        }
        if(command == "?") {
            std::cout << "keepStatePacket" << std::endl;
            std::cout << "commandDataPacket" << std::endl;
            std::cout << "navXPacket" << std::endl;
            std::cout << "encoderPacket" << std::endl;
            std::cout << "potPacket" << std::endl;
            std::cout << "distanceSensorPacket" << std::endl;
            std::cout << "switchPacket" << std::endl;
        }
        processCommand(command);
    }
}


