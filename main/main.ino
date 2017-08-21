#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
int analogPin = A0;    // read from multiplexer using analog input 0
int resetPin = 7;      // reset is attached to digital pin 3
int strobePin = 8;     // strobe is attached to digital pin 2
#define PIN_LEDSTRIP 9 // Digital signal connected to green wire on LED strip, white wire GND, red wire 5V
long spectrumValue[7];  // to hold analog values
int maxVal = 1023;     // max of all spectrumValue

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN_LEDSTRIP, NEO_GRB + NEO_KHZ400);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
// Funkar med signalgenerator från mobilen. 
//http://www.szynalski.com/tone-generator/
void setup()
{
 Serial.begin(9600);
 pinMode(analogPin, INPUT);
 pinMode(strobePin, OUTPUT);
 pinMode(resetPin, OUTPUT);
 digitalWrite(resetPin, HIGH);
 digitalWrite(resetPin, LOW);
 digitalWrite(strobePin, HIGH);
 strip.begin();
 strip.show(); // Initialize all pixels to 'off'
}

void readSpectrum(long *spectrumArray)
{
 digitalWrite(resetPin, HIGH);
 digitalWrite(resetPin, LOW);
 delayMicroseconds(20);
 for (int i = 0; i < 7; i++){
   digitalWrite(strobePin, LOW);
   delayMicroseconds(30); // to allow the output to settle
   spectrumValue[i] = analogRead(analogPin);
   digitalWrite(strobePin, HIGH);
   delayMicroseconds(20);
 }
}

void updateLEDS(long *spectrumArray,int max )
{
  int i,j;
  int colorVal = 250*spectrumArray[0]/max;
  if (colorVal < 50)  {
    colorVal = 0;
    }
  Serial.print("Val: ");
  Serial.print(colorVal);
  Serial.print(" ");
  for (i = 0; i < 9; i++) {
    strip.setPixelColor( i, colorVal, colorVal, 0);
  }
  colorVal = 250*spectrumArray[1]/max;
  if (colorVal < 50)  {
    colorVal = 0;
    }
  Serial.print(colorVal);
  Serial.print(" ");
  
  for (i = 9; i < 18; i++) {
    strip.setPixelColor( i, 0, colorVal, colorVal);
  }
  colorVal = 250*spectrumArray[2]/max;
  if (colorVal < 50)  {
    colorVal = 0;
    }
  Serial.print(colorVal);
  Serial.print(" ");
  
  for (i = 18; i < 26; i++) {
    strip.setPixelColor( i, colorVal, colorVal, 0);
  }
  colorVal = 250*spectrumArray[3]/max;
  Serial.print(colorVal);
  Serial.print(" ");
  if (colorVal < 50)  {
    colorVal = 0;
    }
  for (i = 26; i < 34; i++) {
    strip.setPixelColor( i, colorVal, 0, colorVal);
  }
  colorVal = 250*spectrumArray[4]/max;
  if (colorVal < 50)  {
    colorVal = 0;
    }
  Serial.print(colorVal);
  Serial.print(" ");
  for (i = 34; i < 42; i++) {
    strip.setPixelColor( i, 0, colorVal, 0);
  }
  colorVal = 250*spectrumArray[5]/max;
  if (colorVal < 50)  {
    colorVal = 0;
    }
  Serial.print(colorVal);
  Serial.print(" ");
  for (i = 42; i < 51; i++) {
    strip.setPixelColor( i, colorVal, 0, colorVal);
  }
  colorVal = 250*spectrumArray[5]/max;
  if (colorVal < 50)  {
    colorVal = 0;
    }
  Serial.print(colorVal);
  Serial.print(" ");
  for (i = 51; i < 60; i++) {
    strip.setPixelColor( i, 0, colorVal, colorVal);
  }
  strip.show();
  delay(5);
}

void loop()
{
  readSpectrum(&spectrumValue[0]);
  for (int i = 0; i < 7; i++)
  {
    Serial.print(" ");
    Serial.print(spectrumValue[i]);   
  }
 
  Serial.println();
  // determine intensity of sound, check for max?
  updateLEDS(&spectrumValue[0],1023);
  delay(50);
}
