// http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-hc-sr501-motion-sensor-tutorial/
// https://github.com/andrewrapp/xbee-arduino

#include <XBee.h>

XBee xbee = XBee();
ZBRxResponse rx = ZBRxResponse();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  // XBee
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  // LED
  digitalWrite(LED_BUILTIN, LOW);

  flashLed(100);
  delay(500);
  flashLed(1000);
  delay(500);
  flashLed(100);
  delay(500);
}

void loop() {
  xbee.readPacket();
  
    if (xbee.getResponse().isAvailable()) {
      // got something
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
        
        // data should be 01-LED-ON or 01-LED-OFF
        
        uint8_t* payloadData = rx.getData();
        uint8_t payloadDataLength = rx.getDataLength();
        if (payloadDataLength >= 9 && payloadData[7] == 'O' && payloadData[8] == 'N') {
          digitalWrite(LED_BUILTIN, HIGH);
        } else if (payloadDataLength >= 10 && payloadData[7] == 'O' && payloadData[8] == 'F' && payloadData[9] == 'F') {
          digitalWrite(LED_BUILTIN, LOW);
        } else {
          flashLed(100);
          delay(100);
          flashLed(100);
        }
      } else {
        // not something we were expecting
        flashLed(1000);
      }
    } else if (xbee.getResponse().isError()) {
        flashLed(1000);
    }
}

void flashLed(int d) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(d);
  digitalWrite(LED_BUILTIN, LOW);
}

