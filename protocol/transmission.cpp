/*
 * transmission.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: sdai
 */
#include "transmission.h"

int bytesToInt(const unsigned char* b, int length) {
	int val = 0;
	int j = 0;
	for (int i = length-1; i >= 0; --i)
	{
		val += (b[i] & 0xFF) << (8*j);
        ++j;
    }
    return val;
}

//////////////////////////////////////////////////////////////////////////
//						TransmissionPacket 								//
//////////////////////////////////////////////////////////////////////////
transmissionPacket::transmissionPacket() {
	this->messageType = 0;
	this->messageLength = 0;
	this->messageData = nullptr;
}

transmissionPacket::transmissionPacket(const transmissionPacket& src) {
	this->messageType = src.messageType;
	this->messageLength = src.messageLength;
	this->messageData = new unsigned char[messageLength + 1];
	for (int i = 0; i < messageLength; i++) {
		messageData[i] = src.messageData[i];
	}
}

transmissionPacket::~transmissionPacket() {
	if (this->messageData != nullptr) {
		delete[] this->messageData;
	}
}

transmissionPacket& transmissionPacket::operator =(
		const transmissionPacket& src) {
	this->messageType = src.messageType;
	this->messageLength = src.messageLength;
	if (this->messageData != nullptr) {
		delete[] this->messageData;
	}
	this->messageData = new unsigned char[this->messageLength + 1];
	for (int i = 0; i < messageLength; i++) {
		messageData[i] = src.messageData[i];
	}
	return *this;
}

void transmissionPacket::setMessageType(const unsigned char t) {
	this->messageType = t;
}

unsigned char transmissionPacket::getMessageType() const {
	return this->messageType;
}

unsigned char transmissionPacket::getLength() const {
	return this->messageLength;
}

void transmissionPacket::setData(transmissionData& data) {
	if (messageData != nullptr) {
		delete[] messageData;
	}
	const unsigned char* packet = data.toPacket();
	messageLength = data.getLength();
	messageData = new unsigned char[messageLength];
	for (int i = 0; i < messageLength; ++i) {
		messageData[i] = packet[i];
	}
	delete[] packet;
	return;
}

void transmissionPacket::setData(const unsigned char* data) {
	if (this->messageData != nullptr) {
		delete[] messageData;
	}
	this->messageData = (unsigned char*) data;
}

unsigned char* transmissionPacket::toPacket() const {
	unsigned char* packet = new unsigned char[messageLength + 3];
	packet[0] = messageType;
	packet[1] = messageLength;
	if (messageData != nullptr) {
		for (int i = 2; i < messageLength + 2; ++i) {
			packet[i] = messageData[i - 2];
		}
		return packet;
	} else {
		delete[] packet;
		return nullptr;
	}

}

const unsigned char* transmissionPacket::getData() const {
	return messageData;
}

void transmissionPacket::setMessageLength(const unsigned char length) {
	messageLength = length;
}
/////////////////////////////////////////////////////////////////////
//							TransmissionData					   //
/////////////////////////////////////////////////////////////////////
transmissionData::~transmissionData() {

}

/////////////////////////////////////////////////////////////////////
//							KeepStateData						   //
/////////////////////////////////////////////////////////////////////

keepStateData::keepStateData() {
	this->state = 0;
}

keepStateData::keepStateData(const keepStateData& src) {
	for (int i = 0; i < 2; ++i) {
		sequence[i] = src.sequence[i];
	}
	this->state = src.state;
}

keepStateData& keepStateData::operator =(const keepStateData& src) {
	for (int i = 0; i < 2; ++i) {
		sequence[i] = src.sequence[i];
	}
	this->state = src.state;
	return *this;
}

bool keepStateData::operator ==(const keepStateData& src) {
	bool st = false;
	bool seq = true;
	st = (state == src.getState());
	for(int i=0;i<2;++i) {
		if(sequence[i] != src.sequence[i]) {
			seq = false;
			break;
		}
	}
	return st && seq;
}

