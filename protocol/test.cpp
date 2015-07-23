/*
 * test.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: Emperor
 */
#include "iostream"
#include "transmission.h"



using namespace std;

int main() {
	sensorData data;
	data.setArduinoStat(1);
	sensorInfo info;
	info.stat=0;
	info.reading=1;
	for(int i = 0 ;i< 10; i++) {
		info.id = i;
		data.addSensor(info);
	}
	const unsigned char* packet = data.toPacket();
	std::cout << std::endl;
	for(int i=0;i<2 + data.getSensorNum() * 4;++i) {
		std::cout<< (short)packet[i];
	}
	std::cout << std::endl;
	delete packet;


	return 0;
	transmissionPacket tp;
	commandData cmd;
	cmd.setCommand(3);
	cmd.setParameter(2);
	tp.setData(cmd);

	commandData newData = interpreter::interpretCommandData(tp);
	std::cout << "cmd:" << (short)newData.getCommand() << " param:" << (short)newData.getParameter() << std::endl;
}

