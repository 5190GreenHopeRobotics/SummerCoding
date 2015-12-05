/*
 * transmission.h
 *
 *  Created on: Jul 10, 2015
 *      Author: sdai
 */

#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_
#include "vector.h"
#ifdef COMPUTER
#include <cstdint>
#endif // COMPUTER
#ifndef nullptr
#define nullptr 0
#endif

#ifndef int16_t
#define int16_t int
#endif // int16_t

int16_t bytesToint16_t(const unsigned char* data, int16_t length);
void int16_tToBytes(const int16_t data, unsigned char* result);
void swap(unsigned char bytes[2]);
template<typename T>
unsigned char* convertToByte(T& data);

template<typename T>
T genericByteToType(unsigned char* data, T& type);

unsigned char* cpyBytes(const unsigned char* bytes, const int16_t length);
void cpyBytes(unsigned char* target, const unsigned char* src, const int16_t start,
		const int16_t end);
void cpyBytes(unsigned char* target, const unsigned char* src,
		const int16_t start, const int16_t end, const int16_t size);
void endianConverter(unsigned char* data, const int16_t length);
/**
 * the transmissionData abstract class
 * The datagram of the transmission protocol, it is encapsulated with in the transmissionPacket
 */
class transmissionData {
public:
	/**
	 * a function that flattens this object to raw byte, user should free the returned point16_ter to avoid memory leaks
	 * @return the raw byte values
	 */
	virtual const unsigned char* toPacket() = 0;
	/**
	 * an getter that access the length of the datagram
	 * @return the length of the data
	 */
	virtual const int16_t getLength() const = 0;
	/**
	 * does nothing
	 */
	virtual ~transmissionData();
};

/**
 * The transmission meta packet
 * The meta packet to be send that includes transmissionData
 */
class transmissionPacket {
private:
	unsigned char messageType;
	unsigned char messageLength;
	unsigned char* messageData;

public:
	/**
	 * default constructor, init members
	 */
	transmissionPacket();
	/**
	 * copy constructor
	 * @param src the object to copy from
	 */
	transmissionPacket(const transmissionPacket& src);
	/**
	 * frees the memory
	 */
	~transmissionPacket();
	/**
	 * assignment operator that clear this object, and copy over data from src
	 * @param src the data to copy
	 * @return this
	 */
	transmissionPacket& operator =(const transmissionPacket& src);
	/**
	 * setter that set the message type
	 * @param t the value to set
	 */
	void setMessageType(const unsigned char t);
	/**
	 * the getter that returns the message type
	 * @return message type
	 */
	unsigned char getMessageType() const;
	/**
	 * the function that includes the transmission datagram int16_to this object. It also set the message length based on data
	 * @param data the data to include
	 *
	 */
	void setData(transmissionData& data);
	/**
	 * the setter that set the data length, use setData if possible
	 * @see setData(transmissionData& data)
	 * @param length the length(of data) to set
	 */
	void setMessageLength(const unsigned char length);
	/**
	 * the getter that returns the message length(only the data portion), plus 2 to get the whole length
	 * @return the length of data
	 */
	unsigned char getLength() const;
	/**
	 * set the raw byte data, use the setData with transmissionData if possible. Set the data length manually.
	 * @see setMessageLength(const unsigned char length)
	 * @see setData(transmissionData& data)
	 * @param data the raw byte to set
	 */
	void setData(const unsigned char* data);
	/**
	 * getter that returns the raw byte data portion
	 * @return the raw byte datagram
	 */
	const unsigned char* getData() const;
	/**
	 * generates the packet as bytes, free them after using them to avoid leak.
	 * @return the raw bytes packet
	 */
	unsigned char* toPacket() const;
};

/**
 * the keepStateData datagram
 * the keepStateData datagram is the keep-alive message sent from the roborio to the arduino
 * @see transmissionData
 */
