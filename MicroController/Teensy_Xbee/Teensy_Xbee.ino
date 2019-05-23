//Author Thom Smit
//WIP template for communication between arduino and raspberrypi
#include <Wire.h>
#include <MPU6050.h>

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
unsigned long timer = 0;
float timestep = 0.01;

float pitch = 0;
float roll = 0;
float yaw = 0;

char rx_Byte = 0;//last received byte
String rx_Msg = "";//received message
bool rx_Complete = false;//is de transmission done
char rx_Byte2 = 0;//last received byte
String rx_Msg2 = "";//received message
bool rx_Complete2 = false;//is de transmission done

int SpeedIn = 12;
int Speed;

void setup() {
  //ini serial interface
  Serial.begin(115200);
  Serial1.begin(115200);
  //demo pin, change this.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SpeedIn,INPUT);

  //INI accelgyro
     while(!accelgyro.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  accelgyro.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
accelgyro.setThreshold(3);

}


void loop() {

  Refresh();
  //Message received do command
  if(rx_Complete){
    //Serial.println("Command:"+rx_Msg+"Sum:" +SendSum + "CalculatedSum:" + String(checksum(rx_Msg))); //print contains all useful debug information
    rx_Msg = rx_Msg + '\n';
    int lenght_msg = rx_Msg.length() +1;
    char msg_char[lenght_msg];
    rx_Msg.toCharArray(msg_char,lenght_msg);
    
    Serial1.write(msg_char);
     rx_Msg = "";
     rx_Complete = false;
   }
   if(rx_Complete2){
    //Serial.println("Command:"+rx_Msg+"Sum:" +SendSum + "CalculatedSum:" + String(checksum(rx_Msg))); //print contains all useful debug information
    rx_Msg2 = rx_Msg2 + '\n';
    int lenght_msg = rx_Msg2.length() +1;
    char msg_char[lenght_msg];
    rx_Msg2.toCharArray(msg_char,lenght_msg);
    Serial.write(msg_char);
    rx_Msg2 = "";
    rx_Complete2 = false;
   }
  //clean message   
}

void Refresh(){
  //Speed = digitalRead(SpeedIn);
  Serial.print(Speed);  
      Vector norm = accelgyro.readNormalizeGyro();

      pitch = pitch + norm.YAxis
}
//serialEventInterupt
void serialEvent(){
  while(Serial.available()){
    rx_Byte = (char)Serial.read();//Read next byte
    rx_Msg += rx_Byte;   
    if(rx_Byte == '\n'){//endling and cleanup
      rx_Complete = true;
      }
  }
}
void serialEvent1(){
  while(Serial1.available()&& rx_Complete2 == false){
    rx_Byte2 = (char)Serial1.read();//Read next byte
    rx_Msg2 += rx_Byte2;   
    if(rx_Byte2 == '\n'){//endling and cleanup
      rx_Complete2 = true;
      }
  }
}
