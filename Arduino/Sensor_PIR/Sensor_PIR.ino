// http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-hc-sr501-motion-sensor-tutorial/
// https://github.com/andrewrapp/xbee-arduino

#include <XBee.h>

const int ledPin = 13;  // LED on Pin 13 of Arduino
const int pirPin = 7;   // HC-S501 (Passive infrared sensor)

XBee xbee = XBee();
uint8_t sensor01_payload[] = { '0','1','-', 'P','I','R','-','O','N','-','0' }; // SensorNum - SensorType - SensorVal - Counter (01-PIR-ON-0)
int oldPirValue = LOW;  // Old sensor value, to detect changes
int pktCount = 0;       // Single digit counter 0 .. 9

void setup() {
  // HC-SR501 Motion Detector
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  // XBee
  Serial.begin(9600);
  xbee.setSerial(Serial);

  // LED
  digitalWrite(ledPin, oldPirValue);
}

void loop() {
  int tmpPirValue; // Place to store read PIR Value

  tmpPirValue = digitalRead(pirPin);
  digitalWrite(ledPin, tmpPirValue);

  if (oldPirValue == LOW && tmpPirValue == HIGH) {
    digitalWrite(ledPin, tmpPirValue);
    oldPirValue = tmpPirValue;
    sendIRDetection();
    delay(5000);
  } else if (tmpPirValue == LOW) {
    digitalWrite(ledPin, tmpPirValue);
    oldPirValue = tmpPirValue;
  }
}

void sendIRDetection() {
  // Specify the address of the remote XBee (this is the SH + SL); use 0 + 0  to address the concentrator
  // XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40e7d69c);
  XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);

  // Create a TX Request
  sensor01_payload[sizeof(sensor01_payload)-1] = pktCount +  '0';
  ++pktCount %= 10;
  ZBTxRequest zbTx = ZBTxRequest(addr64, sensor01_payload, sizeof(sensor01_payload));    

  // Send your request
  xbee.send(zbTx);
}
