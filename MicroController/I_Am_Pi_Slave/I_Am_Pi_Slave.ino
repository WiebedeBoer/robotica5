// Author Thom Smit en Wiebe de Boer en Rob Alma en Kris
// WIP template for communication between arduino and raspberrypi

// Includes
#include <SPI.h>
#include "mcp4xxx.h"
#include <Wire.h>

using namespace icecave::arduino;

MCP4XXX* pot;

// Serial event
char rx_Byte = 0;             // Last received byte
String rx_Msg = "";           // Received message
char rx_Byte2 = 0;             // Last received byte for serial 1
String rx_Msg2 = "";           // Received message for serial 1
String SendSum;               // Checksum from raspberry
bool rx_Complete = false;     // Transmission
bool rx_Complete2 = false;     // Transmission for serial 1

bool ReadingCheckSum = false; // Reading chechsum

// Initializing all variables
#define IRPin 20              // Sensor value distance analog
float distance;

#define micPin 14             // Microphone
#define soundLPin 15          // Geluid tonen laag
#define soundMPin 16          // Geluid tonen midden
#define soundHPin 17          // Geluid tonen hoog
int mic, soundL, soundM, soundH;

#define speedRPin 13          // Sensor value speed right digital
#define speedLPin 12          // Sensor value speed left digital
int speedL, speedR;

#define gyrosclPin 18         // Gyroscoop scl
#define gyrosdaPin 19         // Gyroscoop sda
#define gyrointPin 11         // Gyroscoop int digital

const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;

unsigned long timer = 0;
float timestep = 0.01;

float pitch = 0;
float roll = 0;
float yaw = 0;

//function strings
String ret_distance;
String ret_sound;
String ret_speed;
String ret_gyro;

//setup
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(speedRPin, INPUT);
  pinMode(speedLPin, INPUT);
  digitalWrite(speedRPin, LOW);
  digitalWrite(speedLPin, LOW);

  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

//loop
void loop() {
  updateDistance();
  updateMicrophone();
  updateGyro();
  //measureSpeed();
  delay(2500);

  

//  Debug distance
//  updateDistance();
//  Serial.print("Analog value: "); Serial.println(analogRead(IRPin));
//  Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");

//  Debug microphone
//  updateMicrophone();
//  Serial.print("Microphone values: Mic/Low/Medium/High - ");
//  Serial.print(mic);Serial.print("/");Serial.print(soundL);Serial.print("/");
//  Serial.print(soundM);Serial.print("/");Serial.println(soundH);
//  Serial.println(readMicrophone());

//  Debug speed
//  Serial.print("Right: ");Serial.println(digitalRead(speedRPin));
}

// SENSOR READING
// read value from pin pn, return value is mapped between 0 and mx-1
int readSensor(int pn, int mx)
{
  return map(analogRead(pn), 0, 1023, 0, mx-1);    
}

//send sensor values function
String sendSensorsValues(){ 
  ret_distance = updateDistance();
  ret_sound = updateMicrophone();
  ret_speed = getSpeed();
  ret_gyro = updateGyro();
  //return alle waarden
  return "ack:Sensor?<"+ret_distance+";"+ret_sound+";"+ret_speed+";"+ret_gyro+">|";
}

//SPEED
String speedSensors(){
  int leftSpeed;
  //return "ack:Sensor?<"+ String(leftSpeed) + ">|";
}

