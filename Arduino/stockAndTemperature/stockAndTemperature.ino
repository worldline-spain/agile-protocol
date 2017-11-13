#include <SoftwareSerial.h>

// Includes
#include <OneWire.h>
#include <DallasTemperature.h>
#include <XBee.h>

// Defines
#define TemperaturePin 2
#define Trigger 5
#define Echo 7  //Echo output

#define SOUND_VEL 0.0343 //sound velocity

#define TEMPERATURE "temperature"
#define PRESENCE "presence"

// Xbee custom (Z4)
//#define XBEE_SH 0x0013A200
//#define XBEE_SL 0x40E7D742

// Xbee concentrator
#define XBEE_SH 0x00000000
#define XBEE_SL 0x00000000

//Constructors
OneWire ourWire(TemperaturePin);
DallasTemperature sensors(&ourWire);
XBee xbee = XBee();

// Global variables
long responseTime;
float distance;

SoftwareSerial sw(2,3);

//Setup -> only executed 1 time
void setup() {
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  sw.begin(9600);
  xbee.setSerial(sw);

  Serial.begin(9600);

  sensors.begin();

}

//Main -> infinite loop;
void loop() {
  distanceSensorMetric();
  //temperatureSensorMetric();

  // Generate metric every second.
  delay(1000);
}

void distanceSensorMetric() {

  // Wake up sensor
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  // Calculate the distancen with the response time.
  responseTime = (pulseIn(Echo, HIGH) / 2);
  distance = float(responseTime * SOUND_VEL);

  // Create distance message
  createDataMessage(distance, PRESENCE);
}

void temperatureSensorMetric() {
  // Get sensor metric
  sensors.requestTemperatures();

  // Create temperature message
  createDataMessage(sensors.getTempCByIndex(0), TEMPERATURE);
}

void createDataMessage(float metricValue, String type) {
  String message = "{\"type\":\""+type+"\",\"data\":{\"date\":\"PUT_DATE\",\"value\":" + metricValue + "} }" + "}";
  Serial.println(message);                   
  sendMessage(message);

}

void sendMessage(String message) {
  // Specify the address of the remote XBee;
  XBeeAddress64 addr64 = XBeeAddress64(XBEE_SH, XBEE_SL);

  // Create a TX Request
  uint8_t dataArray[message.length()+1];
  message.toCharArray(dataArray, message.length()+1);
  ZBTxRequest zbTx = ZBTxRequest(addr64, dataArray, message.length());

  // Send
  xbee.send(zbTx);
}






