#include "AX12A.h"
#include <SPI.h>
#include "mcp4xxx.h"

using namespace icecave::arduino;

MCP4XXX* potL;
MCP4XXX* potR;

#define DirectionPin  (2u)
#define BaudRate    (500000ul) // <--- Servo baud rate Default 1000000

#define potLpin1 3
#define potLpin2 4

#define potRpin1 5
#define potRpin2 6

unsigned long previousMillis = 0;

const long interval = 500;

char buffer[100] = {0};
char rx_Byte = 0; //last received byte
String rx_Msg = ""; //received message
String SendSum; //checksum from raspberry
bool rx_Complete = false; //is de transmission done
bool ReadingCheckSum = false; //reading chechsum

String servoA = "", servoB = "", wheelL = "", wheelR = ""; 
int ser, di, pos, sp;
bool drivingL, drivingR;

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
  //Serial.println(checksum("servoDS?,1;2;200&5;0|"));
  unsigned long currentMillis = millis();
  
  //downwards();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
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
  }
}

void serialEvent() {
  while(Serial.available() && rx_Complete == false){
    rx_Byte = (char)Serial.read(); //Read next byte
    
    if(!ReadingCheckSum){ rx_Msg += rx_Byte; } // Enter byte to message
    else { SendSum += rx_Byte; } // Enter byte to sendsum

    //switch from to message to sendsum
    if(rx_Byte == '|'){ ReadingCheckSum = true; }
    
    if(rx_Byte == '\n'){ //ending and cleanup
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
        previousMillis = millis(); // 1 Second no input is reset
        result = respondMotor() + String(checksum(respondMotor())) + "\n";
      }
      
      int resultLength = result.length() +1; // convert string to char array
      char resultarray[resultLength];
      result.toCharArray(resultarray, resultLength);
      Serial.write(resultarray);//send chararray to rp

      //clean message
      rx_Msg = ""; SendSum = ""; servoA = ""; servoB = "";
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

void motor(String input) {
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == "&") {
      wheelL = input.substring(0, i);
      wheelR = input.substring(i+1);
      break;
    }
  }

  // Search seperator and move servo
  if (wheelL.length() > 0) {
    for (int j = 0; j < wheelL.length(); j++) {
      if (wheelL.substring(j, j+1) == ";") {
        moveWheelLeft(wheelL.substring(0,j).toInt(), wheelL.substring(j+1).toInt());
        break;
      }
    }
  }

  if (wheelR.length() > 0) {
    for (int k = 0; k < wheelR.length(); k++) {
      if (wheelR.substring(k, k+1) == ";") {
        moveWheelRight(wheelR.substring(0,k).toInt(), wheelR.substring(k+1).toInt());
        break;
      }
    }
  }
}

void moveWheelLeft(int di, int speed) {
  if (di == 0) {
    drivingL = false;
    digitalWrite(potLpin1, LOW);
    digitalWrite(potLpin2, LOW);
    return;
  }
  
  drivingL = true;

  if (di == 1) {
    digitalWrite(potLpin1, LOW);
    digitalWrite(potLpin2, HIGH);
  } else if (di == 2) {
    digitalWrite(potLpin1, HIGH);
    digitalWrite(potLpin2, LOW);
  }
  potL->set(speed);
}

void moveWheelRight(int di, int speed) {
  if (di == 0) {
    drivingR = false;
    digitalWrite(potRpin1, LOW);
    digitalWrite(potRpin2, LOW);
    return;
  }
  
  drivingR = true;

  if (di == 1) {
    digitalWrite(potRpin1, LOW);
    digitalWrite(potRpin2, HIGH);
  } else if (di == 2) {
    digitalWrite(potRpin1, HIGH);
    digitalWrite(potRpin2, LOW);
  }

  potR->set(speed);
}

