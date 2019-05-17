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

bool NearTray;
int AfstandsSensor = 0;
int Distance = 0;

void setup() {
  // put your setup code here, to run once:
    //ini serial interface
  Serial.begin(115200);
}

//test led
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
  // put your main code here, to run repeatedly:

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
      else if (rx_Msg == "Sensor?|"){
        String result = readSensors(Distance);
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

//sensor function
String readSensors(int Distance){ 

    //afstandsensor lezen
  AfstandsSensor = analogRead(A0);
  Serial.println(AfstandsSensor);
      //delay(150);

//drempel waardes
if(AfstandsSensor <300){
    NearTray = true;
}
else{
    NearTray = false;
}

  pot->set(Distance);
  return "ack:Sensor?<Distance>|";

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
