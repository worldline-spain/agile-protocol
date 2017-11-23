#include <XBee.h>
#include <SoftwareSerial.h>
#include <LowPower.h>

const int PIR_PIN = 3;   // HC-S501 (Passive infrared sensor)
const int XBEE_WAKE = 5;
const int XBEE_OUT = 9;
const int XBEE_IN = 7;

// Xbee concentrator
#define XBEE_SH 0x00000000
#define XBEE_SL 0x00000000

XBee xbee = XBee();
int oldPirValue = LOW;  // Old sensor value, to detect changes
int pktCount = 0;       // Single digit counter 0 .. 9

SoftwareSerial sw(XBEE_OUT,XBEE_IN);

int counter =0;

void setup() {
      // HC-SR501 Motion Detector
      pinMode(LED_BUILTIN, OUTPUT);
      pinMode(PIR_PIN, INPUT);
    
      Serial.begin(9600);
    
      // XBee
      sw.begin(9600);
      xbee.setSerial(sw);
    
      // LED
      digitalWrite(LED_BUILTIN, oldPirValue);
}

void loop() {
   if (counter==0) {   
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
      counter=1;
   } else {
    counter--;
 //   LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
    delay(100);
  }
  

}

void sendIRDetection(int value, String type) {
  String message = "{\"type\": \"" + type + "\", \"data\": {\"date\": \"PUT_DATE\", \"value\": \"" + value + "\"}}";
   Serial.println(message);  
  
  XBeeAddress64 addr64 = XBeeAddress64(XBEE_SH, XBEE_SL);

  // Create a TX Request
  uint8_t dataArray[message.length()+1];
  message.toCharArray((char *)dataArray, message.length()+1);
  ZBTxRequest zbTx = ZBTxRequest(addr64, dataArray, message.length());
  
  // wake up the XBee
  //pinMode(XBEE_WAKE, OUTPUT);
  //digitalWrite(XBEE_WAKE, LOW);
  //delay(100);
  // Send
    xbee.send(zbTx);
  // put the XBee to sleep
  //pinMode(XBEE_WAKE, INPUT); // put pin in a high impedence state
  //digitalWrite(XBEE_WAKE, HIGH);

}
