#include <I2C.h>
#include <Adafruit_NeoPixel.h>
#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

#define PIN 12
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(9600); 
  I2c.begin(); // Opens & joins the irc bus as master
  delay(100); // Waits to make sure everything is powered up before sending or receiving data  
  I2c.timeOut(50); // Sets a timeout to ensure no locking up of sketch if I2C communication fails
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  int distance = getDistance();
  if(distance == -1) {
    colorWipe(strip.Color(255,0,0),1,60);
  } else {
    Serial.println(distance);
    colorWipe(strip.Color(0,255,17),1,distance/10);
  }

}

void colorWipe(uint32_t c, uint8_t wait, int amount) {
  int i = 0;
  for(i=0; i<amount; i++) {
      strip.setPixelColor(i, c);
  }
  for(i=amount; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();
}

int getDistance() 
{
  // Write 0x04 to register 0x00
  uint8_t nackack = 100; // Setup variable to hold ACK/NACK resopnses  
  while (nackack != 0){
    // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.write(LIDARLite_ADDRESS,RegisterMeasure, MeasureValue); // Write 0x04 to 0x00
    //Serial.println(nackack);
    if(nackack == 32) {
      return -1;
    }
    delay(15); // Wait 15 ms to prevent overpolling
  }
    byte distanceArray[2]; // array to store distance bytes from read function
  
  // Read 2byte distance from register 0x8f
  nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.read(LIDARLite_ADDRESS,RegisterHighLowB, 2, distanceArray); // Read 2 Bytes from LIDAR-Lite Address and store in array
    if(nackack == 32) {
      return -1;
    }
    delay(15); // Wait 15 ms to prevent overpolling
  }
  int distance = (distanceArray[0] << 8) + distanceArray[1];  // Shift high byte [0] 8 to the left and add low byte [1] to create 16-bit int
  //Serial.println(distance);
  return distance;
}

