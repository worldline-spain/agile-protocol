// http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-hc-sr501-motion-sensor-tutorial/
// https://github.com/andrewrapp/xbee-arduino

#include <XBee.h>

const int PIR_PIN = 7;   // HC-S501 (Passive infrared sensor)

XBee xbee = XBee();
int oldPirValue = LOW;  // Old sensor value, to detect changes
int pktCount = 0;       // Single digit counter 0 .. 9

void setup() {
  // HC-SR501 Motion Detector
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  // XBee
  Serial.begin(9600);
  xbee.setSerial(Serial);

  // LED
  digitalWrite(LED_BUILTIN, oldPirValue);
}

void loop() {
  int newPirValue; // Place to store read PIR Value

  newPirValue = digitalRead(PIR_PIN);
  digitalWrite(LED_BUILTIN, newPirValue);

  if (oldPirValue == LOW && newPirValue == HIGH) {
    oldPirValue = newPirValue;
    sendIRDetection(newPirValue, "PRESENCE");
    // delay(5000);
  } else if (newPirValue == LOW) {
    oldPirValue = newPirValue;
  }
}

void sendIRDetection(int value, String type) {
  String message = "{\"type\": \"" + type + "\", \"data\": {\"date\": \"PUT_DATE\", \"value\": \"" + value + "\"}}";

  // Specify the address of the remote XBee (this is the SH + SL); use 0 + 0  to address the concentrator
  // XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40e7d69c);
  XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);

  // Create a TX Request
  uint8_t dataArray[message.length()+1];
  message.toCharArray((char *)dataArray, message.length()+1);
  ZBTxRequest zbTx = ZBTxRequest(addr64, dataArray, message.length());

  // Send
  xbee.send(zbTx);
}
