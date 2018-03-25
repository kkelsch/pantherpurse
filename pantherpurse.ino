#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>

//
// This code is built off of Adafruit's Sparkle Skirt tutorial
//


// Declare variables
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
#define MOVE_THRESHOLD 1


// LEDs
#define CIRCLE_LED_PIN 6
Adafruit_NeoPixel circleLeds = Adafruit_NeoPixel(16, CIRCLE_LED_PIN, NEO_GRB + NEO_KHZ800);


// purple colors
uint8_t myColors[][3] =         {{200,   0, 200},     // purple
                                 {255,   0,   255},   // magenta 
                                 {138, 43, 226},      // dark violet
                                 {147, 112, 219},     // medium purple
                                 {75, 0, 120},        // indigo
                                 {128, 0, 128}        // blurple
                               };
                               
// don't edit the line below
#define COLORSIZED sizeof(myColors) / 3



void setup() 
{
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  
  if(!accel.begin())
  {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  }

  circleLeds.begin();
  circleLeds.show();
}

void loop() 
{
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
  Serial.print("Accel X: "); Serial.print(event.acceleration.x);  Serial.print(" ");
  Serial.print("Y: "); Serial.print(event.acceleration.y);        Serial.print(" ");
  Serial.print("Z: "); Serial.print(event.acceleration.z);        Serial.print(" ");
 

  double storedVector = event.acceleration.x*event.acceleration.x;
  storedVector += event.acceleration.y*event.acceleration.y;
  storedVector += event.acceleration.z*event.acceleration.z;
  storedVector = sqrt(storedVector);
  Serial.print("Len: "); 
  Serial.println(storedVector);
  
  // wait a bit
  delay(100);
  
  // get new data!
  accel.getEvent(&event);
  double newVector = event.acceleration.x*event.acceleration.x;
  newVector += event.acceleration.y*event.acceleration.y;
  newVector += event.acceleration.z*event.acceleration.z;
  newVector = sqrt(newVector);
  Serial.print("New Len: "); Serial.println(newVector);
  
  // are we moving 
  if (abs(newVector - storedVector) > MOVE_THRESHOLD) {
    Serial.println("Twinkle!");
    flashRandom(9, 1);  // first number is 'wait' delay, shorter num == shorter twinkle
    flashRandom(9, 3);  // second number is how many neopixels to simultaneously light up
    flashRandom(9, 2);
  }
}




void flashRandom(int wait, uint8_t howmany) {
 
  for(uint16_t i=0; i<howmany; i++) {
    // pick a random favorite color!
    int c = random(COLORSIZED);
    int red = myColors[c][0];
    int green = myColors[c][1];
    int blue = myColors[c][2]; 
 
    // get a random pixel from the list
    int j = random(circleLeds.numPixels());
    //Serial.print("Lighting up "); Serial.println(j); 
    
    // now we will 'fade' it in 5 steps
    for (int x=0; x < 5; x++) {
      int r = red * (x+1); r /= 5;
      int g = green * (x+1); g /= 5;
      int b = blue * (x+1); b /= 5;
      
      circleLeds.setPixelColor(j, circleLeds.Color(r, g, b));
      circleLeds.show();
      delay(wait);
    }
    // & fade out in 5 steps
    for (int x=5; x >= 0; x--) {
      int r = red * x; r /= 5;
      int g = green * x; g /= 5;
      int b = blue * x; b /= 5;
      
      circleLeds.setPixelColor(j, circleLeds.Color(r, g, b));
      circleLeds.show();
      delay(wait);
    }
  }
  
}
