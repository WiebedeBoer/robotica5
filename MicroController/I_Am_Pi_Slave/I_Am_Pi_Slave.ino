// Author Thom Smit en Wiebe de Boer en Rob Alma en Kris
// WIP template for communication between arduino and raspberrypi

// Includes
#include <Wire.h>

// Serial event
char rx_Byte = 0;             // Last received byte
String rx_Msg = "";           // Received message
String SendSum;               // Checksum from raspberry
bool rx_Complete = false;     // Transmission
bool ReadingCheckSum = false; // Reading chechsum

// Initializing all variables
#define IRPin 20                // Sensor value distance analog
float distance;

#define micPin 14             // Microphone
#define soundLPin 15          // Geluid tonen laag
#define soundMPin 16          // Geluid tonen midden
#define soundHPin 17          // Geluid tonen hoog
int mic, soundL, soundM, soundH;

#define speedRPin 13          // Sensor value speed right digital
#define speedLPin 12          // Sensor value speed left digital
int speedL, speedR;
int maxMillis = 500;
unsigned long startMillis = millis();
int left = 0, right = 0, count = 0;

unsigned long timer = 0;
float timestep = 0.01;

String modus = "";

void setup() {
  Serial.begin(115200);

  pinMode(speedRPin, INPUT);
  pinMode(speedLPin, INPUT);
  digitalWrite(speedRPin, LOW);
  digitalWrite(speedLPin, LOW);
}

//loop
void loop() {
  updateDistance();
  measureSpeed();
  updateMicrophone();
//  delay(2500);

  
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

void serialEvent() {
  while(Serial.available() && rx_Complete == false){
    rx_Byte = (char)Serial.read(); // Read next byte
    
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
    
    // checksum(rx_Msg) == SendSum.toInt()
    if(true) { //control checksum with sendsum, for error checking. It continues when no error is found.
      String result = "";
      if(rx_Msg == "info?|"){ // info?,|10
        result = respondInfo() + String(checksum(respondInfo())) + "\n";
      }
      else if(rx_Msg == "refresh?|"){
        Serial1.write("refresh?,02490;4324|");
        Serial.println(modus);
        result = "ack:RefreshMessageSend|?<" + modus + ">\n";
      }
      else {
        result = "ack:noAction?<>|\n";
      }
      
      int resultLength = result.length() +1;          // Convert string to char array
      char resultarray[resultLength];
      result.toCharArray(resultarray, resultLength);  // Result to Array
      Serial.write(resultarray);                      // Send chararray to rp
    }
    
    // Clean message
    rx_Msg = ""; SendSum = "";
  }
  
  rx_Complete = false;
}

void serialEvent1() {
  Serial.print("Got something from that xbee");
  while(Serial1.available() && rx_Complete == false){
    rx_Byte = (char)Serial1.read(); // Read next byte
    
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
    Serial.print("Got something from that xbee");
    String OriginalMessage = rx_Msg;
    int commaIndex = rx_Msg.indexOf(',');
    String rx_Msg_Value = rx_Msg.substring(commaIndex +1, rx_Msg.length() -1);
    rx_Msg = rx_Msg.substring(0, commaIndex) + "|";
    
    // checksum(rx_Msg) == SendSum.toInt()
    if(true) { //control checksum with sendsum, for error checking. It continues when no error is found.
      if(rx_Msg == "refresh?|"){
        modus = rx_Msg_Value;
      }
    }
  }
  
  rx_Complete = false;
}

String getDistance() {
  return String(distance);
}

String getSpeed() {
  return String(speedL) + ";" + String(speedR);
}

String getMicrophone() {
  return String(soundL) + ";" + String(soundM) + ";" + String(soundH);
}

// return String: distance, speed, microphone
String respondInfo() { return "ack:info?<"+ getDistance() + ";" + getSpeed() + ";" + getMicrophone() + ">|"; }

// Calculate checksum.
int checksum(String Str){
  int sum = 0;
  for(int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
    }
    return sum;
}
