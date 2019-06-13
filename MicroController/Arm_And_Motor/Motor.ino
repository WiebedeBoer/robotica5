/* Contains all functions and variables used regarding the motors*/

String wheelL = "", wheelR = "";

void motor(String input) {
  valuesSplit(input, "&", wheelL, wheelR);

  if (debug) {Serial.println(wheelL);}
  valuesSplit(wheelL, ";", di, sp);
  
  moveWheelLeft(di.toInt(), sp.toInt());

  if (debug) {Serial.println(wheelR);}
  valuesSplit(wheelR, ";", di, sp);
  
  moveWheelRight(di.toInt(), sp.toInt());
}

void moveWheelLeft(int di, int speed) {
  if (di == 0) {
    drivingL = false;
    digitalWrite(potLpin1, LOW);
    digitalWrite(potLpin2, LOW);
    return;
  }
  
  drivingL = true;

  if (di == 1) {
    if (debug) {Serial.println("Forward L");}
    digitalWrite(potLpin1, LOW);
    digitalWrite(potLpin2, HIGH);
  } else if (di == 2) {
    if (debug) {Serial.println("Back L");}
    digitalWrite(potLpin1, HIGH);
    digitalWrite(potLpin2, LOW);
  } else {
    if (debug) { Serial.println("did nothing L");}
  }
  
  potL->set(speed);
}

void moveWheelRight(int di, int speed) {
  if (di == 0) {
    drivingR = false;
    digitalWrite(potRpin1, LOW);
    digitalWrite(potRpin2, LOW);
    return;
  }
  
  drivingR = true;

  if (di == 1) {
    if (debug) {Serial.println("Forward R"); }
    digitalWrite(potRpin1, LOW);
    digitalWrite(potRpin2, HIGH);
  } else if (di == 2) {
    if (debug) {Serial.println("Back R"); }
    digitalWrite(potRpin1, HIGH);
    digitalWrite(potRpin2, LOW);
  } else {
    if (debug) { Serial.println("did nothing R");}
  }

  potR->set(speed);
}
