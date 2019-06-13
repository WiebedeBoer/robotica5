#include <Nextion.h>

#define joy1x A0
#define joy1y A1
#define joy2x A3
#define joy2y A4


//Movement Page
NexText JoyL            = NexText(1, 2, "JoyL");            // Joystick Left
NexText JoyR            = NexText(1, 3, "JoyR");            // Joystick Right
NexText curModus        = NexText(1, 3, "modus");           // Current Modus
NexNumber TCSpeed       = NexNumber(1, 7, "TCSpeed");       // Numberbox for speed -- Next to slider
NexProgressBar CSpeed   = NexProgressBar(1, 5, "CSpeed");   // Speedbar Current Speed
NexSlider SSpeed        = NexSlider(1, 4, "SSPeed");        // Speedbar Slider

//Distance Page
NexNumber TDistance     = NexNumber(2, 3, "TDistance");     // Text for Distance

//Angle Page
NexNumber NAngle        = NexNumber(3, 3, "NAngle");        // Number of Angle 
NexNumber NYaw          = NexNumber(3, 3, "NYaw");          // Number of Yaw 
NexNumber NPitch        = NexNumber(3, 3, "NPitch");        // Number of Pitch 

//Mode Page
//NexButton BPitch      = NexButton(4, 3, "BPitch");        // Pitch Button -- Same as SPitch
NexButton BPoortjes     = NexButton(4, 4, "BPoortje");      // Poortje Button
NexButton BChicken      = NexButton(4, 5, "BChicken");      // Chickenrun Button
NexButton BTrap         = NexButton(4, 11, "BTrap");        // Trap Button 
NexButton BGrind        = NexButton(4, 6, "BGrind");        // Grind Button
NexButton BBlauw        = NexButton(4, 7, "BBlauw");        // Balkje Button
NexButton BRace         = NexButton(4, 8, "BRace");         // Race Button
NexButton BFlag         = NexButton(4, 12, "BFlag");        // Flag Button
NexButton BDanceSingle  = NexButton(4, 9, "BDanceSingle");  // Single Dance Button
NexButton BDanceLine    = NexButton(4, 10, "BDanceLine");   // Line Dance Button

//Sound Page
NexButton SPitch        = NexButton(5, 3, "SPitch");        // Pitch Button -- Same as BPitch

char buffer[100] = {0};
char rx_Byte = 0;               // Last received byte
String rx_Msg = "";             // Received message
String SendSum;                 // Checksum from raspberry
bool rx_Complete = false;       // Boolean is de transmission done
bool ReadingCheckSum = false;   // Reading chechsum
uint32_t robotspeed = 90;       // Standaard Speed -- 90
uint32_t IAngle = 0;
uint32_t IPitch = 0;
uint32_t IYaw = 0;
uint32_t IDistance = 0;
uint32_t IModus;
int currentpage = 1;
bool changedModus = false;
String currentModus = "start";  // Current Mode -- Begin Mode is Start

String modus;

String JoyLtext;
String JoyRtext;
int joyLX, joyLY, joyRX, joyRY;


// List Items Nextion
NexTouch *nex_listen_list[] = 
{
  &BPoortjes,
  &BChicken,
  &BTrap,
  &BGrind,
  &BBlauw,
  &BRace,
  &BFlag,
  &BDanceSingle,
  &BDanceLine,
  &JoyL,
  &JoyR,
  NULL
};

void setup() {
  nexInit();
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(9600);
  //BPoortje.attachPop(BPoortjePopCallback);
  Serial2.begin(115200);


  // Button attachPops
  // If Button is pressed go to the function assigned 
  BPoortjes.attachPop(BPoortjesPopCallback, &BPoortjes);
  BChicken.attachPop(BChickenPopCallback, &BChicken);
  BTrap.attachPop(BTrapPopCallback, &BTrap);
  BGrind.attachPop(BGrindPopCallback, &BGrind);
  BBlauw.attachPop(BBlauwPopCallback, &BBlauw);
  BRace.attachPop(BRacePopCallback, &BRace);
  BFlag.attachPop(BFlagPopCallback, &BFlag);
  BDanceSingle.attachPop(BDanceSiPopCallback, &BDanceSingle);
  BDanceLine.attachPop(BDanceLiPopCallback,  &BDanceLine);



  
  //delay(2000);  // This delay is just in case the nextion display didn't start yet, to be sure it will receive the following command.
}

// Callback functie knop BPoortje
void BPoortjesPopCallback(void *ptr) 
{
  currentModus = "Poortje";
  Serial.println(currentModus);
}

// Callback functie knop BChicken
void BChickenPopCallback(void *ptr) 
{
  currentModus = "Chicken";
  Serial.println(currentModus);
}

// Callback functie knop BTrap
void BTrapPopCallback(void *ptr)
{
  currentModus = "Trap";
  Serial.println(currentModus);
}

// Callback functie knop BGrind
void BGrindPopCallback(void *ptr){
  currentModus = "Grind";
  Serial.println(currentModus);
}

// Callback functie knop BBlauw
void BBlauwPopCallback(void *ptr){
  currentModus = "Balkje";
  Serial.println(currentModus);
}

// Callback functie knop BRace
void BRacePopCallback(void *ptr){
  currentModus = "Race";
  Serial.println(currentModus);
}

// Callback functie knop BFlag
void BFlagPopCallback(void *ptr){
  currentModus = "Flag";
  Serial.println(currentModus);
}

// Callback functie knop BDance Single Dance
void BDanceSiPopCallback(void *ptr){
  currentModus = "DanceSi";
  Serial.println(currentModus);
}

// Callback functie knop BPoortje Line Dance
void BDanceLiPopCallback(void *ptr){
  currentModus = "DanceLi";
  Serial.println(currentModus);
}

void loop() {
//Execute_AfstandBediening();
  nexLoop(nex_listen_list); //Loop through list of Items
  updateJoy();
  //delay(10);
}

// Set Joystick Buttons
void updateJoy() {
  joyLX = analogRead(joy1x);
  joyLY = analogRead(joy1y);
  joyRX = analogRead(joy2x);
  joyRY = analogRead(joy2y);
}

// 
String getJoy() {
  return String(joyLX) + ";" + String(joyLY) + ";" + String(joyRX) + ";" + String(joyRY);
}

String Respond_AfstandBediening(){
    return  "ack:refresh?<"+JoyLtext +":"+ JoyRtext+">|" ;
}

void Execute_AfstandBediening(){
    JoyLtext = String(joyLX/16) + "," + String(joyLY/16);
    JoyLtext.toCharArray(buffer, JoyLtext.length());
    JoyL.setText(buffer);
    JoyRtext = String(joyRX/16) + "," + String(joyRY/16);
    JoyRtext.toCharArray(buffer, JoyRtext.length());
    JoyR.setText(buffer);
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
  for(unsigned int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
  }
  return sum;
}
