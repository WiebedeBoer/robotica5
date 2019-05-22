#include "AX12A.h"

#define DirectionPin  (2u)
#define BaudRate    (500000ul) // <--- Servo baud rate Default 1000000
//  Please change Arduino Serial monitor baud rate to 57600

//  If  your Servo baud rate 1000000
//  Please change Arduino Serial monitor baud rate to 1000000

unsigned long previousMillis = 0;

const long interval = 10000;

char buffer[100] = {0};
char rx_Byte = 0; //last received byte
String rx_Msg = ""; //received message
String SendSum; //checksum from raspberry
bool rx_Complete = false; //is de transmission done
bool ReadingCheckSum = false; //reading chechsum

void setup()
{
  Serial.begin(115200);
  
  ax12a.begin(BaudRate, DirectionPin, &Serial1);
  ax12a.move(1, 0);
}

void loop()
{
  //Serial.println(checksum("servoDS?,1;2;200&5;0|"));
//  unsigned long currentMillis = millis();
  
  //downwards();
  
//  if (currentMillis - previousMillis >= interval) {
//    previousMillis = currentMillis;
//
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
//  }
}

String servoA = "", servoB = ""; int ser, di, pos, sp;
void serialEvent() {
  while(Serial.available() && rx_Complete == false){
    rx_Byte = (char)Serial.read(); //Read next byte
    
    if(!ReadingCheckSum){ rx_Msg += rx_Byte; } // Enter byte to message
    else { SendSum += rx_Byte; } // Enter byte to sendsum

    //switch from to message to sendsum
    if(rx_Byte == '|'){ ReadingCheckSum = true; }
    
    if(rx_Byte == '\n'){ //endling and cleanup
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
      else if(rx_Msg == "servoDS?|"){ // Seriele input examle: servoDS?,1;1;200&5;0|10
        moveServoDS(rx_Msg_Value);
        result = respondServoDS() + String(checksum(respondServoDS())) + "\n";
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

void downwards() {
  int aServo = 1, bServo = 2;
  int aPos = 300, bPos = 300;
  int aSpeed = 50, bSpeed = 20;
  
  //Setup arm movement position
  ax12a.moveSpeedRW(aServo, aPos, aSpeed);
  ax12a.moveSpeedRW(bServo, bPos, bSpeed);
  //Start arm movement
  ax12a.action();

  Serial.println("Starting while");
  while (true) {
    Serial.println(readPos(aServo));
    Serial.println(readPos(bServo));
    if (readPos(aServo) >= aPos-10 && readPos(bServo) >= bPos-10) { break; }
  }

  Serial.println("Resetting in");
  for (int d = 3; d > 0; d--) {
     Serial.println(d);
    delay(1000);
  }

  ax12a.move(aServo, 200);
  ax12a.move(bServo, 200);

  while (true) {
    if (readPos(aServo) < 510 && readPos(bServo) < 510) { break; }
  }

  Serial.println("Reset succes");
  delay(2500);
}

/* function AX12A.h
  int readTemperature(unsigned char ID);
  int readVoltage(unsigned char ID);
  int readPosition(unsigned char ID);
  int readSpeed(unsigned char ID);
  int readLoad(unsigned char ID);
*/


void move() {
  ax12a.move(1, 200);//ID, position 0-1023 dat is 0-300graden
  delay(1000);
  ax12a.move(1, 600);
  delay(3000);

  //reg = ax12a.readPosition((1u));

  //Serial.println(reg);

  ax12a.move(2, 200);
  delay(1000);
  ax12a.move(2, 600);
  delay(3000);

  ax12a.move(3, 200);
  delay(1000);
  ax12a.move(3, 600);
  delay(3000);

  ax12a.move(4, 200);
  delay(1000);
  ax12a.move(4, 600);
  delay(3000);
}

void moveRW() {
  ax12a.moveRW(1, 200);
  ax12a.moveRW(2, 200);
  ax12a.moveRW(3, 200);
  ax12a.moveRW(4, 200);
  ax12a.action();

  delay(1000);

  ax12a.moveRW(1, 600);
  ax12a.moveRW(2, 600);
  ax12a.moveRW(3, 600);
  ax12a.moveRW(4, 600);
  ax12a.action();

  delay(1000);
}
