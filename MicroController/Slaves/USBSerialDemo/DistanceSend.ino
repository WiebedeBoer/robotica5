//Author Thom Smit and co author editor Wiebe de Boer
//WIP template for communication between arduino and raspberrypi

//for serial
char rx_Byte = 0;//last received byte
String rx_Msg = "";//received message
String SendSum;//checksum from raspberry
bool rx_Complete = false;//is de transmission done
bool ReadingCheckSum = false;//reading chechsum

bool LedOnOff;

//gate
//bool NearGate;
int AfstandsSensor = 0;
int GateDistance =0;

void setup() {
  //ini serial interface
  Serial.begin(115200);
  //demo pin, change this.
  pinMode(LED_BUILTIN, OUTPUT);
}

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

   }
  //clean message   
  rx_Msg = "";
  SendSum = "";
  rx_Complete = false;
}
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
//distance sensor read
void distance(int GateDistance){
    //afstandsensor lezen
  AfstandsSensor = analogRead(A0);
  Serial.println(AfstandsSensor);
      delay(150);
  
  }
