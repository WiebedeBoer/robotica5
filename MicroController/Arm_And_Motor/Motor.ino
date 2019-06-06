/* Contains all functions and variables used regarding the motors*/

String wheelL = "", wheelR = "";

void motor(String input) {
  valuesSplit(input, "&", wheelL, wheelR);

  valuesSplit(wheelL, ";", di, sp);

  moveWheelLeft(di.toInt(), sp.toInt());

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
    digitalWrite(potLpin1, LOW);
    digitalWrite(potLpin2, HIGH);
  } else if (di == 2) {
    digitalWrite(potLpin1, HIGH);
    digitalWrite(potLpin2, LOW);
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
    digitalWrite(potRpin1, LOW);
    digitalWrite(potRpin2, HIGH);
  } else if (di == 2) {
    digitalWrite(potRpin1, HIGH);
    digitalWrite(potRpin2, LOW);
  }

  potR->set(speed);
}
