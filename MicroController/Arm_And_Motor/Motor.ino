/* Contains all functions and variables used regarding the motors*/

String wheelL = "", wheelR = "";

void motor(String input) {
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == "&") {
      wheelL = input.substring(0, i);
      wheelR = input.substring(i+1);
      break;
    }
  }

  // Search seperator and move servo
  if (wheelL.length() > 0) {
    for (int j = 0; j < wheelL.length(); j++) {
      if (wheelL.substring(j, j+1) == ";") {
        moveWheelLeft(wheelL.substring(0,j).toInt(), wheelL.substring(j+1).toInt());
        break;
      }
    }
  }

  if (wheelR.length() > 0) {
    for (int k = 0; k < wheelR.length(); k++) {
      if (wheelR.substring(k, k+1) == ";") {
        moveWheelRight(wheelR.substring(0,k).toInt(), wheelR.substring(k+1).toInt());
        break;
      }
    }
  }
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
