#include "AX12A.h"
#include "SPI.h"
#include "mcp4xxx.h"

using namespace icecave::arduino;

// Init motor library
MCP4XXX* potL;
MCP4XXX* potR;

#define DirectionPin  (2u)
#define BaudRate    (500000ul) // Servo baud rate Default 1000000

#define potLpin1 3
#define potLpin2 4

#define potRpin1 5
#define potRpin2 6

bool debug = false; // Debug enables a lot of Serial prints

unsigned long motorPreviousMillis = 0;

int motorInterval = 300;

char buffer[100] = {0};
char rx_Byte = 0;             // Last received byte
String rx_Msg = "";           // Received message
String SendSum;               // Checksum from raspberry
bool rx_Complete = false;     // Is de transmission done
bool ReadingCheckSum = false; // Reading chechsum

bool drivingL, drivingR;

int ax12aPos[5] = {0,0,0,0,0};

int ax12aHeadPos[4][3] = {{210, 512, 820},{820, 820, 510},{330, 580, 890}, {240, 200, 500}};
int ax12aEyesPos[2][3] = {{943, 200, 500},{80, 270, 420}};

//String id, di, pos, sp;

void setup()
{
  Serial.begin(115200); // Servo's Serial
  
  // Init motor on pins
  potL = new MCP4XXX(10);
  potR = new MCP4XXX(9);

  // Motor setup
  pinMode(potLpin1, OUTPUT);
  pinMode(potLpin2, OUTPUT);
  pinMode(potRpin1, OUTPUT);
  pinMode(potRpin2, OUTPUT);
  digitalWrite(potLpin1, LOW);
  digitalWrite(potLpin2, LOW);
  digitalWrite(potRpin1, LOW);
  digitalWrite(potRpin2, LOW);
  potL->set(0);
  potR->set(0);

  // Servo setup and start position
  ax12a.begin(BaudRate, DirectionPin, &Serial1);

  // Egg Arm
  ax12a.move(1, 530); //0 - 1023 //0 - 1000 //68mm vanaf onderkant //85mm vanaf onder tot join //nieuwe begin 512 //max 800 //min 400
  ax12a.move(2, 170); //200 min // 200 begin //max 550
  ax12a.move(3, 250); //250 begin //50 min //max 500
  ax12a.move(4, 530); //min 350 //begin 530 // max 700 
  ax12a.move(5, 500); //min 100 //max 190 //begin 190

  // Head Arm
  ax12a.move(6, 210);
  ax12a.move(7, 820);
  ax12a.move(8, 330);
  ax12a.move(9, 240);
  ax12a.move(10, 100);
  ax12a.move(11, 100);
  
  if (debug) { motorInterval = 2500; }
}

void loop()
{
  // Update servo positions
  for (int i = 0; i <= 4; i++) {
    ax12aPos[i] = readPos(i+1);
  }

  unsigned long currentMillis = millis(); // Current millis
  
  if (currentMillis - motorPreviousMillis >= motorInterval) { // Check wether interval passed
    motorPreviousMillis = currentMillis; // Set last check millis

    // If driving, stop motor
    if (drivingL || drivingR) { // Check Motor.ino for more details
      digitalWrite(potLpin1, LOW);
      digitalWrite(potLpin2, LOW);
      digitalWrite(potRpin1, LOW);
      digitalWrite(potRpin2, LOW);
      potL->set(0);
      potR->set(0);
      drivingL = false;
      drivingR = false;
    }

    if (debug) { // Print servo values
      Serial.println("-----------------------------------------");
      for (int i = 1; i <= 5; i++) {
        Serial.print("Servo "); Serial.println(i);
        Serial.print("Temp");Serial.print(" = "); Serial.println(readTemp(i));
        Serial.print("Posi");Serial.print(" = "); Serial.println(readPos(i));
        Serial.print("Spee");Serial.print(" = "); Serial.println(readSpeed(i));
        Serial.println("");
      }
      Serial.println("-----------------------------------------");
    }
  }
}

// Triggers on Serial event
void serialEvent() {
  if (!rx_Complete) {
    // Read serial content
    while(Serial.available() && !rx_Complete) {
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
        String result = "ack:noActionArmAndMotor?<>|\n";
        
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
        else if(rx_Msg == "headPos1?|"){
          for (int i = 6; i <= 11; i++){
            ax12a.moveSpeed(i, ax12aHeadPos[i-6][0], 50);
            delay(200);
          }
        }
        else if(rx_Msg == "headPos2?|") {
          for (int i = 6; i <= 8; i++) {
            ax12a.moveSpeed(i, ax12aHeadPos[i-6][1], 50);
          }
        }
        else if(rx_Msg == "headPos3?|") {
          for (int i = 6; i <= 8; i++) {
            ax12a.moveSpeed(i, ax12aHeadPos[i-6][2], 50);
          }
        }
        else if(rx_Msg == "eyesPos1?|") {
          for (int i = 10; i <= 11; i++){
            ax12a.moveSpeed(i, ax12aEyesPos[i-10][0], 50);
          }
        }
        else if(rx_Msg == "eyesPos2?|") {
          for (int i = 6; i <= 8; i++){
            ax12a.moveSpeed(i, ax12aEyesPos[i-10][1], 50);
          }
        }
        else if(rx_Msg == "eyesPos3?|") {
          for (int i = 6; i <= 8; i++){
            ax12a.moveSpeed(i, ax12aEyesPos[i-10][2], 50);
          }
        }        
        
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
    }
    rx_Complete = false;
  }
}

// Calculate checksum.
int checksum(String Str){
  int sum = 0;
  for(int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
  }
  return sum;
}

// Split input to output1 and output2
void valuesSplit(String input, String splitter, String &output1, String &output2) {
  output1 = "", output2 = "";
  
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == splitter) {  // Found splitter
      output1 = input.substring(0, i);
      output2 = input.substring(i+1);
      break;
    }
  }
  
  if (output1 == "") { output1 = input; } // Splitter not found
}

// Serial response functions
String respondServo() { return "ack:servo?<>|"; }

String respondServoS() { return "ack:servoS?<>|"; }

String respondServoDS() { return "ack:servoDS?<>|"; }

String respondMotor() { return "ack:motor?<>|"; }
