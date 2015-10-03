/*
 * test.cpp
 *
 *  Created on: Aug 1, 2015
 *      Author: sdai
 */
#include "transmission.h"
#include <iostream>
int main() {
	unsigned char* buffer = new unsigned char[2];
	intToBytes(1321,buffer);
	packetBuffer buf;
	buf.addByte(0);
	buf.addByte(3);
	buf.addByte(1);
	buf.addByte(buffer[0]);
	buf.addByte(buffer[1]);
	delete[] buffer;
	frc5190::vector<transmissionPacket> tp = buf.getPackets();
	std::cout << "packet Num:" << tp.getSize() << std::endl;
	transmissionPacket p = tp[0];
	keepStateData data = interpreter::interpretStatSeq(p);
	const unsigned char* packet = p.toPacket();
	std::cout << "packet 1:";
	for(int i=0;i<p.getLength() + 2;++i) {
		std::cout << (short)packet[i];
	}
	std::cout << std::endl;
	delete[] packet;

	std::cout << "packet 1 keep state:" << std::endl;
	std::cout << "state" <<(short)data.getState() << std::endl;
	std::cout << "seq:" << data.getSequence() << std::endl;
}


