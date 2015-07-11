/*
 * test.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: sdai
 */
#include <iostream>
#include "transmission.h"

int main() {
	sensorData data;
	data.setArduinoStat(1);
	sensorInfo info;
	info.id = 0;
	info.reading = 13;
	info.stat = 2;
	data.addSensor(info);
	info.id = 1;
	data.addSensor(info);
	const unsigned char* packet = data.toPacket();
	std::cout << std::endl;
	for(int i=0;i<2 + data.getSensorNum() * 4;++i) {
		std::cout<< (short)packet[i];
	}
	std::cout << std::endl;
	delete packet;
}



