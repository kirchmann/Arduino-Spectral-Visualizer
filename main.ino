int analogPin = A1; // read from multiplexer using analog input 0
int resetPin = 7; // reset is attached to digital pin 3
int strobePin = 8; // strobe is attached to digital pin 2
int spectrumValue[7]; // to hold analog values

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
}
void readSpectrum(int *spectrumArray)
{
 digitalWrite(resetPin, HIGH);
 digitalWrite(resetPin, LOW);
 for (int i = 0; i < 7; i++){
   digitalWrite(strobePin, LOW);
   delayMicroseconds(30); // to allow the output to settle
   spectrumValue[i] = analogRead(analogPin);
   digitalWrite(strobePin, HIGH);
 }
}
void loop()
{
 readSpectrum(&spectrumValue[0]);
  for (int i = 0; i < 7; i++){
   Serial.print(" ");
   Serial.print(spectrumValue[i]);
   digitalWrite(strobePin, HIGH);
 }

 Serial.println();
 delay(200);
}
