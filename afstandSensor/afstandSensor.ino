//Author Thom Smit en Wiebe de Boer en Rob Alma en Kris
//WIP template for communication between arduino and raspberrypi

//usb
char rx_Byte = 0;//last received byte
String rx_Msg = "";//received message
String SendSum;//checksum from raspberry
bool rx_Complete = false;//is de transmission done
bool ReadingCheckSum = false;//reading chechsum

//pins
#define analogPin 20 // sensor value afstand
#define echoPin 9 // sensor value afstand input

#define snelheidLinksPin 12 //sensor value snelheid links

#define microfoonPin 14 //microfoon
#define geluidLaagPin 15 //geluid tonen laag
#define geluidMiddenPin 16 //geluid tonen midden
#define geluidHoogPin 17 //geluid tonen hoog

#define gyrosclPin 18 //gyroscoop scl
#define gyrosdaPin 19 //gyroscoop sda

//motor
#include <SPI.h>
#include "mcp4xxx.h"

using namespace icecave::arduino;

MCP4XXX* pot;

int i = 0;// waarde naar potmeter voor snelheid motor van 0-128

//distance variables
int sensorValue = 0; // Variable to store actual sensor value
bool NearTray;
int AfstandsSensor = 0;
int Distance = 0;

//speed variables

//gyro variables

//sound variables
int soundLow = 0;
int soundMedium = 0;
int soundHigh = 0;

//setup
void setup() {
  // put your setup code here, to run once:
    //ini serial interface
  Serial.begin(115200);
  pinMode(echoPin, INPUT);
}

//test led
bool LedOnOff;
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

//loop
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

//AFSTAND
// read value from pin pn, return value is mapped between 0 and mx-1
int readSensor(int pn, int mx)
{
  return map(analogRead(pn), 0, 1023, 0, mx-1);    
}

//sensor function
String readSensors(int Distance){ 
sensorValue = readSensor(0, 100); // update sensor value
    //afstandsensor lezen
  AfstandsSensor = analogRead(A0);
  Serial.println(AfstandsSensor);
      //delay(150);
      sensorValue = analogRead(analogPin); // read the value from the sensor
      
      long duration;
      int distance;

      duration = pulseIn(echoPin, HIGH);
      distance = (duration * 0.0334) / 2;

//drempel waardes
if(AfstandsSensor <300){
    NearTray = true;
}
else{
    NearTray = false;
}

  pot->set(Distance);
  return "ack:Sensor?<"+ String(distance) + ">|";

}

//SOCKET
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
