/* Contains all functions and variables used regarding the robot arm */

String servoA = "", servoB = "";
int di, sp, pos;

void moveServo(String input) {
  // Split input to servoA and servoB
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == "&") {
      servoA = input.substring(0, i);
      servoB = input.substring(i+1);
      break;
    }
  }

  if (servoB == "") { servoA = input; } //Serial.println(servoA); }
  
  // Search seperator and move servo
  if (servoA.length() > 0) {
    for (int j = 0; j < servoA.length(); j++) {
      if (servoA.substring(j, j+1) == ";") {
        ax12a.move(servoA.substring(0,j).toInt(), servoA.substring(j+1).toInt());
        break;
      }
    }
  }
  
  // Search seperator and move servo
  if (servoB.length() > 0) {
    for (int k = 0; k < servoB.length(); k++) {
      if (servoB.substring(k, k+1) == ";") {
        ax12a.move(servoB.substring(0,k).toInt(), servoB.substring(k+1).toInt());
        break;
      }
    }
  }

  servoA = ""; servoB = "";
}

void moveServoDS(String input) {
  // Split rx_Msg_Value to servoA and servoB
  for (int i = 0; i < input.length(); i++) {
    if (input.substring(i, i+1) == "&") {
      servoA = input.substring(0, i);
      servoB = input.substring(i+1);
      break;
    }
  }

  if (servoB == "") { servoA = input; } //Serial.println(servoA); }
  
  // Search seperator and move servo
  if (servoA.length() > 0) {
    for (int j = 0; j < servoA.length(); j++) {
      if (servoA.substring(j, j+1) == ";") { // Search first seperator
        int ser = servoA.substring(0,j).toInt();
        for (int m = j; m < servoA.substring(j+1).length(); m++) {
          if(servoA.substring(m,m+1) == ";") { // Search second seperator
            di = servoA.substring(j+1, m).toInt();
            sp = servoA.substring(m+1).toInt();
            if (di==0){pos = readPos(ser);}
            else if (di==1){pos=readPos(ser)+25; if (pos > 1023){pos = 1023;}}
            else if (di==2){pos=readPos(ser)-25; if (pos < 0){pos = 0;}}
            ax12a.moveSpeed(ser, pos, sp);
          }
        }
        break;
      }
    }
  }
  
  // Search seperator and move servo
  if (servoB.length() > 0) {
    for (int j = 0; j < servoB.length(); j++) {
      if (servoB.substring(j, j+1) == ";") { // Search first seperator
        int ser = servoB.substring(0,j).toInt();
        for (int m = j; m < servoB.substring(j+1).length(); m++) {
          if(servoB.substring(m,m+1) == ";") {
            di = servoB.substring(j+1, m).toInt();
            sp = servoB.substring(m+1).toInt();
            if (di==0){pos = readPos(ser);}
            else if (di==1){pos=readPos(ser)+25;}
            else if (di==2){pos=readPos(ser)-25; if (pos < 0){pos = 0;}}
            ax12a.moveSpeed(ser, pos, sp);
          }
        }
        break;
      }
    }
  }

  servoA = ""; servoB = "";
}

String getAllPositions() {
  String response = "";
  for (int i = 1; i <= 5; i++) {
    response += i + ";" + ax12aPos[i-1];
    if (i < 5) { response += "&"; }
  }
}

int readTemp(int i) { return ax12a.readTemperature(i); }

int readPos(int i) { return ax12a.readPosition(i); }

int readVolt(int i) { return ax12a.readVoltage(i); }

int readLoad(int i) { return ax12a.readLoad(i); }

int readSpeed(int i) { return ax12a.readSpeed(i); }

/* Test function */
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
