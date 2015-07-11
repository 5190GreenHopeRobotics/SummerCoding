/*
 * transmission.h
 *
 *  Created on: Jul 10, 2015
 *      Author: sdai
 */

#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_

#ifndef nullptr
#define nullptr NULL
#endif
class transmissionData {
public:
	virtual const unsigned char* toPacket() = 0;
	virtual ~transmissionData();
};

class transmissionPacket {
protected:
	unsigned char messageType;
	unsigned char messageLength;
	unsigned char* messageData;

public:
	transmissionPacket();
	transmissionPacket(const transmissionPacket& src);
	~transmissionPacket();
	transmissionPacket& operator = (const transmissionPacket& src);
	bool operator==(const transmissionPacket& src);
	void setMessageType(unsigned char t);
	unsigned char getMessageType()const;
	void setData(transmissionData& data);
	unsigned char* toPacket()const;
};

class keepStateData : public transmissionData {
protected:
	unsigned char state;
	unsigned char sequence;
public:
	keepStateData();
	keepStateData(const keepStateData& src);
	keepStateData& operator =(const keepStateData& src);
	bool operator == (const keepStateData& src);
	void setState(const unsigned char state);
	unsigned char getState()const;
	void setSequence(const unsigned char sequence);
	unsigned char getSequence() const;
	const unsigned char* toPacket();
};

class commandData : public transmissionData {
protected:
	unsigned char command;
	unsigned char param;
public:
	commandData();
	commandData(const commandData& src);
	commandData& operator = (const commandData& src);
	bool operator == (const commandData& src);
	void setCommand(const unsigned char command);
	unsigned char getCommand() const;
	void setParameter(const unsigned char param);
	unsigned char getParameter() const;
	const unsigned char* toPacket();
};

typedef struct {
	unsigned char id;
	unsigned char stat;
	unsigned char reading;
} sensorInfo;


class sensorData : public transmissionData {
protected:
	const int block;
	int capacity;
	int current;
	unsigned char arduinoStat;
	unsigned char sensorNum;
	sensorInfo* sensors;
	bool cmpSensorData(const sensorInfo* src,const sensorInfo* data);
	void cpSensorData(sensorInfo* src,const sensorInfo* data);
	void relocate();
public:
	sensorData();
	sensorData(const sensorData& data);
	sensorData& operator= (const sensorData& src);
	bool operator == (const sensorData& src);
	void setArduinoStat(const unsigned char stat);
	unsigned char getArduinoStat() const;
	unsigned char getSensorNum() const;
	void addSensor(const sensorInfo& data);
	const sensorInfo* getSensors() const;
	const unsigned char* toPacket();
	~sensorData();
};

#endif /* TRANSMISSION_H_ */
