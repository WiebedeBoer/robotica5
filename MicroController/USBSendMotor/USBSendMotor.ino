//Author Thom Smit en Wiebe de Boer en Rob Alma
//WIP template for communication between arduino and raspberrypi

//usb
char rx_Byte = 0;//last received byte
String rx_Msg = "";//received message
String SendSum;//checksum from raspberry
bool rx_Complete = false;//is de transmission done
bool ReadingCheckSum = false;//reading chechsum

//led
bool LedOnOff;

//motor
#include <SPI.h>
#include "mcp4xxx.h"

using namespace icecave::arduino;

MCP4XXX* pot;

int i = 0;// waarde naar potmeter voor snelheid motor van 0-128

bool drivingL, drivingR;

void setup() {
  //ini serial interface
  Serial.begin(115200);
  //demo pin, change this.
  pinMode(LED_BUILTIN, OUTPUT);
  //motor
  // Construct an instance of the MCP4XXX to manage the digipot.
  // The first parameter is the pin number to use for 'chip select' (CS), if you are
  // using the default SPI CS pin for your Arduino you can simply omit this parameter.
  pot = new MCP4XXX(10);//chipselect pin teensy

  pinMode(3, OUTPUT);//INa
  pinMode(4, OUTPUT);//INb
  // INa en INb zijn voor de richting, beide hoog (of laag?) is motor op rem?
}

//sp = speed, value between 0-128
//direction = forward, backward or stand stil
//direction = forward: 1, backward: 2, stil: 0
String wheelLeft(int sp, int direction) {
  Serial.println("Wheelleft Adjustment");
  if (direction == 0) {
    drivingL = false;
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    return;
  }

  if (direction == 1) {
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    Serial.println("Directiona");
  } else if (direction == 2) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    Serial.println("Picobello");
  }

  pot->set(sp);
  return "ack:Motor?<Speed>|";
}

String Led(){
  if(LedOnOff == true){
    LedOnOff = false;
    return "ack:Led?<On>|";
    }
  if(LedOnOff == false){
    LedOnOff = true;
    return "ack:Led?<Off>|";
  }
}
void loop() {
  //Message received do command
  if(rx_Complete){
    //Serial.println("Command:"+rx_Msg+"Sum:" +SendSum + "CalculatedSum:" + String(checksum(rx_Msg))); //print contains all useful debug information
  if(checksum(rx_Msg) == SendSum.toInt()){ //control checksum with sendsum, for error checking
    //possible commands and code here   
      if(rx_Msg == "Led?|"){
        String result = Led();
        String Checksum = String(checksum(result));
        Serial.println(result + Checksum );  
        }
      else if (rx_Msg == "Motor?|"){
        String result = wheelLeft(200,1);
        String Checksum = String(checksum(result));
        Serial.println(result + Checksum );  
       }     

   }
  //clean message   
  rx_Msg = "";
  SendSum = "";
  rx_Complete = false;
}
}
//serialEventInterupt
void serialEvent(){
  while(Serial.available()){
    rx_Byte = (char)Serial.read();//Read next byte
    if(ReadingCheckSum == false){//enter byte to message
    rx_Msg += rx_Byte;
    }else{//enter byte to sendsum
      SendSum += rx_Byte;
      }    
    if(rx_Byte == '|'){//switch from to message to sendsum
      ReadingCheckSum = true;
    } 
    
    if(rx_Byte == '\n'){//endling and cleanup
      rx_Complete = true;
      ReadingCheckSum = false;
      }
  }
}
//calculate checksum.
int checksum(String Str){
  int sum = 0;
  for(int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
    }
    //Serial.println(sum);
    return sum;
}
