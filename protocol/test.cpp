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
	info.stat = 0;
	info.reading = 1;
	for (int i = 0; i < 10; i++) {
		info.id = i;
		data.addSensor(info);
	}
	transmissionPacket p;
	p.setData(data);
	std::cout << "length:" << (short) p.getLength() << std::endl;
	const unsigned char* packet = p.toPacket();
	for (int i = 0; i < p.getLength() + 2; ++i) {
		std::cout << (short) packet[i];
	}

	delete packet;

	packet = p.toPacket();

	transmissionPacket r = interpretRawData(packet);

	delete packet;

	packet = r.toPacket();

	std::cout << std::endl;
	for (int i = 0; i < p.getLength() + 2; ++i) {
		std::cout << (short) packet[i];
	}

}

