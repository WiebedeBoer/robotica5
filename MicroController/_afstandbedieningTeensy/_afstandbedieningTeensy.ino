#include <Nextion.h>

// Define joysticks location on pins. 
#define joy1y A0  // Analog 0
#define joy1x A1  // Analog 1
#define joy1Dig 16 
#define joy2y A3  // Analog 3
#define joy2x A4  // Analog 4
#define joy2Dig 19

// Home Page
NexButton btnArm        = NexButton(0, 10, "BArm");        // Arm Button

// Arm Page
NexButton btnHome    = NexButton(1, 4, "Home");        // Home Button

// Movement Page
NexText JoyL            = NexText(1, 2, "JoyL");            // Joystick Left
NexText JoyR            = NexText(1, 3, "JoyR");            // Joystick Right
NexNumber TCSpeed       = NexNumber(1, 7, "TCSpeed");       // Numberbox for speed -- Next to slider
NexProgressBar CSpeed   = NexProgressBar(1, 5, "CSpeed");   // Speedbar Current Speed
NexSlider SSpeed        = NexSlider(1, 4, "SSPeed");        // Speedbar Slider

// Distance Page
NexNumber TDistance     = NexNumber(2, 3, "TDistance");     // Text for Distance

//Angle Page
NexNumber NAngle        = NexNumber(3, 3, "NAngle");        // Number of Angle 
NexNumber NYaw          = NexNumber(3, 3, "NYaw");          // Number of Yaw 
NexNumber NPitch        = NexNumber(3, 3, "NPitch");        // Number of Pitch 

// Mode Page
NexButton btnPitch      = NexButton(4, 3, "BPitch");        // Pitch Button -- Same as SPitch       // NOT USED -- Button declared in Nextion
NexButton btnPoortje    = NexButton(4, 4, "BPoortje");      // Poortje Button
NexButton btnChicken    = NexButton(4, 5, "BChicken");      // Chickenrun Button
NexButton btnTrap       = NexButton(4, 11, "BTrap");        // Trap Button 
NexButton btnGrind      = NexButton(4, 6, "BGrind");        // Grind Button
NexButton btnBlue       = NexButton(4, 7, "BBlauw");        // Balkje Button
NexButton btnRace       = NexButton(4, 8, "BRace");         // Race Button
NexButton btnFlag       = NexButton(4, 12, "BFlag");        // Flag Button
NexButton btnDanceSingle= NexButton(4, 9, "BDanceSingle");  // Single Dance Button
NexButton btnDanceLine  = NexButton(4, 10, "BDanceLine");   // Line Dance Button
NexButton btnLeeg       = NexButton(4, 14, "BLeeg");        // Geen Mode
NexText curModus        = NexText(4, 15, "TCModus");        // Current Modus

// QR Page
NexButton btnDuckstad   = NexButton(9, 3, "BDuckstad");
NexButton btnEindhoven  = NexButton(9, 6, "BEindhoven");
NexButton btnEibergen   = NexButton(9, 4, "BEibergen");
NexButton btnBarneveld  = NexButton(9, 7, "BBarneveld");

// Sound Page
NexButton SPitch        = NexButton(5, 3, "SPitch");        // Pitch Button -- Same as BPitch

char buffer[100] = {0};         // Buffer voor data van joystick
char rx_Byte = 0;               // Last received byte
String rx_Msg = "";             // Received message
String SendSum;                 // Checksum from raspberry
bool rx_Complete = false;       // Boolean is de transmission done
bool ReadingCheckSum = false;   // Reading chechsum

uint32_t robotspeed = 90;       // Standaard Speed -- 90
uint32_t IDistance = 0;
bool eggTrig = false;

String curMode = "Start"; // Current Mode -- Begin Mode is Start
String curQR = "Start";   // Current QR -- Begin Mode is Start
 
