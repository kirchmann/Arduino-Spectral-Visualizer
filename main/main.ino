#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
int analogPin = A0;    // read from multiplexer using analog input 0
int resetPin = 7;      // reset is attached to digital pin 3
int strobePin = 8;     // strobe is attached to digital pin 2
#define PIN_LEDSTRIP 9 // Digital signal connected to green wire on LED strip, white wire GND, red wire 5V
long spectrumValue[7]; // to hold analog values
int maxVal = 1023;     // max of all spectrumValue
int potentiometer = A1;// read value from potentiometer to set threshold

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
  pinMode(potentiometer, INPUT);
  digitalWrite(resetPin, HIGH);
  delay(1);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  delay(1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void readSpectrum(long *spectrumArray)
{
  int threshold = map(analogRead(potentiometer), 0,1023,0,100);
  Serial.print("pot: ");
  Serial.print(threshold);
  Serial.println();
  //int threshold = 21;
  digitalWrite(resetPin, HIGH);
  delay(5);
  digitalWrite(resetPin, LOW);
  delayMicroseconds(20);
  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds  (35);
    spectrumValue[i] = analogRead(analogPin);
    spectrumValue[i]=map(spectrumValue[i], 0,1023,0,255);
    if (spectrumValue[i] < threshold){
      spectrumValue[i] = 0;
      }
    digitalWrite(strobePin, HIGH);
  }
}

void updateLEDS(long *spectrumArray, int max )
{
  int i, j;
  int upperLim;
  int maxVal = 80;
  int colorVal = spectrumArray[0];
  upperLim = map(colorVal,0,70,0,10); //after testing realized that I need more sensitivity here 
  for (i = 0; i < upperLim; i++) {
    strip.setPixelColor( i, 255, 0, 255);
  }
  for (i = upperLim; i < 10; i++) {
    strip.setPixelColor( i, 0, 0, 0);
  }
  
  colorVal = spectrumArray[1];
  upperLim = map(colorVal,0,100,0,10);
  for (i = 10; i < 10+upperLim; i++) {
    strip.setPixelColor( i, 0, 20, 150);
  }
  for (i = 10+upperLim; i < 20; i++) {
    strip.setPixelColor( i, 0, 0, 0);
  }
  colorVal = spectrumArray[2];
  upperLim = map(colorVal,0,100,0,10);
  for (i = 20; i < 20+upperLim; i++) {
    strip.setPixelColor( i, 0, 255, 200);
  }
  for (i = 20+upperLim; i < 30; i++) {
    strip.setPixelColor( i, 0, 0, 0);
  }

  colorVal = spectrumArray[3];
  upperLim = map(colorVal,0,100,0,10);
  for (i = 30; i < 30+upperLim; i++) {
    strip.setPixelColor( i, 255, 0, 0);
  }
  for (i = 30+upperLim; i < 40; i++) {
    strip.setPixelColor( i, 0, 0, 0);
  }
  

   colorVal = spectrumArray[4];
  upperLim = map(colorVal,0,100,0,10);
  for (i = 40; i < 40+upperLim; i++) {
    strip.setPixelColor( i, 0, 255, 0);
  }
  for (i = 40+upperLim; i < 50; i++) {
    strip.setPixelColor( i, 0, 0, 0);
  }
    colorVal = spectrumArray[5];
  upperLim = map(colorVal,0,100,0,10);
  for (i = 50; i < 50+upperLim; i++) {
    strip.setPixelColor( i, 0, 0, 255);
  }
  for (i = 50+upperLim; i < 60; i++) {
    strip.setPixelColor( i, 0, 0, 0);
  }

  strip.show();
  delay(1);
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
  updateLEDS(&spectrumValue[0], 700);
  delay(20);
}
