
#include "AX12A.h"

#define DirectionPin  (2u)
#define BaudRate    (500000ul) // Servo baud rate Default 1000000

int ax12aPos[5] = {0,0,0,0,0};

void setup() {
  Serial.begin(115200); // Servo's Serial
   
  // Servo setup and start position
  ax12a.begin(BaudRate, DirectionPin, &Serial);
  ax12a.move(6, 530); //0 - 1023 //0 - 1000 //68mm vanaf onderkant //85mm vanaf onder tot join //nieuwe begin 512 //max 800 //min 400
  ax12a.move(7, 200); //200 min // 200 begin //max 550
  ax12a.move(8, 250); //250 begin //50 min //max 500
  ax12a.move(9, 530); //min 350 //begin 530 // max 700 
  ax12a.move(10, 100); //min 100 //max 190 //begin 190
  ax12a.move(11, 530); //min 350 //begin 530 // max 700 
}

void headLow(){
  int ax12aPos[5] = {210,820,330,240};
  checkSum(ax12aPos);
}

void headMedium(){
  int ax12aPos[5] = {512,820,580,200};
  checkSum(ax12aPos);
}

void headHigh(){
  int ax12aPos[5] = {820,510,890,500};
  checkSum(ax12aPos);
}

void checkSum(int moveToPos[]){
  int length = sizeof(moveToPos)/sizeof(int);
  for(int i = 0; i < length;i++){
      int pos = moveToPos[i];
      ax12a.move(i+6, pos);
  }
}