void moveServo(String input) {
  // Split input to servoA and servoB
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == "&") {
      servoA = input.substring(0, i);
      servoB = input.substring(i+1);
      break;
    }
  }

  if (servoB == "") { servoA = input; } //Serial.println(servoA); }
  
  // Search seperator and move servo
  if (servoA.length() > 0) {
    for (int j = 0; j < servoA.length(); j++) {
      if (servoA.substring(j, j+1) == ";") {
        ax12a.move(servoA.substring(0,j).toInt(), servoA.substring(j+1).toInt());
        break;
      }
    }
  }
  
  // Search seperator and move servo
  if (servoB.length() > 0) {
    for (int k = 0; k < servoB.length(); k++) {
      if (servoB.substring(k, k+1) == ";") {
        ax12a.move(servoB.substring(0,k).toInt(), servoB.substring(k+1).toInt());
        break;
      }
    }
  }
}

void moveServoDS(String input) {
  // Split rx_Msg_Value to servoA and servoB
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == "&") {
      servoA = input.substring(0, i);
      servoB = input.substring(i+1);
      break;
    }
  }

  if (servoB == "") { servoA = input; } //Serial.println(servoA); }
  
  // Search seperator and move servo
  if (servoA.length() > 0) {
    for (int j = 0; j < servoA.length(); j++) {
      if (servoA.substring(j, j+1) == ";") { // Search first seperator
        int ser = servoA.substring(0,j).toInt();
        for (int m = j; m < servoA.substring(j+1).length(); m++) {
          if(servoA.substring(m,m+1) == ";") { // Search second seperator
            di = servoA.substring(j+1, m).toInt();
            sp = servoA.substring(m+1).toInt();
            if (di==0){pos = readPos(ser);}
            else if (di==1){pos=readPos(ser)+25; if (pos > 1023){pos = 1023;}}
            else if (di==2){pos=readPos(ser)-25; if (pos < 0){pos = 0;}}
            ax12a.moveSpeed(ser, pos, sp);
          }
        }
        break;
      }
    }
  }
  
  // Search seperator and move servo
  if (servoB.length() > 0) {
    for (int j = 0; j < servoB.length(); j++) {
      if (servoB.substring(j, j+1) == ";") { // Search first seperator
        int ser = servoB.substring(0,j).toInt();
        for (int m = j; m < servoB.substring(j+1).length(); m++) {
          if(servoB.substring(m,m+1) == ";") {
            di = servoB.substring(j+1, m).toInt();
            sp = servoB.substring(m+1).toInt();
            if (di==0){pos = readPos(ser);}
            else if (di==1){pos=readPos(ser)+25;}
            else if (di==2){pos=readPos(ser)-25; if (pos < 0){pos = 0;}}
            ax12a.moveSpeed(ser, pos, sp);
          }
        }
        break;
      }
    }
  }
}

String getAllPositions() {
  String response = "1;";// + readPos(1) + "&2;" + readPos(2) + "&3;" + readPos(3) + "&4;" + readPos(4);
  response += readPos(1);
  response += "&2;";
  response += readPos(2);
  response += "&3;";
  response += readPos(3);
  response += "&4;";
  response += readPos(4);
  return response;
}

int readTemp(int i) { return ax12a.readTemperature(i); }

int readPos(int i) { return ax12a.readPosition(i); }

int readVolt(int i) { return ax12a.readVoltage(i); }

int readLoad(int i) { return ax12a.readLoad(i); }

int readSpeed(int i) { return ax12a.readSpeed(i); }

void runUnderTemp() {
  for (int i = 1; i <= 4; i++) {
    int temp = ax12a.readTemperature(i);
    //Serial.println(temp);
    if (temp < 40) {
      ax12a.setEndless(i, ON);
      ax12a.turn(i, RIGHT, 800);
    } else {
      ax12a.setEndless(i, OFF);
    }
  }

  Serial.println("\n");
  delay(5000);
}

String respondServo() { return "ack:servo?<"+ getAllPositions() +">|"; }

String respondServoDS() { return "ack:servoDS?<"+ getAllPositions() +">|"; }

String respondMotor() { return "ack:motor?<>|"; }