void keepStateData::setState(const unsigned char state) {
	this->state = state;
}

unsigned char keepStateData::getState() const {
	return state;
}

void keepStateData::setSequence(const unsigned char* sequence) {
	for(int i=0;i<2;++i) {
		this->sequence[i] = sequence[i];
	}
}

const unsigned char* keepStateData::getSequence() const {
	return sequence;
}

int keepStateData::getSequenceInt() const {
	return bytesToInt(sequence, 2);
}

const unsigned char* keepStateData::toPacket() {
	unsigned char* packet = new unsigned char[5];
	packet[0] = state;
	for(int i=1;i<3;++i) {
		packet[i] = sequence[i-1];
	}
	return packet;
}

unsigned char keepStateData::getLength() const {
	return 3;
}

//////////////////////////////////////////////////////////////////////
//							CommandData								//
//////////////////////////////////////////////////////////////////////

commandData::commandData() {
	this->command = 0;
}

commandData::commandData(const commandData& src) {
	this->command = src.command;
	setParameter(src.getParameter());
}

commandData& commandData::operator =(const commandData& src) {
	this->command = src.command;
	setParameter(src.getParameter());
	return *this;
}

bool commandData::operator ==(const commandData& src) {
	bool commandEqual;
	bool paramEqual;
	commandEqual = (this->command == src.command);
	for(int i=0;i<2;++i) {
		if(param[i] != src.param[i]) {
			paramEqual = false;
			break;
		}
	}
	return commandEqual && paramEqual;
}

void commandData::setCommand(const unsigned char command) {
	this->command = command;
}

unsigned char commandData::getCommand() const {
	return command;
}

void commandData::setParameter(const unsigned char* param) {
	for(int i=0;i<2;++i) {
		this->param[i] = param[i];
	}
	return;
}

const unsigned char* commandData::getParameter() const {
	return param;
}

const unsigned char* commandData::toPacket() {
	unsigned char* data = new unsigned char[3];
	data[0] = command;
	for(int i=1;i<3;++i) {
		data[i] = param[i-1];
	}
	return data;
}

unsigned char commandData::getLength() const {
	return 3;
}

/////////////////////////////////////////////////////////////
//						SensorInfo						   //
/////////////////////////////////////////////////////////////

bool sensorInfo::operator ==(const sensorInfo& info) {
	bool isReadingSame;
	if(id != info.id || stat != info.stat) {
		return false;
	}
	for(int i=0;i<2;++i) {
		if(reading[i] != info.reading[i]) {
			return false;
		}
	}
	return true;
}

bool sensorInfo::operator !=(const sensorInfo & info) {
	return !(*this == info);
}

////////////////////////////////////////////////////////////
//							SensorData					  //
////////////////////////////////////////////////////////////

sensorData::sensorData() {
	this->arduinoStat = 1;
	this->sensorNum = 0;
}

sensorData::sensorData(const sensorData& data) {
	this->arduinoStat = data.arduinoStat;
	this->sensorNum = data.sensorNum;
	this->data = data.data;
}

sensorData& sensorData::operator =(const sensorData& src) {
	this->arduinoStat = src.arduinoStat;
	this->sensorNum = src.sensorNum;
	this->data = src.data;
	return *this;
}

bool sensorData::operator ==(const sensorData& src) {
	if (this->arduinoStat != src.arduinoStat
			|| this->sensorNum != src.sensorNum) {
		return false;
	}
	return this->data == src.data;
}

void sensorData::setArduinoStat(const unsigned char stat) {
	this->arduinoStat = stat;
}

unsigned char sensorData::getArduinoStat() const {
	return arduinoStat;
}

unsigned char sensorData::getSensorNum() const {
	return sensorNum;
}

void sensorData::addSensor(const sensorInfo& data) {
	this->data.append(data);
	sensorNum++;
}

