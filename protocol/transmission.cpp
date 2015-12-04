/*
 * transmission.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: sdai
 */
#include "transmission.h"

void endianConverter(unsigned char* data, const int16_t length) {
	for (int16_t i = 0; i < length / 2; ++i) {
		unsigned char buf;
		buf = data[i];
		data[i] = data[length - 1 - i];
		data[length - 1 - i] = buf;
	}
}

int16_t bytesToint16_t(const unsigned char* b, int16_t length) {
	int16_t val = (b[1] << 8) + b[0];
	return val;
}

void int16_tToBytes(const int16_t data, unsigned char* result) {
	int16_t buffer = data;
	int16_t size = sizeof(int16_t);
	unsigned char* resultBuffer = (unsigned char*) &buffer;
	for (int16_t i = 0; i < size; ++i) {
		result[i] = resultBuffer[i];
	}
}

void swap(unsigned char bytes[2]) {
	unsigned char buf = bytes[0];
	bytes[0] = bytes[1];
	bytes[1] = buf;
}

unsigned char* cpyBytes(const unsigned char* bytes, const int16_t length) {
	unsigned char* buf = new unsigned char[length];
	for (int16_t i = 0; i < length; ++i) {
		buf[i] = bytes[i];
	}
	return buf;
}

void cpyBytes(unsigned char* target, const unsigned char* src, const int16_t start,
		const int16_t end) {
	for (int16_t i = start; i < end; ++i) {
		target[i] = src[i];
	}
}