class keepStateData: public transmissionData {
private:
	unsigned char state;
	unsigned char sequence[2];
public:
	/**
	 * default ctor
	 */
	keepStateData();
	/**
	 * copy ctor
	 * @param src object to copy from
	 */
	keepStateData(const keepStateData& src);
	/**
	 * assignment operator
	 * @param src object to assign from
	 * @return this
	 */
	keepStateData& operator =(const keepStateData& src);
	/**
	 * comparison operator
	 * @param src the object to compare
	 * @return result
	 */
	bool operator ==(const keepStateData& src);
	/**
	 * set the state field of the datagram, 0 for auto, 1 for tele, 2 for test
	 * @param state the state
	 *
	 */
	void setState(const unsigned char state);
	/**
	 * the getter that gets the state
	 * @return the state
	 */
	unsigned char getState() const;
	/**
	 * the setter that sets the sequence which should with each packet
	 * @param sequence the sequence num
	 */
	void setSequence(const int16_t sequence);
	/**
	 * the getter that gets the sequence
	 * @return the sequence num
	 */
	const int16_t getSequence() const;

	/**
	 * see super class
	 */
	const unsigned char* toPacket();
	/**
	 * see super class
	 */
	const int16_t getLength() const;
};

/**
 * the commandData datagram
 * the command data datagram is the datagram that includes data of instruction that the roborio send to the arduino
 * @see transmissionData
 */
class commandData: public transmissionData {
private:
	unsigned char command;
	unsigned char param[2];
public:
	/**
	 * default ctor
	 */
	commandData();
	/**
	 * copy ctor
	 * @param src object to copy from
	 */
	commandData(const commandData& src);
	/**
	 * assignment operator
	 * @param src object to assign from
	 * @return this
	 */
	commandData& operator =(const commandData& src);
	/**
	 * comparison operator
	 * @param src the object to compare
	 * @return result
	 */
	bool operator ==(const commandData& src);
	/**
	 * sets the command field, which identifies the command.
	 * @param command the command id
	 */
	void setCommand(const unsigned char command);
	/**
	 * gets the command field
	 * @return the command id
	 */
	unsigned char getCommand() const;
	/**
	 * sets the parameter for the command, set to 0 if none
	 * @param param the parameter
	 */
	void setParameter(const int16_t param);
	/**
	 * gets the parameter field
	 * @return the parameter
	 */
	const int16_t getParameter() const;
	/**
	 * see super class
	 */
	const unsigned char* toPacket();
	/**
	 * see super class
	 */
	const int16_t getLength() const;
};

class sensorInfo: public transmissionData {
private:
	unsigned char type;
	unsigned char id;
	unsigned char stat;
	unsigned char length;
protected:
    void copySensorField(const sensorInfo& info);
	unsigned char getDataLength() const;
	void setDataLength(unsigned char length);
	unsigned char getType() const;
	void setType(unsigned char type);
	virtual unsigned char* getBytes() = 0;
public:
    sensorInfo();
    sensorInfo(const sensorInfo& info);
    sensorInfo& operator=(const sensorInfo& info);
	unsigned char getStat() const;
	void setStat(unsigned char stat);
	unsigned char getId() const;
	void setId(unsigned char id);
	const unsigned char* toPacket();
	const int16_t getLength() const;
};

class navXSensor: public sensorInfo {
private:
	float linearAccelX;
	float linearAccelY;
	float linearAccelZ;
	float barometricPressure;
	float altitude;
	float fusedHeading;
	float temperature;
	float magnetometerX;
	float magnetometerY;

	void fillLinearAccelX(unsigned char* buf);
	void fillLinearAccelY(unsigned char* buf);
	void fillLinearAccelZ(unsigned char* buf);
	void fillBarometricPressure(unsigned char* buf);
	void fillAltitude(unsigned char* buf);
	void fillFusedHeading(unsigned char* buf);
	void fillTemperature(unsigned char* buf);
	void fillMagnetometerX(unsigned char* buf);
	void fillMagnetometerY(unsigned char* buf);
	void copySensorField(const navXSensor& info);
protected:
	unsigned char* getBytes();
public:
	navXSensor();
	navXSensor(const navXSensor& info);
	navXSensor& operator=(const navXSensor& info);
	float getAltitude() const;
	void setAltitude(float altitude);
	float getBarometricPressure() const;
	void setBarometricPressure(float barometricPressure);
	float getFusedHeading() const;
	void setFusedHeading(float fusedHeading);
	float getLinearAccelX() const;
	void setLinearAccelX(float linearAccelX);
	float getLinearAccelY() const;
	void setLinearAccelY(float linearAccelY);
	float getMagnetometerX() const;
	void setMagnetometerX(float magnetometerX);
	float getMagnetometerY() const;
	void setMagnetometerY(float magnetometerY);
	float getTemperature() const;
	void setTemperature(float temperature);
	float getLinearAccelZ() const;
	void setLinearAccelZ(float linearAccelZ);
};

