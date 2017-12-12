#include <SoftwareSerial.h>
#include <XBee.h>


#define buzzer 12

// Constructors
XBee xbee = XBee();
SoftwareSerial sw(10,9);
void setup() {
   sw.begin(9600);

  pinMode(buzzer, OUTPUT); //initialize buzzer as an output
  // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
  xbee.setSerial(sw);
  buzzerAlert();
}

void loop() {
  // put your main code here, to run repeatedly:
  xbee.readPacket(100);
  
  if (xbee.getResponse().isAvailable()) {
    buzzerAlert();
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




