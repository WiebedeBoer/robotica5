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

void setup()
{
  Serial.begin(115200);
  potL = new MCP4XXX(10);
  potR = new MCP4XXX(9);

  pinMode(potLpin1, OUTPUT);
  pinMode(potLpin2, OUTPUT);
  pinMode(potRpin1, OUTPUT);
  pinMode(potRpin2, OUTPUT);
  
  ax12a.begin(BaudRate, DirectionPin, &Serial1);
  ax12a.move(1, 0);
}

void loop()
{
  for (int i = 0; i < 5; i++)
    ax12aPos[i] = readPos(i+1);

  unsigned long currentMillis = millis();
  
  if (currentMillis - motorPreviousMillis >= interval) {
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
    if(checksum(OriginalMessage) == SendSum.toInt()) { //control checksum with sendsum, for error checking. It continues when no error is found.
      //possible commands and code here. KEEP IT SHORT the RP waits until its finished.
      String result = "";
      if(rx_Msg == "servo?|"){ // Seriele input example: servo?,1;300&5;0|10
        moveServo(rx_Msg_Value);
        result = respondServo() + String(checksum(respondServo())) + "\n";
      }
      else if(rx_Msg == "servoDS?|"){ // Seriele input example: servoDS?,1;1;200&5;0|10
        moveServoDS(rx_Msg_Value);
        result = respondServoDS() + String(checksum(respondServoDS())) + "\n";
      }
      else if(rx_Msg == "motor?|") { // Seriele input example: motor?,1;64&2;2;64|10
        motor(rx_Msg_Value);
        motorPreviousMillis = millis(); // Not resetting this timer will make the motor stop after * seconds
        result = respondMotor() + String(checksum(respondMotor())) + "\n";
      }
      
      int resultLength = result.length() +1; // Convert string to char array
      char resultarray[resultLength];
      result.toCharArray(resultarray, resultLength);
      Serial.write(resultarray);// Send chararray to rp

      //clean message
      rx_Msg = ""; SendSum = "";
      rx_Complete = false;
    }
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

String respondServoDS() { return "ack:servoDS?<"+ getAllPositions() +">|"; }

String respondMotor() { return "ack:motor?<>|"; }

//    Serial.println("-----------------------------------------");
//    for (int i = 1; i <= 4; i++) {
//      Serial.print("Servo "); Serial.println(i);
//      Serial.print("Temp");Serial.print(" = "); Serial.println(readTemp(i));
//      Serial.print("Posi");Serial.print(" = "); Serial.println(readPos(i));
//      Serial.print("Volt");Serial.print(" = "); Serial.println(readVolt(i));
//      Serial.print("Load");Serial.print(" = "); Serial.println(readLoad(i));
//      Serial.print("Spee");Serial.print(" = "); Serial.println(readSpeed(i));
//      Serial.println("");
//    }
//    Serial.println("-----------------------------------------");