const sensorInfo* sensorData::getSensors() const {
	return this->data.getArray();
}

const unsigned char* sensorData::toPacket() {
	unsigned char* packet = new unsigned char[2 + 3 * sensorNum + 2 * sensorNum];
	packet[0] = arduinoStat;
	packet[1] = sensorNum;
	for (int i = 0; i < sensorNum; ++i) {
		packet[(i + 1) * 2] = data[i].id;
		packet[((i + 1) * 2) + 1] = data[i].stat;
	}
	for (int i = 0; i < sensorNum; ++i) {
		packet[(2 + (2 * sensorNum)) + (i * 3)] = data[i].id;
		packet[3 + (2 * sensorNum) + (i * 3)] = data[i].reading[0];
		packet[4 + (2 * sensorNum) + (i * 3)] = data[i].reading[1];
	}
	return packet;
}

unsigned char sensorData::getLength() const {
	unsigned char result = 0;
	result = (sensorNum * 5) + 2;
	return result;
}

sensorData::~sensorData() {
}

//////////////////////////////////////////////////////////////////////
//								Interpreters				   		//
//////////////////////////////////////////////////////////////////////

keepStateData interpreter::interpretStatSeq(const transmissionPacket& tp) {
	keepStateData ksd;
	unsigned char buf[4];
	buf[0] = tp.getData()[1];
	buf[1] = tp.getData()[2];
	buf[2] = tp.getData()[3];
	buf[3] = tp.getData()[4];
	ksd.setSequence(buf);
	return ksd;
}
sensorData interpreter::interpretSensData(const transmissionPacket& tp) {
	sensorData result;
	const unsigned char* raw = tp.getData();
	result.setArduinoStat(raw[0]);
	int num = raw[1];
	sensorInfo temp;
	for (int i = 0; i < num; ++i) {
		temp.id = raw[(i * 2) + 2];
		temp.stat = raw[(i * 2) + 3];
		temp.reading[0] = raw[3 + (2 * num) + (i * 3)];
		temp.reading[1] = raw[4 + (2 * num) + (i * 3)];
		result.addSensor(temp);
	}
	return result;
}
commandData interpreter::interpretCommandData(const transmissionPacket& tp) {
	commandData data;
	unsigned char buf[2];
	buf[0] = tp.getData()[1];
	buf[1] = tp.getData()[2];
	data.setParameter(buf);
	data.setCommand(tp.getData()[0]);
	return data;
}

////////////////////////////////////////////////////////////
//						Buffer							  //
////////////////////////////////////////////////////////////
transmissionPacket packetBuffer::interpretRawData(const unsigned char* packet) {
	unsigned char length = 0;
	length = packet[1];
	unsigned char type = 0;
	type = packet[0];
	unsigned char* data = new unsigned char[length];
	for (int i = 0; i < length; ++i) {
		data[i] = packet[i + 2];
	}
	transmissionPacket result;
	result.setMessageType(type);
	result.setMessageLength(length);
	result.setData(data);
	return result;
}
void packetBuffer::addByte(unsigned char b) {
	buf.append(b);
}

unsigned char* packetBuffer::getNext() {
	int packetSize = 0;
	int bufSize = buf.getSize();
	if (bufSize < 4) {
		return nullptr;
	}
	packetSize = buf[1] + 2;
	if (bufSize < packetSize) {
		return nullptr;
	}
	unsigned char* p = new unsigned char[packetSize];
	for (int i = 0; i < packetSize; ++i) {
		p[i] = buf[i];
	}
	for (int i = packetSize - 1; i > -1; --i) {
		buf.remove(i);
	}
	return p;

}

frc5190::vector<transmissionPacket> packetBuffer::getPackets() {
	unsigned char* temp;
	frc5190::vector<transmissionPacket> b;
	while (true) {
		temp = getNext();
		if (temp == nullptr) {
			return b;
		}
		b.append(interpretRawData(temp));
		delete[] temp;
	}
}
