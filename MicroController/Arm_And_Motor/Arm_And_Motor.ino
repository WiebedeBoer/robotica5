#include "AX12A.h"
#include <SPI.h>
#include "mcp4xxx.h"
//#include "mcp4xxx.h"

using namespace icecave::arduino;

MCP4XXX* potL;
MCP4XXX* potR;

#define DirectionPin  (2u)
#define BaudRate    (500000ul) // Servo baud rate Default 1000000

#define potLpin1 3
#define potLpin2 4

#define potRpin1 5
#define potRpin2 6

bool debug = false;

unsigned long motorPreviousMillis = 0;

const long motorInterval = 500;

char buffer[100] = {0};
char rx_Byte = 0;             // Last received byte
String rx_Msg = "";           // Received message
String SendSum;               // Checksum from raspberry
bool rx_Complete = false;     // Is de transmission done
bool ReadingCheckSum = false; // Reading chechsum

bool drivingL, drivingR;

int ax12aPos[5] = {0,0,0,0,0};
int ax12aBoundries[5][2] = {{0,1023},{200, 600},{100,640},{350,950},{0,1023}};

int Xinleft = A0; // X Input Pin left
int Yinleft = A1; // Y Input Pin left
int KEYinleft = 16; // Push Button left

int Xinright = A3; // X Input Pin left
int Yinright = A4; // Y Input Pin left
int KEYinright = 19; // Push Button left

void setup()
{
  Serial.begin(115200);
  potL = new MCP4XXX(10);
  potR = new MCP4XXX(9);

  pinMode(potLpin1, OUTPUT);
  pinMode(potLpin2, OUTPUT);
  pinMode(potRpin1, OUTPUT);
  pinMode(potRpin2, OUTPUT);

  pinMode (KEYinleft, INPUT);
  pinMode (KEYinright, INPUT);
  
  ax12a.begin(BaudRate, DirectionPin, &Serial1);
  ax12a.move(1, 550);
  ax12a.move(2, 200);
  ax12a.move(3, 300);
  ax12a.move(4, 550);
  ax12a.move(5, 600);
}

void loop()
{
  
  for (int i = 0; i <= 4; i++) {
    ax12aPos[i] = readPos(i+1);
//    Serial.print(i+1);Serial.print(" = ");Serial.println(ax12aPos[i]);
  }
//  Serial.println("");

  int xValr, yValr, buttonValr, xVall, yVall, buttonVall;

  xVall = analogRead (Xinleft);
  yVall = analogRead (Yinleft);

//  Serial.println(yVall);
  
//  if (yVall > 800) {
//    moveServoS("5;" + String(readPos(5)+10) + ";50&6;0;500|10");
//    delay(100);
//  } else if (yVall < 300) {
//  
//    moveServoS("5;" + String(readPos(5)-10) + ";50&6;0;500|10");
//    delay(100);
//  }

  unsigned long currentMillis = millis();
  
  if (currentMillis - motorPreviousMillis >= motorInterval) {
    motorPreviousMillis = currentMillis;
    if (drivingL || drivingR) {
      digitalWrite(potLpin1, LOW);
      digitalWrite(potLpin2, LOW);
      digitalWrite(potRpin1, LOW);
      digitalWrite(potRpin2, LOW);
      potL->set(0);
      potR->set(0);
      drivingL = false;
      drivingR = false;
    }
  }

  if (debug) {
    Serial.println("-----------------------------------------");
    for (int i = 1; i <= 5; i++) {
      Serial.print("Servo "); Serial.println(i);
      Serial.print("Temp");Serial.print(" = "); Serial.println(readTemp(i));
      Serial.print("Posi");Serial.print(" = "); Serial.println(readPos(i));
      Serial.print("Volt");Serial.print(" = "); Serial.println(readVolt(i));
      Serial.print("Load");Serial.print(" = "); Serial.println(readLoad(i));
      Serial.print("Spee");Serial.print(" = "); Serial.println(readSpeed(i));
      Serial.println("");
    }
    Serial.println("-----------------------------------------");
    delay(1000);
  }
}

void serialEvent() {
  while(Serial.available() && rx_Complete == false){
    rx_Byte = (char)Serial.read(); //Read next byte
    
    if(!ReadingCheckSum){ rx_Msg += rx_Byte; }  // Enter byte to message
    else { SendSum += rx_Byte; }                // Enter byte to sendsum

    //switch from to message to sendsum
    if(rx_Byte == '|'){ ReadingCheckSum = true; }
    
    if(rx_Byte == '\n'){ // End of message, cleanup
      rx_Complete = true;
      ReadingCheckSum = false;
    }
  }
  
  //execute received msg
  if(rx_Complete){
    String OriginalMessage = rx_Msg;
    int commaIndex = rx_Msg.indexOf(',');
    String rx_Msg_Value = rx_Msg.substring(commaIndex +1, rx_Msg.length() -1);
    rx_Msg = rx_Msg.substring(0, commaIndex) + "|";

    // checksum(OriginalMessage) == SendSum.toInt()
    if(true) { //control checksum with sendsum, for error checking. It continues when no error is found.
      //possible commands and code here. Pi waits for ack.
      String result = "";
      if(rx_Msg == "servo?|"){
        result = respondServo() + String(checksum(respondServo())) + "\n";
      }
      else if(rx_Msg == "servoS?|"){
        result = respondServoS() + String(checksum(respondServoS())) + "\n";
      }
      else if(rx_Msg == "servoDS?|"){
        result = respondServoDS() + String(checksum(respondServoDS())) + "\n";
      }
      else if(rx_Msg == "motor?|") {
        result = respondMotor() + String(checksum(respondMotor())) + "\n";
      }
//      else {
//        result = "/n";
//      }
      
      int resultLength = result.length() +1;          // Convert string to char array
      char resultarray[resultLength];
      result.toCharArray(resultarray, resultLength);  // Result to Array
      Serial.write(resultarray);                      // Send chararray to rp

      if(rx_Msg == "servo?|"){ // Seriele input example: servo?,1;300&5;0|10
        moveServo(rx_Msg_Value);
      }
      else if(rx_Msg == "servoS?|"){ // Seriele input example: servoS?,1;100;50&5;0;100|10
        moveServoS(rx_Msg_Value);
      }
      else if(rx_Msg == "servoDS?|"){ // Seriele input example: servoDS?,1;1;200&5;0;50|10
        moveServoDS(rx_Msg_Value);
      }
      else if(rx_Msg == "motor?|") { // Seriele input example: motor?,1;64&2;64|10
        motor(rx_Msg_Value);
        motorPreviousMillis = millis(); // Not resetting this timer will make the motor stop after * seconds
      }
    }
    
    // Clean message
    rx_Msg = ""; SendSum = "";
    rx_Complete = false;
  }
}

//calculate checksum.
int checksum(String Str){
  int sum = 0;
  for(int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
  }
  return sum;
}

// Serial response functions
String respondServo() { return "ack:servo?<"+ getAllPositions() +">|"; }

String respondServoS() { return "ack:servoS?<"+ getAllPositions() +">|"; }

String respondServoDS() { return "ack:servoDS?<"+ getAllPositions() +">|"; }

String respondMotor() { return "ack:motor?<>|"; }
