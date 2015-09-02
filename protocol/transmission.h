/*
 * transmission.h
 *
 *  Created on: Jul 10, 2015
 *      Author: sdai
 */

#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_
#include "vector.h"
#ifndef nullptr
#define nullptr 0
#endif

/**
 * the transmissionData abstract class
 * The datagram of the transmission protocol, it is encapsulated with in the transmissionPacket
 */
class transmissionData {
public:
	/**
	 * a function that flattens this object to raw byte, user should free the returned pointer to avoid memory leaks
	 * @return the raw byte values
	 */
	virtual const unsigned char* toPacket() = 0;
	/**
	 * an getter that access the length of the datagram
	 * @return the length of the data
	 */
	virtual unsigned char getLength() const = 0;
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
protected:
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
	 * the function that includes the transmission datagram into this object. It also set the message length based on data
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
protected:
	unsigned char state;
	unsigned char sequence[4];
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
	void setSequence(const unsigned char* sequence);
	/**
	 * the getter that gets the sequence
	 * @return the sequence num
	 */
	unsigned char* getSequence() const;
	/**
	 * see super class
	 */
	const unsigned char* toPacket();
	/**
	 * see super class
	 */
	unsigned char getLength() const;
};

/**
 * the commandData datagram
 * the command data datagram is the datagram that includes data of instruction that the roborio send to the arduino
 * @see transmissionData
 */
class commandData: public transmissionData {
protected:
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
	void setParameter(const unsigned char* param);
	/**
	 * gets the parameter field
	 * @return the parameter
	 */
	unsigned char* getParameter() const;
	/**
	 * see super class
	 */
	const unsigned char* toPacket();
	/**
	 * see super class
	 */
	unsigned char getLength() const;
};


/**
 * the sensor data
 * the sensor data which goes in sensorData datagram
 * @see sensorData
 */
struct sensorInfo {
	unsigned char id;
	unsigned char stat;
	unsigned char reading;
	/**
	 * equal operator
	 * @param info the struct to compare
	 * @return result
	 */
	bool operator ==(const sensorInfo& info);
	/**
	 * not equal operator
	 * @param info the struct to compare
	 * @return result
	 */
	bool operator !=(const sensorInfo& info);
};

/**
 * the sensor data datagram
 * the sensor data datagram which the arduino sends to roborio constantly
 * @see transmissionData
 */
class sensorData: public transmissionData {
protected:
	unsigned char arduinoStat;
	unsigned char sensorNum;
	frc5190::vector<sensorInfo> data;
public:
	/**
	 * default ctor
	 */
	sensorData();
	/**
	 * copy ctor
	 * @param src object to copy from
	 */
	sensorData(const sensorData& data);
	/**
	 * assignment operator
	 * @param src object to assign from
	 * @return this
	 */
	sensorData& operator=(const sensorData& src);
	/**
	 * comparison operator
	 * @param src the object to compare
	 * @return result
	 */
	bool operator ==(const sensorData& src);
	/**
	 * sets the status of arduino in the datagram, 0 for error, 1 for good
	 * @param stat the status
	 */
	void setArduinoStat(const unsigned char stat);
	/**
	 * gets the status of arduino
	 * @return the status of arduino
	 */
	unsigned char getArduinoStat() const;
	/**
	 * gets the number of sensor added
	 * @return the number of sensor
	 */
	unsigned char getSensorNum() const;
	/**
	 * adds a sensor data to the datagram
	 * @data the new sensor data
	 */
	void addSensor(const sensorInfo& data);
	/**
	 * gets all the sensor data added
	 * @return sensor data
	 */
	const sensorInfo* getSensors() const;
	/**
	 * see super class
	 */
	const unsigned char* toPacket();
	/**
	 * see super class
	 */
	unsigned char getLength() const;
	/**
	 * free the memory
	 */
	~sensorData();
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
 * To convert from the transmission packet to a sensorData datagram
 * @param tp the received transmission packet
 * @return the interpreted data
 */
static sensorData interpretSensData(const transmissionPacket& tp);
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
	 * convert raw bytes into a transmissionPacket which needs to be processed further, this is first step
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

#endif /* TRANSMISSION_H_ */