class basicEncoder: public sensorInfo {
private:
	long counts;
	unsigned char direction;
protected:
	unsigned char* getBytes();
	void copySensorField(const basicEncoder& info);
public:
    basicEncoder(const basicEncoder& src);
	basicEncoder();
	basicEncoder& operator=(const basicEncoder& src);
	long getCounts() const;
	void setCounts(long counts);
	unsigned char getDirection() const;
	void setDirection(unsigned char direction);
};

class basicPotentiometer: public sensorInfo {
private:
	float angle;
protected:
	unsigned char* getBytes();
	void copySensorField(const basicPotentiometer& src);
public:
    basicPotentiometer(const basicPotentiometer& src);
	basicPotentiometer();
	basicPotentiometer& operator=(const basicPotentiometer& src);
	float getAngle() const;
	void setAngle(float angle);
};

class basicDistance: public sensorInfo {
private:
	float distance;
protected:
	unsigned char* getBytes();
	void copySensorField(const basicDistance& src);
public:
	basicDistance();
	basicDistance(const basicDistance& src);
	basicDistance& operator=(const basicDistance& src);
	float getDistance() const;
	void setDistance(float distance);
};

class switchSensor: public sensorInfo {
private:
	unsigned char switchValue;
protected:
	unsigned char* getBytes();
	void copySensorField(const switchSensor& src);
public:
    switchSensor(const switchSensor& src);
	switchSensor();
	switchSensor& operator=(const switchSensor& src);
	unsigned char getSwitchValue() const;
	void setSwitchValue(unsigned char switchValue);
};

class interpreter {
public:
	/**
	 * To convert from the transmission packet to a keepState datagram,
	 * @param tp the received transmission packet
	 * @return the interpreted data
	 */
	static keepStateData interpretStatSeq(const transmissionPacket& tp);
	/**
	 * To convert from the transmission packet to a commandData
	 * @param tp the received transmission packet
	 * @return the interpreted data
	 */
	static commandData interpretCommandData(const transmissionPacket& tp);
};

class packetBuffer {
protected:
	frc5190::vector<unsigned char> buf;
	/**
	 * convert raw bytes int16_to a transmissionPacket which needs to be processed further, this is first step
	 * @param packet the raw bytes
	 * @return converted object
	 */
	transmissionPacket interpretRawData(const unsigned char* packet);
	unsigned char* getNext();
public:
	/**
	 * add a byte to the buffer
	 * @param b the byte
	 */
	void addByte(const unsigned char b);
	/**
	 * convert the bytes in the buffer to transmissionPacket
	 * @return converted packets
	 */
	frc5190::vector<transmissionPacket> getPackets();
};

template<typename T>
unsigned char* convertToByte(T& data) {
	unsigned char* buf = new unsigned char[sizeof(data)];
	union {
		T d;
		unsigned char array[sizeof(data)];
	} converter;
	converter.d = data;
	for (int16_t i = 0; i < sizeof(data); ++i) {
		buf[i] = converter.array[i];
	}
	return buf;
}

template<typename T>
T genericByteToType(unsigned char* data, T& type) {
    union {
        T result;
        unsigned char array[sizeof(type)];
    } converter;
    for(int i=0;i<sizeof(type);++i) {
        converter.array[i] = data[i];
    }
    return converter.result;
}
#endif /* TRANSMISSION_H_ */
