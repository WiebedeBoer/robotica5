bool drivingL, drivingR;

void setup() { }

int lastJoyX = 550;
int lastJoyY = 550;
int joyX, joyY, joyButton;

int genSpeed, leftSpeed, rightSpeed;
void loop()
{
  // Read joystick value
  joyX = analogRead(inputJoyX);
  joyY = analogRead(inputJoyY);
  joyButton = digitalRead(inputJoyButton);

  if (abs(joyX - lastJoyX) > 10 || abs(joyY - lastJoyY) > 10) {    
    lastJoyX = joyX;
    lastJoyY = joyY;

    genSpeed = joyY - 510;
    leftSpeed = joyX - 523 + genSpeed;

    int speed = abs(leftSpeed) / 3.125;

    Serial.print("Speed: ");
    Serial.println(speed);
    if (leftSpeed > 0) {
      Serial.println("Vooruit");
      wheelLeft(speed, 1);
    } else {
      Serial.println("Acteruit");
      wheelLeft(speed, 2);
    }
    
    Serial.println(leftSpeed);
  }

  Serial.print("Joy X: "); Serial.println(joyX);
  Serial.print("Joy Y: "); Serial.println(joyY);
  Serial.print("Joy Button: "); Serial.println(joyButton);
  Serial.println("");
  delay(500);
}

//sp = speed, value between 0-128
//direction = forward, backward or stand stil
//direction = forward: 1, backward: 2, stil: 0
void wheelLeft(int sp, int direction) {
  Serial.println("Wheelleft Adjustment");
  if (direction == 0) {
    drivingL = false;
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    return;
  }

  if (direction == 1) {
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    Serial.println("Directiona");
  } else if (direction == 2) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    Serial.println("Picobello");
  }

  pot->set(sp);
}

//  if (
//  wheelLeft(1, 10);
//  delay(2500);
//  wheelLeft(0, 0);
//  delay(2500);
//  wheelLeft(2, 25);
//  delay(2500);
//  wheelLeft(0, 0);
//  delay(2500);
  
//  digitalWrite(3, LOW);
//  digitalWrite(4, HIGH);
//  i = 0;
//  delay(1000);
// // pot->set(70);//0-128
//  for (i = 0; i <= 128; i++)
//  {
//    pot->set(i);
//    delay(100);
//  }
//  //delay(500);
//  for (i = 128; i >= 0; i--)
//  {
//    pot->set(i);
//    delay(100);
//  }
//
//  Serial.println("Hello world");
//  delay(300);
