#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>
#include <EmonLib.h>

const int XBEE_OUT = 6;
const int XBEE_IN = 5;

// Xbee concentrator
#define XBEE_SH 0x00000000
#define XBEE_SL 0x00000000

#define POWER "power"

#define SLEEP_TIME 3000

XBee xbee = XBee();
SoftwareSerial sw(XBEE_OUT,XBEE_IN);

// Crear una instancia EnergyMonitor
EnergyMonitor energyMonitor;

// Voltaje de nuestra red eléctrica
float voltajeRed = 220.0;

void setup()
{
  Serial.begin(9600);
  // XBee
  sw.begin(9600);
  xbee.setSerial(sw);

  // Iniciamos la clase indicando
  // Número de pin: donde tenemos conectado el SCT-013
  // Valor de calibración: valor obtenido de la calibración teórica
  energyMonitor.current(0, 2.6);
}

void loop()
{
  // Obtenemos el valor de la corriente eficaz
  // Pasamos el número de muestras que queremos tomar
  double Irms = energyMonitor.calcIrms(1484);

  // Calculamos la potencia aparente
  double potencia =  Irms * voltajeRed;

  createDataMessage(potencia, POWER);

  // Mostramos la información por el monitor serie
  Serial.print("Potencia = ");
  Serial.print(potencia);
  Serial.print("    Irms = ");
  Serial.println(Irms);
  delay(SLEEP_TIME);
}

void createDataMessage(float metricValue, String type) {
  String message = "{\"type\":\""+type+"\",\"data\":{\"date\":\"PUT_DATE\",\"value\":" + metricValue + "} }";
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

