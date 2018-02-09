#include <SoftwareSerial.h>
#include <XBee.h>

#define SCANNING 1
#define NO_SCANNING 2

#define MAX_WORD_LENGTH 200

#define MAX_NO_READ_LOOP 10

#define SLEEP_TIME 200

// Xbee custom (Z4)
//#define XBEE_SH 0x0013A200
//#define XBEE_SL 0x40E7D742

// Xbee concentrator
#define XBEE_SH 0x00000000
#define XBEE_SL 0x00000000


int state = NO_SCANNING;

char current_word[MAX_WORD_LENGTH];
int current_word_cursor=0;

int no_read_loop =0;

XBee xbee = XBee();

SoftwareSerial sw(7,5);
SoftwareSerial sw2(11,9);

void setup() {
  Serial.begin(9600);
  sw2.begin(9600);

  xbee.setSerial(sw2);
  
  sw.begin(9600);
  sw.print(F("AT"));
  delay(100);
  while (sw.available() ) {
     String res = sw.readString();
     Serial.println(res);
  }

  sw.print(F("AT+ROLE1"));
  delay(100);
  while (sw.available() ) {
     String res = sw.readString();
     Serial.println(res);
  }
 
  sw.print(F("AT+IMME1"));
  delay(100);
  while (sw.available() ) {
     String res = sw.readString();
     Serial.println(res);
  }
  sw.print(F("AT+RESET"));
  delay(2000);
  while (sw.available() ) {
     String res = sw.readString();
     Serial.println(res);
  }

  delay(20);

  
}

void loop() {
   switch (state) {
    case NO_SCANNING:
          startScanning();
          break;
    case SCANNING:
          processScanning();
          break;
    default :
          break;
    }
   delay(100);
}

void startScanning(){
  sw.print(F("AT+DISI?"));
  state= SCANNING;
  Serial.println(F("START SCANNING"));
}

void processScanning() {
   int count=sw.available();
   if (count > 0) {
      for(;count>0;count--){
          char readChar = sw.read();
          if (readChar=='O') {
            processCurrentWord();
        
          }
          if (current_word_cursor<MAX_WORD_LENGTH){
             current_word[current_word_cursor++]=readChar;
           }
           
          if (current_word[7]=='E') {
            finishScanning();           
           } 
         
        }    
    } else {
      ////Serial.println("NO_READ_LOOP");
      no_read_loop++;
      if (no_read_loop > MAX_NO_READ_LOOP) {
        finishScanning();
        
       } 
     }
}

void finishScanning(){
  //  //Serial.println(String(current_word));
    //Serial.println(F("STOP SCANNING"));
    state= NO_SCANNING;
    clearWordBuffer();
    delay(SLEEP_TIME);
 }

void processCurrentWord() {
  if ((current_word_cursor==78)&&(!checkCharBuffer(current_word,'0',9,16))){
   
    String deviceString =String(current_word);
    Serial.println(deviceString);

    String deviceMajor = "0x"+deviceString.substring(50,54);
    String deviceMinor = "0x"+deviceString.substring(54,58);
    String deviceMeasuredPower = "0x"+ deviceString.substring(58,60);

    unsigned long major = strtol(deviceMajor.c_str(), NULL, 0);
    unsigned long minor = strtol(deviceMinor.c_str(), NULL, 0);
    signed char mp = (signed char)strtol(deviceMeasuredPower.c_str(), NULL, 0);

    
    String deviceId = deviceString.substring(17,49);
    String deviceRssi = deviceString.substring(74,78);
    
    int temp_rssi = deviceRssi.toInt();

    createDataMessage(deviceId,major,minor,mp,temp_rssi); 
    
    Serial.println(deviceId + " "+major + " " + minor+ " "+ mp+ " "+ temp_rssi );
  }
  clearWordBuffer();
 }

int StrToHex(char str[])
{
  return (int) strtol(str, 0, 16);
}

void clearWordBuffer(){
    for (int i =0 ; i<MAX_WORD_LENGTH;i++){
      current_word[i]=0;
    }
    no_read_loop=0;
   current_word_cursor=0;
}

boolean checkCharBuffer(char cadena[],char caracter,int start_idx, int end_idx){
  boolean checkResult = true;
  for (int i=start_idx;checkResult && i<end_idx;i++){
    checkResult&=(cadena[i]==caracter);
  }
  return checkResult;
}

void createDataMessage(String deviceId,unsigned long  major ,unsigned long  minor,signed char mp,int rssi) {
   
  String message = "{\"type\":\"beacon\",\"data\":{\"date\":\"PUT_DATE\",\"value\":{\"ibeaconid\":\""+ deviceId +"\",\"rssi\":"+ rssi +",\"major\":"+ major+",\"minor\":"+ minor+",\"measuredpower\":"+mp+"}}} ";
  
  Serial.println(message );               
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
 
