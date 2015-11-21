/*
 * test.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: sdai
 */

#include "transmission.h"
#include <iostream>
int main() {
	navXSensor sensor;
	sensor.setAltitude(2);
	sensor.setBarometricPressure(1);
	sensor.setFusedHeading(12);
	sensor.setTemperature(123.23);
	sensor.setLinearAccelX(0.4);
	sensor.setLinearAccelY(3);
	sensor.setLinearAccelZ(3.2);
	sensor.setMagnetometerX(12);
	sensor.setMagnetometerY(3);
	sensor.setStat(1);
	sensor.setId(1);
	transmissionPacket tp;
	tp.setData(sensor);
	const unsigned char* packet = tp.toPacket();
	int length = sensor.getLength() + 2;
	for(int i=0;i<length;++i) {
		std::cout << (short)packet[i];
	}
	std::cout << std::endl;
	delete[] packet;
}


