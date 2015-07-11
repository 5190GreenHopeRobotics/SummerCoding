/*
 * transmission.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: sdai
 */
#include "transmission.h"
#include <cstring>
#include <iostream>
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
	strcpy((char*) this->messageData, (const char*) src.messageData);
}

transmissionPacket::~transmissionPacket() {
	if (this->messageData != nullptr) {
		delete this->messageData;
	}
}

transmissionPacket& transmissionPacket::operator =(
		const transmissionPacket& src) {
	this->messageType = src.messageType;
	this->messageLength = src.messageLength;
	if (this->messageData != nullptr) {
		delete this->messageData;
	}
	this->messageData = new unsigned char[this->messageLength + 1];
	strcpy((char*) this->messageData, (const char*) src.messageData);
	return *this;
}

bool transmissionPacket::operator ==(const transmissionPacket& src) {
	bool basic = this->messageType == src.messageType
			&& this->messageLength == src.messageLength;
	bool data = false;
	if (this->messageData == nullptr && src.messageData == nullptr) {
		data = true;
	} else {
		data = strcmp((char*) this->messageData, (char*) src.messageData);
	}
	return basic && data;
}

void transmissionPacket::setMessageType(unsigned char t) {
	this->messageType = t;
}

unsigned char transmissionPacket::getMessageType() const {
	return this->messageType;
}

void transmissionPacket::setData(transmissionData& data) {
	if (messageData != nullptr) {
		delete messageData;
	}
	const unsigned char* packet = data.toPacket();
	messageLength = (unsigned char) strlen((const char*) packet);
	messageData = new unsigned char[messageLength + 1];
	strcpy((char*) messageData, (const char*) packet);
	delete packet;
	return;
}

unsigned char* transmissionPacket::toPacket() const {
	unsigned char* packet = new unsigned char[messageLength + 3];
	packet[0] = messageType;
	packet[1] = messageLength;
	if (messageData != nullptr) {
		strcat((char*) packet, (char*) messageData);
		return packet;
	} else {
		delete packet;
		return nullptr;
	}

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
	this->sequence = 0;
	this->state = 0;
}

keepStateData::keepStateData(const keepStateData& src) {
	this->sequence = src.sequence;
	this->state = src.state;
}

keepStateData& keepStateData::operator =(const keepStateData& src) {
	this->sequence = src.sequence;
	this->state = src.state;
	return *this;
}

bool keepStateData::operator ==(const keepStateData& src) {
	return this->sequence == src.sequence && this->state == src.state;
}

void keepStateData::setState(const unsigned char state) {
	this->state = state;
}

unsigned char keepStateData::getState() const {
	return state;
}

void keepStateData::setSequence(const unsigned char sequence) {
	this->sequence = sequence;
}

unsigned char keepStateData::getSequence() const {
	return sequence;
}

const unsigned char* keepStateData::toPacket() {
	unsigned char* packet = new unsigned char[3];
	packet[0] = state;
	packet[1] = sequence;
	return packet;
}

//////////////////////////////////////////////////////////////////////
//							CommandData								//
//////////////////////////////////////////////////////////////////////

commandData::commandData() {
	this->command = 0;
	this->param = 0;
}

commandData::commandData(const commandData& src) {
	this->command = src.command;
	this->param = src.param;
}

commandData& commandData::operator =(const commandData& src) {
	this->command = src.command;
	this->param = src.param;
	return *this;
}

bool commandData::operator ==(const commandData& src) {
	return this->command == src.command && this->param == src.param;
}

void commandData::setCommand(const unsigned char command) {
	this->command = command;
}

unsigned char commandData::getCommand() const {
	return command;
}

void commandData::setParameter(const unsigned char param) {
	this->param = param;
}

unsigned char commandData::getParameter() const {
	return param;
}

const unsigned char* commandData::toPacket() {
	unsigned char* data = new unsigned char[3];
	data[0] = command;
	data[1] = param;
	return data;
}

////////////////////////////////////////////////////////////
//							SensorData					  //
////////////////////////////////////////////////////////////

sensorData::sensorData() :
		block(5) {
	this->arduinoStat = 1;
	this->capacity = block;
	this->current = -1;
	this->sensorNum = 0;
	this->sensors = new sensorInfo[block + 1];
}

sensorData::sensorData(const sensorData& data) :
		block(data.block) {
	this->arduinoStat = data.arduinoStat;
	this->capacity = data.capacity;
	this->current = data.current;
	this->sensorNum = data.sensorNum;
	cpSensorData(this->sensors, data.sensors);
}

sensorData& sensorData::operator =(const sensorData& src) {
	if (this->sensors != nullptr) {
		delete sensors;
	}
	this->arduinoStat = src.arduinoStat;
	this->capacity = src.capacity;
	this->current = src.current;
	this->sensorNum = src.sensorNum;
	cpSensorData(this->sensors, src.sensors);
	return *this;
}

bool sensorData::operator ==(const sensorData& src) {
	if (this->arduinoStat != src.arduinoStat || this->capacity != src.capacity
			|| this->current != src.current
			|| this->sensorNum != src.sensorNum) {
		return false;
	}
	return cmpSensorData(this->sensors, src.sensors);
}

void sensorData::cpSensorData(sensorInfo* src, const sensorInfo* data) {
	for (int i = 0; i < sensorNum + 1; ++i) {
		src[i].id = data[i].id;
		src[i].reading = data[i].reading;
		src[i].stat = data[i].stat;
	}
}

bool sensorData::cmpSensorData(const sensorInfo* src, const sensorInfo* data) {
	for (int i = 0; i < sensorNum + 1; ++i) {
		if (src[i].id != data[i].id || src[i].reading != data[i].reading
				|| src[i].stat != data[i].stat) {
			return false;
		}
	}
	return true;
}

void sensorData::relocate() {
	sensorInfo* temp = new sensorInfo[capacity + block + 1];
	cpSensorData(temp, this->sensors);
	delete sensors;
	sensors = temp;
	capacity += 5;
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
	this->current++;
	sensors[current] = data;
	sensorNum++;
	if (current == capacity - 1) {
		relocate();
	}
}

const sensorInfo* sensorData::getSensors() const {
	return sensors;
}

const unsigned char* sensorData::toPacket() {
	char* packet = new char[2 + 4 * sensorNum];
	packet[0] = arduinoStat;
	packet[1] = sensorNum;
	char* temp = nullptr;
	for (int i = 0; i < sensorNum; ++i) {
		temp = new char[2];
		temp[0] = sensors[i].id;
		temp[1] = sensors[i].stat;
		std::cout << "temp[0]:" << (int)temp[0] << " temp[1]:" << (int)temp[1] << std::endl;
		strcat(packet,temp);
		delete temp;
	}
	for (int i = 0; i < sensorNum; ++i) {
		temp = new char[2];
		temp[0] = sensors[i].id;
		temp[1] = sensors[i].reading;
		std::cout << "temp[0]:" << (int)temp[0] << " temp[1]:" << (int)temp[1] << std::endl;
		strcat(packet,temp);
		delete temp;
	}
	return (const unsigned char*) packet;
}

sensorData::~sensorData() {
	delete sensors;
}
