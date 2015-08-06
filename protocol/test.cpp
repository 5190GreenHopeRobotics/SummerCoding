/*
 * test.cpp
 *
 *  Created on: Aug 1, 2015
 *      Author: sdai
 */
#include "transmission.h"
#include <iostream>
int main() {
	packetBuffer buf;
	buf.addByte(0);
	buf.addByte(2);
	buf.addByte(1);
	buf.addByte(10);
	buf.addByte(1);
	buf.addByte(2);
	buf.addByte(1);
	buf.addByte(0);

	frc5190::vector<transmissionPacket> tp = buf.getPackets();
	std::cout << "packet Num:" << tp.getSize() << std::endl;
	transmissionPacket p = tp[0];
	transmissionPacket p2 = tp[1];
	const unsigned char* packet = p.toPacket();
	std::cout << "packet 1:";
	for(int i=0;i<p.getLength() + 2;++i) {
		std::cout << (short)packet[i];
	}
	std::cout << std::endl;
	delete[] packet;
	std::cout << "packet 2:";
	packet = p2.toPacket();
	for(int i=0;i<p2.getLength() + 2;++i) {
		std::cout << (short)packet[i];
	}
	std::cout << std::endl;
	delete[] packet;
}