String JoyLtext;  // Set JoyLtext -- Used for joysticks location
String JoyRtext;  // Set JoyRtext -- Used for joysticks location
int joyLX, joyLY, joyRX, joyRY;

unsigned int updateIntervalStart = 250;
unsigned int updateInterval = updateIntervalStart;
unsigned long lastUpdateInterval = 0;

// Intialize event items for nextion
NexTouch *nex_listen_list[] = 
{
  &btnArm,
  &btnHome,
  &btnPitch,
  &btnPoortje,
  &btnChicken,
  &btnTrap,
  &btnGrind,
  &btnBlue,
  &btnRace,
  &btnFlag,
  &btnLeeg,
  &btnDanceSingle,
  &btnDanceLine,
  &btnDuckstad,
  &btnEibergen,
  &btnEindhoven,
  &btnBarneveld,
  &JoyL,
  &JoyR,
  NULL
};

void setup() {
  nexInit();              // Inialize (Connection Nextion)
  pinMode(13, OUTPUT);    // Build in LED
  Serial.begin(115200);   // Start Serial 
  Serial1.begin(9600);    // Nextion Serial
  Serial2.begin(115200);  // XBEE Serial

  // Joystick input
  pinMode(joy1Dig, INPUT);
  pinMode(joy2Dig, INPUT);
  digitalWrite(joy1Dig, HIGH);
  digitalWrite(joy2Dig, HIGH);

  // Home page
  btnArm.attachPush(btnArmPushCallback, &btnArm);
  // Arm Page
  btnHome.attachPush(btnHomePushCallback, &btnHome);
  // Mode page
  btnPitch.attachPop(btnPitchPopCallback, &btnPitch);
  btnPoortje.attachPop(btnPoortjePopCallback, &btnPoortje);
  btnChicken.attachPop(btnChickenPopCallback, &btnChicken);
  btnTrap.attachPop(btnTrapPopCallback, &btnTrap);
  btnGrind.attachPop(btnGrindPopCallback, &btnGrind);
  btnBlue.attachPop(btnBluePopCallback, &btnBlue);
  btnRace.attachPop(btnRacePopCallback, &btnRace);
  btnFlag.attachPop(btnFlagPopCallback, &btnFlag);
  btnDanceSingle.attachPop(btnDanceSiPopCallback, &btnDanceSingle);
  btnDanceLine.attachPop(btnDanceLiPopCallback,  &btnDanceLine);
  btnLeeg.attachPop(btnLeegPopCallback, &btnLeeg);

  // QR page
  btnDuckstad.attachPop(btnDuckstadPopCallback, &btnDuckstad);
  btnEindhoven.attachPop(btnEindhovenPopCallback, &btnEindhoven);
  btnEibergen.attachPop(btnEibergenPopCallback, &btnEibergen);
  btnBarneveld.attachPop(btnBarneveldPopCallback, &btnBarneveld);
  
  delay(2000);  // This delay is just in case the nextion display didn't start yet, to be sure it will receive the following command.
}

//        SET CALLBACK FUNCTIONS FOR BUTTONS OF MODE PAGE
// _________________________________________________________________
//                         BUTTONS MODE
// _________________________________________________________________

// Callback function btnArm
void btnArmPushCallback(void *ptr) { curMode = "Arm"; }

// Callback function btnHome
void btnHomePushCallback(void *ptr) { curMode = "Start"; }

// Callback function btnPitch
void btnPitchPopCallback(void *ptr) { curMode = "Pitch"; }

// Callback function btnPoortje
void btnPoortjePopCallback(void *ptr) { curMode = "Poortje"; }

// Callback funtion btnChicken
void btnChickenPopCallback(void *ptr) { curMode = "Chicken";}

// Callback function btnTrap
void btnTrapPopCallback(void *ptr){ curMode = "Trap";}

// Callback function btnGrind
void btnGrindPopCallback(void *ptr){ curMode = "Grind";}

