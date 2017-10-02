// Includes
#include <OneWire.h>
#include <DallasTemperature.h>
#include <XBee.h>

// Defines
#define Pin 2
#define Trigger 7
#define Input 8  //Echo output

#define SOUND_VEL 0.0343 //sound velocity

#define TEMPERATURE "temperature"
#define PRESENCE "presence"

// Xbee custom (Z4)
#define XBEE_SH 0x0013A200
#define XBEE_SL 0x40E7D742

// Xbee concentrator
//#define XBEE_SH 0x00000000
//#define XBEE_SL 0x00000000

//Constructors
OneWire ourWire(Pin);
DallasTemperature sensors(&ourWire);
XBee xbee = XBee();

// Global variables
long responseTime;
float distance;

//Setup -> only executed 1 time
void setup() {
  pinMode(Trigger, OUTPUT);
  pinMode(Input, INPUT);

  xbee.setSerial(Serial);

  Serial.begin(9600);

  sensors.begin();

}

//Main -> infinite loop;
void loop() {
  distanceSensorMetric();
  temperatureSensorMetric();

  // Generate metric every second.
  delay(1000);
}

void distanceSensorMetric() {

  // Wake up sensor
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  // Calculate the distancen with the response time.
  responseTime = (pulseIn(Input, HIGH) / 2);
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
  String message = "{" +
                   "\"type\":" + "\"" + type + "\"," +
                   "\"data\":" + "{" +
                   "\"date\":" + "\"PUT_DATE\"," +
                   "\"value\":" + metricValue + "}" + "}" + "}";
  sendMessage(message);

}

void sendMessage(String message) {
  // Specify the address of the remote XBee;
  XBeeAddress64 addr64 = XBeeAddress64(XBEE_SH, XBEE_SL);

  // Create a TX Request
  uint8_t dataArray[message.length()];
  message.toCharArray(dataArray, message.length());
  ZBTxRequest zbTx = ZBTxRequest(addr64, dataArray, sizeof(dataArray));

  // Send
  xbee.send(zbTx);
}