//GYROSCOPE
String gyroscopeSensors(){
  int gyro;
  //return "ack:Sensor?<"+ String(gyro) + ">|";
}

  void serialEvent1(){
  while(Serial1.available()&& rx_Complete2 == false){
    rx_Byte2 = (char)Serial1.read();//Read next byte
    rx_Msg2 += rx_Byte2;   
    if(rx_Byte2 == '\n'){//endling and cleanup
      rx_Complete2 = true;
      }
  }
}
//SOCKET
//serialEventInterupt
void serialEvent(){
  while(Serial.available()){
    rx_Byte = (char)Serial.read();// Read next byte
    
    if(ReadingCheckSum == false){ // Add byte to message
    rx_Msg += rx_Byte;
    } else { // Add byte to sendsum
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


  //Message received do command
  if(rx_Complete){
    String OriginalMessage = rx_Msg;
    int commaIndex = rx_Msg.indexOf(',');
    String rx_Msg_Value = rx_Msg.substring(commaIndex +1, rx_Msg.length() -1);
    rx_Msg = rx_Msg.substring(0, commaIndex) + "|";
    
    // checksum(rx_Msg) == SendSum.toInt()
    if(true){ // Control checksum with sendsum, for error checking
      String result = "ack:";
      
      if (rx_Msg == "allSensors?|"){
        //result += "allSensors?|" + String(distance) + ";" + String(speedL) + ";" + String(speedR);
        result += sendSensorsValues();
//        String result = readSensors(distance);
//        String Checksum = String(checksum(result));
//        Serial.println(result + Checksum );  
      } else if (rx_Msg == "speed?|") { // speed?,1;300&5;0|10
//        Serial.println("Getting them speed");
        result += "speed?<" + String(speedL) + ";" + speedR + ">|";
      } else if(rx_Msg == "refresh?|"){
        Serial.print("RefreshMessageSend");
        Serial1.print(rx_Msg + '\n');        
      }
    }
    if(rx_Complete2){
      Serial.print(rx_Msg2 + '\n');
      rx_Complete2 = false;
      rx_Msg2 = "";
    }
    // Clean message   
    rx_Msg = "";
    SendSum = "";
    rx_Complete = false;
  }
}

//DISTANCE
String updateDistance() { 
  distance = (6787.0 / (analogRead(IRPin) - 3.0)) - 4.0; 
  return String(distance);
 }

//SOUND
String updateMicrophone() {
  mic = analogRead(micPin);
  soundL = analogRead(soundLPin);
  soundM = analogRead(soundMPin);
  soundH = analogRead(soundHPin);
  String str_sound = String(mic)+";"+String(soundL)+";"+String(soundM)+";"+String(soundH);
  return str_sound;
}

//SPEED
int maxMillis = 500;
unsigned long startMillis = millis();
int left = 0, right = 0, count = 0;

void measureSpeed() {
  unsigned long currentMillis = millis();
  if (currentMillis - startMillis > maxMillis) {
    speedL = left * 3;
    speedR = right * 3;
    left = 0, right = 0;
    startMillis = currentMillis;
    //Serial.println(speedL);
    //Serial.println(speedR);
    //Serial.print("Total counts: ");Serial.println(count);
    count = 0;
  } else {
    if (digitalRead(speedRPin) == 0) {
      right++;
    }
    if (digitalRead(speedLPin) == 0) {
      left++;
    }

    count++;
  }
}

String getSpeed() {
  return String(speedL+";"+speedR);
}

//GYROSCOPE
// Pins
//#define gyrosclPin 18         // Gyroscoop scl
//#define gyrosdaPin 19         // Gyroscoop sda
//#define gyrointPin 11         // Gyroscoop int digital
String updateGyro() {
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  //Serial.print("AccX = "); Serial.print(AccX);
  //Serial.print(" || AccY = "); Serial.print(AccY);
  //Serial.print(" || AccZ = "); Serial.print(AccZ);
  //Serial.print(" || Temp = "); Serial.print(Temp/340.00+36.53);
  //Serial.print(" || GyroX = "); Serial.print(GyroX);
  //Serial.print(" || GyroY = "); Serial.print(GyroY);
  //Serial.print(" || GyroZ = "); Serial.println(GyroZ);
  return String(GyroX)+";"+String(GyroY)+";"+String(GyroZ);
}

// Calculate checksum.
int checksum(String Str){
  int sum = 0;
  for(int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
    }
    //Serial.println(sum);
    return sum;
}
