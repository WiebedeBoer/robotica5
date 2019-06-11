// Author Thom Smit, Wiebe de Boer, Rob Alma, Kris van den Berg en Stan van der Ploeg

// Includes
#include <Wire.h>

// Serial event
char rx_Byte = 0, rx_Byte_x;             // Last received byte
String rx_Msg = "", rx_Msg_x = "";           // Received message
String SendSum, SendSum_x;               // Checksum from raspberry
bool rx_Complete = false, rx_Complete_x = false;     // Transmission
bool ReadingCheckSum = false, ReadingCheckSum_x; // Reading chechsum

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

String modus = "0";

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
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
  delay(10);

  
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
    if(true) { // Control checksum with sendsum, for error checking. It continues when no error is found.
      String result = "\n";
      
      if(rx_Msg == "info?|"){ // info?,|10
        result = respondInfo() + String(checksum(respondInfo())) + "\n";
      }
      else if(rx_Msg == "refresh?|"){
        result = "sendRefresh?,| \n";                   // Trigger to receive modus on Serial1
        int resultLength = result.length() +1;          // Convert string to char array
        char resultarray[resultLength];
        result.toCharArray(resultarray, resultLength);  // Result to Array
        Serial1.write(resultarray);                     // Send chararray to rp

        result = respondRefresh() + String(checksum(respondRefresh())) + "\n";
      }

      int resultLength = result.length() +1;          // Convert string to char array
      char resultarray[resultLength];
      result.toCharArray(resultarray, resultLength);  // Result to Array
      Serial.write(resultarray);                      // Send chararray to rp
    }
    
    // Clean message
    rx_Msg = ""; SendSum = "";
    rx_Complete = false;
  }
}

void serialEvent1() {
  while(Serial1.available() && rx_Complete_x == false){
    rx_Byte_x = (char)Serial1.read();                 // Read next byte
    if(!ReadingCheckSum_x){ rx_Msg_x += rx_Byte_x; }  // Enter byte to message
    else { SendSum_x += rx_Byte_x; }                  // Enter byte to sendsum

    // Switch from to message to sendsum
    if(rx_Byte_x == '|'){ ReadingCheckSum_x = true; }
    
    if(rx_Byte_x == '\n'){ // End of message, cleanup
      rx_Complete_x = true;
      ReadingCheckSum_x = false;
    }
  }
  
  //execute received msg
  if(rx_Complete_x){
    String OriginalMessage = rx_Msg_x;
    int commaIndex = rx_Msg_x.indexOf(',');
    String rx_Msg_Value_x = rx_Msg_x.substring(commaIndex +1, rx_Msg_x.length() -1);
    rx_Msg_x = rx_Msg_x.substring(0, commaIndex) + "|";
    
    // checksum(rx_Msg) == SendSum.toInt()
    if(true) { // Control checksum with sendsum, for error checking. It continues when no error is found.
      if(rx_Msg_x == "modus?|"){
        modus = rx_Msg_Value_x;
      }
    }
    rx_Msg_x = ""; SendSum_x = "";
    rx_Complete_x = false;
  }
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

String respondRefresh() { return "ack:refresh?<"+ modus + ">|"; }

// Calculate checksum.
int checksum(String Str){
  int sum = 0;
  for(int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
    }
    return sum;
}