void cpyBytes(unsigned char* target, const unsigned char* src, const int16_t start,
		const int16_t end, const int16_t size) {
	for (int16_t i = start, j = 0; i < end, j < size; ++i, ++j) {
		target[i] = src[j];
	}
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
	for (int16_t i = 0; i < messageLength; i++) {
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
	for (int16_t i = 0; i < messageLength; i++) {
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
	for (int16_t i = 0; i < messageLength; ++i) {
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
		for (int16_t i = 2; i < messageLength + 2; ++i) {
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
	for (int16_t i = 0; i < 2; ++i) {
		sequence[i] = src.sequence[i];
	}
	this->state = src.state;
}

keepStateData& keepStateData::operator =(const keepStateData& src) {
	for (int16_t i = 0; i < 2; ++i) {
		sequence[i] = src.sequence[i];
	}
	this->state = src.state;
	return *this;
}

bool keepStateData::operator ==(const keepStateData& src) {
	bool st = false;
	bool seq = true;
	st = (state == src.getState());
	for (int16_t i = 0; i < 2; ++i) {
		if (sequence[i] != src.sequence[i]) {
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

void keepStateData::setSequence(const int16_t sequence) {
    int16_tToBytes(sequence, this->sequence);
}

const int16_t keepStateData::getSequence() const {
	return bytesToint16_t(sequence, 2);
}

const unsigned char* keepStateData::toPacket() {
	unsigned char* packet = new unsigned char[5];
	packet[0] = state;
	unsigned char* buf = cpyBytes(sequence, 2);
	swap(buf);
	for (int16_t i = 1; i < 3; ++i) {
		packet[i] = buf[i - 1];
	}
	delete[] buf;
	return packet;
}

const int16_t keepStateData::getLength() const {
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
	for (int16_t i = 0; i < 2; ++i) {
		if (param[i] != src.param[i]) {
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

void commandData::setParameter(const int16_t param) {
	unsigned char* buffer = new unsigned char[2];
	int16_tToBytes(param, buffer);
	for (int16_t i = 0; i < 2; ++i) {
		this->param[i] = buffer[i];
	}
	delete[] buffer;
	return;
}

const int16_t commandData::getParameter() const {
	return bytesToint16_t(param, 2);
}

const unsigned char* commandData::toPacket() {
	unsigned char* data = new unsigned char[3];
	data[0] = command;
	unsigned char* buf = cpyBytes(param, 2);
	swap(buf);
	for (int16_t i = 1; i < 3; ++i) {
		data[i] = buf[i - 1];
	}
	delete[] buf;
	return data;
}

const int16_t commandData::getLength() const {
	return 3;
}

//////////////////////////////////////////////////////////////////////
//								interpreters				   		//
//////////////////////////////////////////////////////////////////////

keepStateData interpreter::interpretStatSeq(const transmissionPacket& tp) {
	keepStateData ksd;
	const unsigned char* data = tp.getData();
	unsigned char* buf = cpyBytes(data, 3);
	ksd.setState(buf[0]);
	unsigned char* sequenceBuffer = new unsigned char[2];
	sequenceBuffer[0] = buf[1];
	sequenceBuffer[1] = buf[2];
	swap(sequenceBuffer);
	ksd.setSequence(bytesToint16_t(sequenceBuffer,2));
	delete[] buf;
	delete[] sequenceBuffer;
	return ksd;
}

commandData interpreter::interpretCommandData(const transmissionPacket& tp) {
	commandData data;
	unsigned char buf[2];
	buf[0] = tp.getData()[1];
	buf[1] = tp.getData()[2];
	swap(buf);
	data.setParameter(bytesToint16_t(buf, 2));
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
	for (int16_t i = 0; i < length; ++i) {
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
	int16_t packetSize = 0;
	int16_t bufSize = buf.getSize();
	if (bufSize < 4) {
		return nullptr;
	}
	packetSize = buf[1] + 2;
	if (bufSize < packetSize) {
		return nullptr;
	}
	unsigned char* p = new unsigned char[packetSize];
	for (int16_t i = 0; i < packetSize; ++i) {
		p[i] = buf[i];
	}
	for (int16_t i = packetSize - 1; i > -1; --i) {
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
//////////////////////////////////////////////////////////////////
//							Sensor Info							//
//////////////////////////////////////////////////////////////////

sensorInfo::sensorInfo()
{
    this->id = 0;
    this->stat = 0;
}

sensorInfo::sensorInfo(const sensorInfo& info) {
    copySensorField(info);
}

unsigned char sensorInfo::getDataLength() const {
	return length;
}

void sensorInfo::setDataLength(unsigned char length) {
	this->length = length;
}

unsigned char sensorInfo::getStat() const {
	return stat;
}

void sensorInfo::setStat(unsigned char stat) {
	this->stat = stat;
}

unsigned char sensorInfo::getType() const {
	return type;
}

unsigned char sensorInfo::getId() const {
	return id;
}

void sensorInfo::setId(unsigned char id) {
	this->id = id;
}

void sensorInfo::setType(unsigned char type) {
	this->type = type;
}

const int16_t sensorInfo::getLength() const {
	return 4 + getDataLength();
}

void sensorInfo::copySensorField(const sensorInfo& info) {
    this->id = info.id;
    this->stat = info.stat;
}

sensorInfo& sensorInfo::operator=(const sensorInfo& info) {
    copySensorField(info);
}

const unsigned char* sensorInfo::toPacket() {
	unsigned char dataLength = getDataLength();
	unsigned char* buffer = new unsigned char[getLength()];
	unsigned char* data = getBytes();
	cpyBytes(buffer, data, 4, 40, 36);
	buffer[0] = getType();
	buffer[1] = getId();
	buffer[2] = getStat();
	buffer[3] = getDataLength();
	delete[] data;
	return buffer;

}

//////////////////////////////////////////////////////////////////
//							Sensor Data							//
//////////////////////////////////////////////////////////////////
navXSensor::navXSensor() {
	setType(1);
	setDataLength(36);
	this->altitude = 0;
	this->barometricPressure = 0;
	this->fusedHeading = 0;
	this->linearAccelX = 0;
	this->linearAccelY = 0;
	this->linearAccelZ = 0;
	this->magnetometerX = 0;
	this->magnetometerY = 0;
	this->temperature = 0;
}

navXSensor::navXSensor(const navXSensor& info)
: navXSensor()
{
    copySensorField(info);
}

void navXSensor::copySensorField(const navXSensor& info)
{
    this->altitude = info.altitude;
    this->barometricPressure = info.barometricPressure;
    this->temperature = info.temperature;
    this->fusedHeading = info.fusedHeading;
    this->linearAccelX = info.linearAccelX;
    this->linearAccelY = info.linearAccelY;
    this->linearAccelZ = info.linearAccelZ;
    this->magnetometerX = info.magnetometerX;
    this->magnetometerY = info.magnetometerY;
}

sensorInfo& navXSensor::operator=(const navXSensor& info) {
    copySensorField(info);
}

float navXSensor::getAltitude() const {
	return altitude;
}

void navXSensor::setAltitude(float altitude) {
	this->altitude = altitude;
}

float navXSensor::getBarometricPressure() const {
	return barometricPressure;
}

void navXSensor::setBarometricPressure(float barometricPressure) {
	this->barometricPressure = barometricPressure;
}

float navXSensor::getFusedHeading() const {
	return fusedHeading;
}

void navXSensor::setFusedHeading(float fusedHeading) {
	this->fusedHeading = fusedHeading;
}

float navXSensor::getLinearAccelX() const {
	return linearAccelX;
}

void navXSensor::setLinearAccelX(float linearAccelX) {
	this->linearAccelX = linearAccelX;
}

float navXSensor::getLinearAccelY() const {
	return linearAccelY;
}

void navXSensor::setLinearAccelY(float linearAccelY) {
	this->linearAccelY = linearAccelY;
}

float navXSensor::getMagnetometerX() const {
	return magnetometerX;
}

void navXSensor::setMagnetometerX(float magnetometerX) {
	this->magnetometerX = magnetometerX;
}

float navXSensor::getMagnetometerY() const {
	return magnetometerY;
}

void navXSensor::setMagnetometerY(float magnetometerY) {
	this->magnetometerY = magnetometerY;
}

float navXSensor::getTemperature() const {
	return temperature;
}

float navXSensor::getLinearAccelZ() const {
	return linearAccelZ;
}

void navXSensor::setLinearAccelZ(float linearAccelZ) {
	this->linearAccelZ = linearAccelZ;
}

void navXSensor::setTemperature(float temperature) {
	this->temperature = temperature;
}

void navXSensor::fillAltitude(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->altitude);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 0, 4, 4);
	delete[] converterBuf;
}

void navXSensor::fillBarometricPressure(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->barometricPressure);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 4, 8, 4);
	delete[] converterBuf;
}

void navXSensor::fillFusedHeading(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->fusedHeading);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 8, 12, 4);
	delete[] converterBuf;
}

void navXSensor::fillTemperature(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->temperature);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 12, 16, 4);
	delete[] converterBuf;

}
void navXSensor::fillLinearAccelX(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->linearAccelX);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 16, 20, 4);
	delete[] converterBuf;
}
void navXSensor::fillLinearAccelY(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->linearAccelY);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 20, 24, 4);
	delete[] converterBuf;
}

void navXSensor::fillLinearAccelZ(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->linearAccelZ);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 24, 28, 4);
	delete[] converterBuf;
}

