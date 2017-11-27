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
boolean alarmState=false;
SoftwareSerial sw(XBEE_OUT,XBEE_IN);

void blink() 
{  
  alarmState=true;

}

void setup() {
    
//      pinMode(XBEE_WAKE, INPUT); // put pin in a high impedence state
//      digitalWrite(XBEE_WAKE, HIGH);
      
      pinMode(PIR_PIN, INPUT); 
      attachInterrupt(digitalPinToInterrupt(PIR_PIN), blink, RISING );  
      Serial.begin(9600);
      // XBee
      sw.begin(9600);
      xbee.setSerial(sw);

}

void loop() {
  if (alarmState) {
      Serial.println("WAKED UP!!!!");
      sendIRDetection(1, "PRESENCE");
      alarmState=false;
  } else {
     Serial.println("SLEEEPPP");
     Serial.flush();
     LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  } 
}


void sendIRDetection(int value, String type) {
  String message = "{\"type\": \"" + type + "\", \"data\": {\"date\": \"PUT_DATE\", \"value\": \"" + value + "\"}}";
    
  
  XBeeAddress64 addr64 = XBeeAddress64(XBEE_SH, XBEE_SL);

  // Create a TX Request
  uint8_t dataArray[message.length()+1];
  message.toCharArray((char *)dataArray, message.length()+1);
  ZBTxRequest zbTx = ZBTxRequest(addr64, dataArray, message.length());
  
  // wake up the XBee
  pinMode(XBEE_WAKE, OUTPUT);
  digitalWrite(XBEE_WAKE, LOW);
  delay(1000);
  // Send
  xbee.send(zbTx);
 
//  delay(1000);
  // put the XBee to sleep
  pinMode(XBEE_WAKE, INPUT); // put pin in a high impedence state
  digitalWrite(XBEE_WAKE, HIGH);
  Serial.println(message); 
}
