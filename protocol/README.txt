The transmission protocol

basic package:

+--------------------------+------------------------+-------------------------------------------------------+
|Message Type(1 Byte)      |Message Length(1Byte)	| Message Data(any number of bytes, depends on the data)|
+--------------------------+------------------------+-------------------------------------------------------+

Message Length = the length of message data

Data type:
Keep State Data (1):
+-----------------------+-----------------------+
|Status (1Byte)			|sequence (2 Byte int)	|
+-----------------------+-----------------------+
If roborio still on,
Status = 2

Otherwise,
Status = 1

Sequence:
Increment by 1 every package.
Once reached max of 2 byte, reset to 0.


Command Data (2):

+---------------------+-------------------------+
|Command Id(1 Byte)   |Parameter (2 Byte)		|
+---------------------+-------------------------+

Sensor Data (3):

+---------------------+-------------------------+
|Sensor Type(1Byte)   |Sensor Id(1Byte)			|
+---------------------+-------------------------+
|Sensor Status(1Byte) |Data Length(1Byte)		|
+---------------------+-------------------------+
|Data ($(Data Length) Byte))					|
+-----------------------------------------------+

Sensor Type is the type of the sensor, defined later,
Sensor Id is the identifier for a sensor under the type:
	in situation with multiple sensor of the same type (example: 2 encoder),
	the sensor id is used to identify the exact sensor.
Sensor Status is the status of the sensor:
	1 - operational
	2 - error occurred
Data Length is the length of the data

Specifics that goes into data segment:

NavX Sensor:

Sensor Type = 1;

The Data Segment:

In the order follow:

-altitude: 4 byte float
-barometricPressure: 4 byte float
-fusedHeading: 4 byte float
-temperature: 4 byte float
-linear acceleration X: 4 byte float
-linear acceleration Y: 4 byte float
-linear acceleration Z: 4 byte float
-magnetometer X: 4 Byte float
-magnetometer Y: 4 Byte float

Encoder:

Sensor Type = 2;

The Data Segment:

In the order follow:

-encoder count: 4 Byte integer value
-encoder direction: 1 Byte value

Potentiometer:

Sensor Type = 3;

The Data Segment:

In the order follow:

-angle: 4 Byte float

A distance measuring sensor:

Sensor Type = 4;

The Data Segment:

In the order follow:

-distance: 4 byte float

A Switch Type Sensor:

Sensor Type = 5;

The Data Segment:

In the order follow:

-switchValue: 1 Byte value : 1 for on, and 2 for off.
