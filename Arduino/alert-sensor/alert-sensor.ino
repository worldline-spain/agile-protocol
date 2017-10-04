#include <XBee.h>

#define lowestPinLed 2
#define highestPinLed 4
#define buzzer 8

// Constructors
XBee xbee = XBee();

void setup() {
  // put your setup code here, to run once:
  for (int thisPin = lowestPinLed; thisPin <= highestPinLed; thisPin++) {
    pinMode(thisPin, OUTPUT); //initialize thisPin as an output
  }
  pinMode(buzzer, OUTPUT); //initialize buzzer as an output

  // Start the serial port
  Serial.begin(9600);
  // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
  xbee.setSerial(Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  xbee.readPacket(100);
  if (xbee.getResponse().isAvailable()) {

    buzzerAlert();

    ledAlert();
  }
}

void buzzerAlert() {
  for (char i = 0; i < 3; i++) {
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
  }

}

void ledAlert() {
  for (int thisPin = lowestPinLed; thisPin <= highestPinLed; thisPin++)
  {
    digitalWrite(thisPin, HIGH); //turn this led on
    delay(100);//wait for 100 microseconds
  }
  //fade from the highest to the lowest
  for (int thisPin = highestPinLed; thisPin >= lowestPinLed; thisPin--)
  {
    digitalWrite(thisPin, LOW); //turn this led off
    delay(100);//wait for 100 microseconds
  }
  for (int thisPin = highestPinLed; thisPin >= lowestPinLed; thisPin--)
  {
    digitalWrite(thisPin, HIGH); //turn this led on
    delay(100);//wait for 100 microseconds
  }
  for (int thisPin = lowestPinLed; thisPin <= highestPinLed; thisPin++)
  {
    digitalWrite(thisPin, LOW); //turn this led off
    delay(100);//wait for 100 microseconds
  }
}