void navXSensor::fillMagnetometerX(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->magnetometerX);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 28, 32, 4);
	delete[] converterBuf;
}

void navXSensor::fillMagnetometerY(unsigned char* buf) {
	unsigned char* converterBuf;
	converterBuf = convertToByte(this->magnetometerY);
	endianConverter(converterBuf, 4);
	cpyBytes(buf, converterBuf, 32, 36, 4);
	delete[] converterBuf;
}

unsigned char* navXSensor::getBytes() {
	unsigned char* buffer = new unsigned char[getDataLength()];
	fillAltitude(buffer);

	fillBarometricPressure(buffer);

	fillFusedHeading(buffer);

	fillTemperature(buffer);

	fillLinearAccelX(buffer);

	fillLinearAccelY(buffer);

	fillLinearAccelZ(buffer);

	fillMagnetometerX(buffer);

	fillMagnetometerY(buffer);

	return buffer;
}

basicEncoder::basicEncoder() {
	setType(2);
	setDataLength(5);
	this->counts = 0;
	this->direction = 0;
}

long basicEncoder::getCounts() const {
	return counts;
}

void basicEncoder::setCounts(long counts) {
	this->counts = counts;
}

unsigned char basicEncoder::getDirection() const {
	return direction;
}

void basicEncoder::setDirection(unsigned char direction) {
	this->direction = direction;
}

unsigned char* basicEncoder::getBytes() {
	unsigned char* buffer = new unsigned char[getDataLength()];
	unsigned char* converter = convertToByte(this->counts);
	endianConverter(converter, 4);
	cpyBytes(buffer, converter, 0, 4);
	delete[] converter;
	buffer[4] = direction;
	return buffer;
}

basicPotentiometer::basicPotentiometer() {
	setType(3);
	setDataLength(4);
	this->angle = 0;
}

float basicPotentiometer::getAngle() const {
	return angle;
}

void basicPotentiometer::setAngle(float angle) {
	this->angle = angle;
}

unsigned char* basicPotentiometer::getBytes() {
	unsigned char* buf = convertToByte(this->angle);
	endianConverter(buf, 4);
	return buf;
}

basicDistance::basicDistance() {
	setType(4);
	setDataLength(4);
	this->distance = 0;
}
float basicDistance::getDistance() const {
	return distance;
}

void basicDistance::setDistance(float distance) {
	this->distance = distance;
}

unsigned char* basicDistance::getBytes() {
	unsigned char* buffer = convertToByte(this->distance);
	endianConverter(buffer, 4);
	return buffer;
}

switchSensor::switchSensor() {
	setType(5);
	setDataLength(1);
	this->switchValue = 0;
}

unsigned char switchSensor::getSwitchValue() const {
	return switchValue;
}

void switchSensor::setSwitchValue(unsigned char switchValue) {
	this->switchValue = switchValue;
}

unsigned char* switchSensor::getBytes() {
	unsigned char* buffer = new unsigned char[1];
	buffer[0] = this->switchValue;
	return buffer;
}
