#include <Nextion.h>
#include <NexProgressBar.h>
#define joy1x A0
#define joy1y A1
#define joy2x A3
#define joy2y A4

NexText JoyL = NexText(0, 2, "JoyL"); //Joystick Left
NexText JoyR = NexText(0, 3, "JoyR"); //Joystick Right
NexNumber TCSpeed = NexNumber(1, 7, "TCSpeed"); //Numberbox for speed -- Next to slider
NexNumber NAngle = NexNumber(3, 3, "NAngle"); //Number of Angle 
NexNumber NYaw = NexNumber(3, 3, "NYaw"); //Number of Yaw 
NexNumber NPitch = NexNumber(3, 3, "NPitch"); //Number of Pitch 
NexSlider SSpeed = NexSlider(1, 4, "SSPeed"); //Speedbar Slider
NexNumber TDistance = NexNumber(2, 3, "TDistance"); //Distance
NexProgressBar CSpeed  = NexProgressBar(1, 5, "CSpeed"); //Speedbar Current Speed

char buffer[100] = {0};
char rx_Byte = 0;//last received byte
String rx_Msg = "";//received message
String SendSum;//checksum from raspberry
bool rx_Complete = false;//is de transmission done
bool ReadingCheckSum = false;//reading chechsum
uint32_t robotspeed = 90;
uint32_t IAngle = 0;
uint32_t IPitch = 0;
uint32_t IYaw = 0;
uint32_t IDistance = 0;

String JoyLtext;
String JoyRtext;

NexTouch *nex_listen_list[] = 
{
  &JoyL,  
  &JoyR,
  &SSpeed,
  &TCSpeed,
  NULL  // String terminated
};

void setup() {
  pinMode(13, OUTPUT);
  Serial2.begin(115200);
  Serial1.begin(9600);
  delay(2000);  // This dalay is just in case the nextion display didn't start yet, to be sure it will receive the following command.
  Serial1.print("baud=115200"); 
  Serial1.write(0xff);  // We always have to send this three lines after each command sent to nextion.
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.end();  // End the serial comunication of baud=9600
  Serial1.begin(115200);  // Start serial comunication at baud=115200
  }

void loop() {
  //Message received do command  
  Execute_AfstandBediening();
  nexLoop(nex_listen_list);
}

String Respond_AfstandBediening(){
    return  "ack:refresh?<"+JoyLtext +":"+ JoyRtext+">|" ;
}

void Execute_AfstandBediening(){
    JoyLtext = String(analogRead(joy1x)/16) + "," + String(analogRead(joy1y)/16);
    JoyLtext.toCharArray(buffer, JoyLtext.length());
    JoyL.setText(buffer);
    JoyRtext = String(analogRead(joy2x)/16) + "," + String(analogRead(joy2y)/16);
    JoyRtext.toCharArray(buffer, JoyRtext.length());
    JoyR.setText(buffer);
    TCSpeed.setValue(robotspeed); //Set TCSpeed number part to the same as CSpeed value. 
    NAngle.setValue(IAngle);
    NYaw.setValue(IYaw);
    NPitch.setValue(IPitch);
    TDistance.setValue(IDistance);
       
}

//serialEventInterupt
void serialEvent2(){
  while(Serial2.available() && rx_Complete == false){
    rx_Byte = (char)Serial2.read();//Read next byte
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
      //Serial.println(rx_Msg);
      ReadingCheckSum = false;
      }
  }
  //execute received msg
      if(rx_Complete){
      String OriginalMessage = rx_Msg;
      int commaIndex = rx_Msg.indexOf(',');
      String rx_Msg_Speed = rx_Msg.substring(commaIndex +1, rx_Msg.length() -1);
      rx_Msg = rx_Msg.substring(0, commaIndex) + "|";      
      if(checksum(OriginalMessage) == SendSum.toInt()){ //control checksum with sendsum, for error checking. It continues when no error is found.
      //possible commands and code here. KEEP IT SHORT the RP waits until its finished.
        if(rx_Msg == "refresh?|"){
          String result = Respond_AfstandBediening(); // returns ack:refresh?<var1:var2>|
          String Checksum = String(checksum(result));
          
          robotspeed =  rx_Msg_Speed.toInt(); // 
          result = result + Checksum + "\n";
          int resultLength = result.length() +1; // convert string to char array
          char resultarray[resultLength];
          result.toCharArray(resultarray, resultLength);
          Serial2.write(resultarray);//send chararray to rp
          }                
     }
    //clean message   
    rx_Msg = "";
    SendSum = "";
    rx_Complete = false;
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