// Callback function btnBlue
void btnBluePopCallback(void *ptr){ curMode = "Balkje";}

// Callback functie knop btnRace
void btnRacePopCallback(void *ptr){ curMode = "Race";}

// Callback functie knop btnFlag
void btnFlagPopCallback(void *ptr){ curMode = "Flag";}

// Callback functie knop BDance Single Dance
void btnDanceSiPopCallback(void *ptr){ curMode = "DanceSi";}

// Callback functie knop BPoortje Line Dance
void btnDanceLiPopCallback(void *ptr){ curMode = "DanceLi";}

// Callback functie knop BPoortje Line Dance
void btnLeegPopCallback(void *ptr){ curMode = "Start";}

//                         BUTTONS QR
// _________________________________________________________________

// Callback functie knop Duckstad 
void btnDuckstadPopCallback(void *ptr){ curQR = "Duckstad"; }   

// Callback functie knop Eindhoven 
void btnEindhovenPopCallback(void *ptr){ curQR = "Eindhoven"; }

// Callback functie knop Eibergen 
void btnEibergenPopCallback(void *ptr){ curQR = "Eibergen"; }

// Callback functie knop Barneveld 
void btnBarneveldPopCallback(void *ptr){ curQR = "Barneveld"; }

// _________________________________________________________________

void loop() {
  nexLoop(nex_listen_list);   // Loop through list of Items
  updateJoy();                // Update Joysticks functie aanroepen
  updateNextion();            // Update screen values on Nextion
  
  unsigned long currentMillis = millis(); // Current millis
  
  if (currentMillis - lastUpdateInterval >= updateInterval) { // Check wether interval passed
    lastUpdateInterval = currentMillis; // Set last check millis
    
    if (digitalRead(joy1Dig) == 0) {
      eggTrig = !eggTrig;
      Serial.println(String(eggTrig));
      updateInterval = updateInterval + 2000;
    } else {
      updateInterval = updateIntervalStart;
    }
  }
}

// Execute function
void updateNextion(){
  // Update JoyL text on Nextion
  Serial1.print("JoyL.txt=");
  Serial1.print("\"");
  Serial1.print(String(joyLX) + "," + String(joyLY));
  Serial1.print("\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  // Update JoyR text on Nextion
  Serial1.print("JoyR.txt=");
  Serial1.print("\"");
  Serial1.print(String(joyRX) + "," + String(joyRY));
  Serial1.print("\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  // Update current mode on Nextion
  Serial1.print("TCModus.txt=");
  Serial1.print("\"");
  Serial1.print(curMode);
  Serial1.print("\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

// Update joystick values
void updateJoy() {
  joyLX = map(1023-analogRead(joy2x), 0, 940, 0, 56) - 6;
  joyLY = map(analogRead(joy2y), 0, 900, 0, 56);
  joyRX = map(analogRead(joy1x), 0, 900, 0, 56);
  joyRY = map(1023-analogRead(joy1y), 0, 920, 0, 56) - 6;
}

// Return Joystick Values
String getJoy() {
  return String(joyLX) + ";" + String(joyLY) + ";" + String(joyRX) + ";" + String(joyRY); 
}

//serialEventInterupt
void serialEvent2(){
  // Read serial
  while(Serial2.available() && rx_Complete == false){
    rx_Byte = (char)Serial2.read();   //Read next byte
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

    // Checksum check;
    if(true){ // Control checksum with sendsum, for error checking. It continues when no error is found
      String result = "NoActionController?,<>|\n";
      
      if(rx_Msg == "sendRefresh?|"){
        result = "info?," + String(curMode) + ";" + getJoy() + ";" + String(eggTrig) + " \n";
      }

      Serial.print("Resonding: ");Serial.println(result);
      
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

// Return checksum value
int checksum(String Str){
  int sum = 0;
  for(unsigned int i = 0; i < Str.length();i++){
    sum += (int)Str[i];
  }
  return sum;
}
