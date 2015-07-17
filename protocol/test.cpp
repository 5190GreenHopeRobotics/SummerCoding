/*
 * test.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: sdai
 */
#include <iostream>
#include "transmission.h"

using namespace std;

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
	const unsigned char* packet = p.toPacket();
	delete packet;
	packet = p.toPacket();
	transmissionPacket r = interpreter::interpretRawData(packet);
	delete packet;
	packet = r.getData();
	for(int i=0;i< r.getLength();i++) {
		std::cout << (short)packet[i];
	}
	std::cout << std::endl;
	sensorData s = interpreter::interpretSensData(p);
	const sensorInfo* d = s.getSensors();
	std::cout << "sensorNum:" << (short)s.getSensorNum() << std::endl;
	std::cout << "stat:" << (short)s.getArduinoStat() << std::endl;
	for(int i=0;i< s.getSensorNum();++i){
		cout << "id:" << (short)d[i].id << " stat:"<< (short)d[i].stat << " reading:" << (short)d[i].reading << endl;
	}

	/*cout << (ks == kState) << endl;*/

}

