#include <Nextion.h>
#include <NexProgressBar.h>
#include <NexButton.h>

#define joy1x A0
#define joy1y A1
#define joy2x A3
#define joy2y A4

//Movement Page
NexText JoyL = NexText(0, 2, "JoyL"); //Joystick Left
NexText JoyR = NexText(0, 3, "JoyR"); //Joystick Right
NexText curModus = NexText(0, 3, "modus"); //Joystick Right
NexNumber TCSpeed = NexNumber(1, 7, "TCSpeed"); //Numberbox for speed -- Next to slider
NexProgressBar CSpeed  = NexProgressBar(1, 5, "CSpeed"); //Speedbar Current Speed
NexSlider SSpeed = NexSlider(1, 4, "SSPeed"); //Speedbar Slider

//Distance Page
NexNumber TDistance = NexNumber(2, 3, "TDistance"); //Distance

//Angle Page
NexNumber NAngle = NexNumber(3, 3, "NAngle"); //Number of Angle 
NexNumber NYaw = NexNumber(3, 3, "NYaw"); //Number of Yaw 
NexNumber NPitch = NexNumber(3, 3, "NPitch"); //Number of Pitch 

//Mode Page
NexButton BPitch = NexButton(4, 3, "BPitch"); //Pitch Button -- Same as SPitch
NexButton BPoortje = NexButton(4, 4, "BPoortje"); // Poortje Button
NexButton BChicken = NexButton(4, 5, "BChicken"); // Chickenrun Button
NexButton BTrap = NexButton(4, 11, "BTrap"); // Trap Button
NexButton BGrind = NexButton(4, 7, "BGrind"); // Grind Button
NexButton BBlauw = NexButton(4, 8, "BBlauw"); // Balkje Button
NexButton BRace = NexButton(4, 9, "BRace"); // Race Button
NexButton BFlag = NexButton(4, 11, "BFlag"); //Flag Button
NexButton BDanceSingle = NexButton(4, 9, "BDanceSingle"); // Single Dance Button
NexButton BDanceLine = NexButton(4, 10, "BDanceLine"); // Line Dance Button

//Sound Page
NexButton SPitch = NexButton(5, 3, "SPitch"); // Pitch Button -- Same as BPitch

//Head Page

//Light Page

//Arm Page

//QR Page

//Pitch Page


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
uint32_t IModus;

bool modusChanged = false;

String modus;

String JoyLtext;
String JoyRtext;
int joyLX, joyLY, joyRX, joyRY;

NexTouch *nex_listen_list[] = 
{
  &JoyL,  
  &JoyR,
  &SSpeed,
  &TCSpeed,
  &BPitch,
  &BPoortje,
  NULL  // String terminated
};

void setup() {
  pinMode(13, OUTPUT);
  Serial2.begin(115200);
  delay(2000);  // This delay is just in case the nextion display didn't start yet, to be sure it will receive the following command.
}

void loop() {
  Execute_AfstandBediening();
  nexLoop(nex_listen_list);
  updateJoy();
  delay(10);
}

void updateJoy() {
  joyLX = analogRead(joy1x);
  joyLY = analogRead(joy1y);
  joyRX = analogRead(joy2x);
  joyRY = analogRead(joy2y);
}

String getJoy() {
  return String(joyLX) + ";" + String(joyLY) + ";" + String(joyRX) + ";" + String(joyRY);
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
    BPoortje.attachPop(BPoortjePressed, &BPoortje);
}
/**
 * Button to return the response
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer
 */
void BPoortjePressed(void *ptr){
  NexButton *BPoortjes = (NexButton *)ptr;
  memset(buffer, 0, sizeof(buffer));
  BPoortjes -> getText(buffer, sizeof(buffer));
  if (strcmp(buffer,"ON"))
  {
    IModus = 1;
  }
}

void setModus(String modus) {
  Serial.print("modus is nu: ");Serial.println(modus);
}
void moduschoise(){
  if(IModus == 1){
    //Modustest = "Poortje";
    Serial.println("Test123"); 
  }
}

//serialEventInterupt
void serialEvent2(){
  while(Serial2.available() && rx_Complete == false){
    rx_Byte = (char)Serial2.read();//Read next byte
    if(ReadingCheckSum == false){//enter byte to message
      rx_Msg += rx_Byte;
    }
    else{//enter byte to sendsum
      SendSum += rx_Byte;
    }
        
    if(rx_Byte == '|'){ // Switch from to message to sendsum
      ReadingCheckSum = true;
    }
    
    if(rx_Byte == '\n'){ // End of line, cleanup
      rx_Complete = true;
      ReadingCheckSum = false;
    }
  }
  
  // Execute received msg
  if(rx_Complete){
    String OriginalMessage = rx_Msg;
    int commaIndex = rx_Msg.indexOf(',');
    String rx_Msg_Speed = rx_Msg.substring(commaIndex +1, rx_Msg.length() -1);
    rx_Msg = rx_Msg.substring(0, commaIndex) + "|";

    // checksum(OriginalMessage) == SendSum.toInt()
    if(true){ // Control checksum with sendsum, for error checking. It continues when no error is found
      String result = "NoAction?,<>|\n";
      
      if(rx_Msg == "sendRefresh?|"){
        //result = "modus?,<" + modus[i] + "?";
        result = "modus?," + String(modus) + ";" + getJoy() + "\n";
        modus = String(modus.toInt()+1);
        //result = Respond_AfstandBediening() + String(checksum(result)) + "\n";
      }
      
      int resultLength = result.length() +1; // Convert string to char array
      char resultarray[resultLength];
      result.toCharArray(resultarray, resultLength);
      Serial2.write(resultarray);// Send chararray to rp
      
   }
     
    // Clear message
    rx_Msg = ""; SendSum = "";
    rx_Complete = false;
  }
}

int checksum(String Str){
  int sum = 0;
  for(int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
  }
  return sum;
}
