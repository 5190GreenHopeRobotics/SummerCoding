#include <transmission.h>
#include <vector.h>
#include <binarySearchTree.hh>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            12
#define NUMPIXELS 60

namespace light {
  Adafruit_NeoPixel pixels;
  enum color { 
    RED=1, GREEN=2, ORANGE=3, BLACK=4   }; 

  void burnLedSetup(const int pin)
  {
    pixels = Adafruit_NeoPixel(NUMPIXELS, pin, NEO_GRB + NEO_KHZ800);
    pixels.begin(); // This initializes the NeoPixel library.
    pixels.show();

  }
  void burnLedColor(const int ledNum, const color colorWanted){
    if(colorWanted==RED){
      pixels.setPixelColor(ledNum,pixels.Color(255,0,0));
    }
    if(colorWanted==GREEN){
      pixels.setPixelColor(ledNum,pixels.Color(0,255,0));
    }
    if(colorWanted==ORANGE){
      pixels.setPixelColor(ledNum,pixels.Color(255,165,0));
    }
    if(colorWanted==BLACK){
      pixels.setPixelColor(ledNum,pixels.Color(0,0,0));
    }
    pixels.show();
  }
}


class sensor {
public:
  virtual unsigned char getStatus() = 0;
  virtual int getPeriod() = 0;
  virtual transmissionPacket generatePacket() = 0;
  virtual int getType() =0;
  virtual int getId() = 0;
};

class sensorManager {
public:
  sensorManager() {
    currentSeq = 1; 
  }
  void addSensor(sensor* toAdd) {
    buffer.append(toAdd); 
  }
  void update() {
    if(isPeriod(500)) {
      transmissionPacket keepAlive;
      keepAlive.setMessageType(1);
      keepStateData ksd;
      ksd.setState(1);
      ksd.setSequence(currentSeq);
      keepAlive.setData(ksd);
      unsigned char* alivePacket = keepAlive.toPacket();
      for(int i =0; i<keepAlive.getLength() + 2; ++i) {
        Serial.write(alivePacket[i]); 
      }
      delete[] alivePacket;
      currentSeq += 1;
    }
    for(sensor** i = buffer.begin();i != buffer.end(); i++) {
      int period = (*i)->getPeriod();
      unsigned char stat = (*i)->getStatus();
      if(stat == 2) {
        burnLedColor(calculateLed(**i), light::RED);
      } 
      else if(stat == 1) {
        burnLedColor(calculateLed(**i),light::GREEN); 
      } 
      else {
        burnLedColor(calculateLed(**i),light::ORANGE); 
      }
      if(isPeriod(period)) {
        transmissionPacket tp = (*i)->generatePacket();
        unsigned char* packet = tp.toPacket();
        for(int j = 0;j < tp.getLength() + 2;++j) {
          Serial.write(packet[j]);
        }
        delete[] packet; 
      }
    }

  }

  ~sensorManager() {
    for(sensor **i = buffer.begin(); i!= buffer.end(); i++) {
      delete *i;
    } 
  }
private:
  boolean isPeriod(int period) {
    long currentTime =millis();
    long last = periodTracker[period];
    if(currentTime - last > period) {
      periodTracker[period] = currentTime;
      return true; 
    }
    return false;
  }
  int calculateLed(sensor& toCalc) {
    int result = toCalc.getType() * 10;
    result += toCalc.getId();
    return result; 
  }
  frc5190::vector<sensor*> buffer;
  binarySearchTree<int,long> periodTracker;
  int currentSeq;
};

class failTest : public sensor {
  virtual unsigned char getStatus() {
    return 2;
  }
  virtual int getPeriod() {
    return 1000;
  }
  virtual transmissionPacket generatePacket() {

    return transmissionPacket();
  }
  virtual int getType() {
    return 1;
  }
  virtual int getId() {
    return 0;
  }
};

sensorManager manager;

void setup() {
  light::burnLedSetup(12);
  Serial.begin(9600);
  manager.addSensor(new failTest());
}

void loop() {
  manager.update();
}



