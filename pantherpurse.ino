#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>


// Declare variables
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
#define MOVE_THRESHOLD 1


#define LED_PIN      6   // which pin your pixels are connected to
#define NUM_LEDS    16   // how many LEDs you have
#define BRIGHTNESS 100   // 0-255, higher number is brighter. 
#define SATURATION 255   // 0-255, 0 is pure white, 255 is fully saturated color
#define SPEED       500   // How fast the colors move.  Higher numbers = faster motion
#define STEPS        10   // How wide the bands of color are.  1 = more like a gradient, 10 = more like stripes
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;


// palattes
const TProgmemPalette16 LighterPurpleColors_p PROGMEM =
{
  CRGB::Purple,
  CRGB::DarkMagenta,
  CRGB::Violet ,
  CRGB::Purple,

  CRGB::Purple,
  CRGB::DarkMagenta,
  CRGB::Purple,
  CRGB::Violet ,

  CRGB::DarkMagenta,
  CRGB::Purple,
  CRGB::Plum ,
  CRGB::DarkMagenta,

  CRGB::Purple,
  CRGB::Violet ,
  CRGB::DarkMagenta ,
  CRGB::Black
};


void setup()
{

  // Setup LEDs
  delay( 2000 ); // power-up safety delay
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  0 );
  currentBlending = LINEARBLEND;
  FastLED.clear();
  FastLED.show();


#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);

  if (!accel.begin())
  {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  }

}

void loop()
{

  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);
  Serial.print("Accel X: "); Serial.print(event.acceleration.x);  Serial.print(" ");
  Serial.print("Y: "); Serial.print(event.acceleration.y);        Serial.print(" ");
  Serial.print("Z: "); Serial.print(event.acceleration.z);        Serial.print(" ");

  double storedVector = event.acceleration.x * event.acceleration.x;
  storedVector += event.acceleration.y * event.acceleration.y;
  storedVector += event.acceleration.z * event.acceleration.z;
  storedVector = sqrt(storedVector);
  Serial.print("Len: ");
  Serial.println(storedVector);

  // wait a bit
  delay(100);

  // get new data!
  accel.getEvent(&event);
  double newVector = event.acceleration.x * event.acceleration.x;
  newVector += event.acceleration.y * event.acceleration.y;
  newVector += event.acceleration.z * event.acceleration.z;
  newVector = sqrt(newVector);
  Serial.print("New Len: "); Serial.println(newVector);

  // are we moving
  if (abs(newVector - storedVector) > MOVE_THRESHOLD) {
    Serial.println("Show Colors!");
    currentPalette = LighterPurpleColors_p;
    swipeColors(10);
    FastLED.clear();
  }
  FastLED.clear();
}


void swipeColors(uint8_t interations) {
  FastLED.setBrightness(0);
  FillLEDsFromPaletteColors(0);
  fadeIn();

  for (int i = 0; i <= NUM_LEDS * interations; i++) {

    FillLEDsFromPaletteColors(i);
    FastLED.show();
    FastLED.delay(1000 / SPEED);
  }

  FastLED.setBrightness(0);
  FastLED.clear();
  FastLED.show();
}



void fadeIn(){
 for(int i=0; i<= BRIGHTNESS; i++){
    FastLED.setBrightness(i);
    FastLED.show();
    delay(9);
 }
 
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += STEPS;
  }
}


//void  fadeUp ()
//{
//
//      for ( int i = 0; i < NUM_LEDS; i++ )
//          {
//            leds[i] = CRGB::Orange; // Can be any colour
//            leds[i].maximizeBrightness(FastLED_fade_counter);  // 'FastLED_fade_counter' How high we want to fade up to 255 = maximum.
//          }
//
//            FastLED.show();
//            FastLED_fade_counter ++ ;   // Increment
//
//}




