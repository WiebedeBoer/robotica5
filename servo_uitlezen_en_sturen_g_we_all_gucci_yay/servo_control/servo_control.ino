#include "AX12A.h"

#define DirectionPin  (2u)
#define BaudRate    (500000ul) // <--- Servo baud rate Default 1000000
//  Please change Arduino Serial monitor baud rate to 57600

//  If  your Servo baud rate 1000000
//  Please change Arduino Serial monitor baud rate to 1000000

unsigned long previousMillis = 0;

const long interval = 2500;

bool initDown = true;

void setup()
{
  ax12a.begin(BaudRate, DirectionPin, &Serial1);
  ax12a.move(1, 200);
  ax12a.move(2, 200);
}

void loop()
{
  unsigned long currentMillis = millis();

  //Serial.println("Initiating downwards");
  
  //Temp check
  if (readTemp(1) > 70) { // Quit script, servo 1 overheated
    
  }
  
  downwards();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    Serial.println("-----------------------------------------");
    // Get info servo's
    for (int i = 1; i <= 4; i++) {
      Serial.print("Servo "); Serial.println(i);
      Serial.print("Temp");Serial.print(" = "); Serial.println(readTemp(i));
      Serial.print("Posi");Serial.print(" = "); Serial.println(readPos(i));
      Serial.print("Volt");Serial.print(" = "); Serial.println(readVolt(i));
      Serial.print("Load");Serial.print(" = "); Serial.println(readLoad(i));
      Serial.print("Spee");Serial.print(" = "); Serial.println(readSpeed(i));
      Serial.println("");
    }
    Serial.println("-----------------------------------------");
  }

}

void downwards() {
  int aServo = 1, bServo = 2;
  int aPos = 300, bPos = 300;
  int aSpeed = 50, bSpeed = 20;
  
  //Setup arm movement position
  ax12a.moveSpeedRW(aServo, aPos, aSpeed);
  ax12a.moveSpeedRW(bServo, bPos, bSpeed);
  //Start arm movement
  ax12a.action();

  Serial.println("Starting while");
  Serial.println("Starting while");
  Serial.println("Starting while");
  Serial.println("Starting while");
  Serial.println("Starting while");
  while (true) {
    Serial.println(readPos(aServo));
    Serial.println(readPos(bServo));
    if (readPos(aServo) >= aPos-10 && readPos(bServo) >= bPos-10) { break; }
  }

  Serial.println("Resetting in");
  for (int d = 3; d > 0; d--) {
     Serial.println(d);
    delay(1000);
  }

  ax12a.move(aServo, 200);
  ax12a.move(bServo, 200);

  while (true) {
    if (readPos(aServo) < 510 && readPos(bServo) < 510) { break; }
  }

  Serial.println("Reset succes");
  delay(2500);
}

/* function AX12A.h
  int readTemperature(unsigned char ID);
  int readVoltage(unsigned char ID);
  int readPosition(unsigned char ID);
  int readSpeed(unsigned char ID);
  int readLoad(unsigned char ID);
*/

int readTemp(int i) { return ax12a.readTemperature(i); }

int readPos(int i) { return ax12a.readPosition(i); }

int readVolt(int i) { return ax12a.readVoltage(i); }

int readLoad(int i) { return ax12a.readLoad(i); }

int readSpeed(int i) { return ax12a.readSpeed(i); }

void runUnderTemp() {
  for (int i = 1; i <= 4; i++) {
    int temp = ax12a.readTemperature(i);
    //Serial.println(temp);
    if (temp < 40) {
      ax12a.setEndless(i, ON);
      ax12a.turn(i, RIGHT, 800);
    } else {
      ax12a.setEndless(i, OFF);
    }
  }

  Serial.println("\n");
  delay(5000);
}
void move() {
  ax12a.move(1, 200);//ID, position 0-1023 dat is 0-300graden
  delay(1000);
  ax12a.move(1, 600);
  delay(3000);

  //reg = ax12a.readPosition((1u));

  //Serial.println(reg);

  ax12a.move(2, 200);
  delay(1000);
  ax12a.move(2, 600);
  delay(3000);

  ax12a.move(3, 200);
  delay(1000);
  ax12a.move(3, 600);
  delay(3000);

  ax12a.move(4, 200);
  delay(1000);
  ax12a.move(4, 600);
  delay(3000);
}

void moveRW() {
  ax12a.moveRW(1, 200);
  ax12a.moveRW(2, 200);
  ax12a.moveRW(3, 200);
  ax12a.moveRW(4, 200);
  ax12a.action();

  delay(1000);

  ax12a.moveRW(1, 600);
  ax12a.moveRW(2, 600);
  ax12a.moveRW(3, 600);
  ax12a.moveRW(4, 600);
  ax12a.action();

  delay(1000);
}
