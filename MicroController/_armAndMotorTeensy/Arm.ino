/* Contains all functions and variables used regarding the robot arm */

String servoA = "", servoB = "";
String id, di, pos, sp;

int maxPositions[5][2] = {{400,800}, {35,680}, {65, 450}, {350, 950}, {185, 251}};

void checkPosition(int id, String &target) {
  if (target.toInt() < maxPositions[id-1][0]) {
    target = String(maxPositions[id-1][0]);
  }
  else if (target.toInt() > maxPositions[id-1][1]) {
    target = String(maxPositions[id-1][1]);
  }
}

// Move servo with id and position
void moveServo(String input) {
  valuesSplit(input, "&", servoA, servoB);

  if (debug) { Serial.print("servoA: "); Serial.println(servoA); }
  valuesSplit(servoA, ";", id, pos);

  if (debug) { Serial.print("Id / Pos: ");Serial.print(id);Serial.print(" / ");Serial.println(pos); }
  checkPosition(id.toInt(), pos);
  ax12a.moveRW(id.toInt(), pos.toInt());

  if (debug) { Serial.print("servoB: "); Serial.println(servoB); }
  valuesSplit(servoB, ";", id, pos);

  if (debug) { Serial.print("Id / Pos: ");Serial.print(id);Serial.print(" / ");Serial.println(pos); }
  checkPosition(id.toInt(), pos);
  ax12a.moveRW(id.toInt(), pos.toInt());
  
  ax12a.action();

  id = ""; pos = "";
}

// Move servo with id, direction and speed
void moveServoDS(String input) {
  valuesSplit(input, "&", servoA, servoB);
  
  if (debug) { Serial.print("servoA: "); Serial.println(servoA); }
  valuesSplit(servoA, ";", id, pos);
  valuesSplit(pos, ";", pos, sp);

  if (pos.toInt() == 0) { pos = String(ax12aPos[id.toInt()-1]); }
  else if (pos.toInt() == 1) { pos = String(ax12aPos[id.toInt()-1]+25); }
  else if (pos.toInt() == 2) { pos = String(ax12aPos[id.toInt()-1]-25); }
  if (debug) { Serial.print("Id / Pos / Speed: ");Serial.print(id);Serial.print(" / ");Serial.print(pos);Serial.print(" / ");Serial.println(sp); }
  checkPosition(id.toInt(), pos);
  ax12a.moveSpeedRW(id.toInt(), pos.toInt(), sp.toInt());

  if (debug) { Serial.print("servoB: "); Serial.println(servoB); }
  valuesSplit(servoB, ";", id, pos);
  valuesSplit(pos, ";", pos, sp);

  if (pos.toInt() == 0) { pos = String(ax12aPos[id.toInt()-1]); }
  else if (pos.toInt() == 1) { pos = String(ax12aPos[id.toInt()-1]+25); }
  else if (pos.toInt() == 2) { pos = String(ax12aPos[id.toInt()-1]-25); }
  if (debug) { Serial.print("Id / Pos / Speed: ");Serial.print(id);Serial.print(" / ");Serial.print(pos);Serial.print(" / ");Serial.println(sp); }
  checkPosition(id.toInt(), pos);
  ax12a.moveSpeedRW(id.toInt(), pos.toInt(), sp.toInt());

  ax12a.action();

  servoA = ""; servoB = "";
}

// Move servo with id, position and speed
void moveServoS(String input) {
  valuesSplit(input, "&", servoA, servoB);
  
  if (debug) { Serial.print("servoA: "); Serial.println(servoA); }
  valuesSplit(servoA, ";", id, pos);
  valuesSplit(pos, ";", pos, sp);
  if (debug) { Serial.print("Id / Pos / Speed: ");Serial.print(id);Serial.print(" / ");Serial.print(pos);Serial.print(" / ");Serial.println(sp); }
  checkPosition(id.toInt(), pos);
  ax12a.moveSpeedRW(id.toInt(), pos.toInt(), sp.toInt());

  if (debug) { Serial.print("servoB: "); Serial.println(servoB); }
  valuesSplit(servoB, ";", id, pos);
  valuesSplit(pos, ";", pos, sp);
  if (debug) { Serial.print("Id / Pos / Speed: ");Serial.print(id);Serial.print(" / ");Serial.print(pos);Serial.print(" / ");Serial.println(sp); }
  checkPosition(id.toInt(), pos);
  ax12a.moveSpeedRW(id.toInt(), pos.toInt(), sp.toInt());
  
  ax12a.action();

  servoA = ""; servoB = "";
}

// Return string with all servo positions
String getAllPositions() {
  String response = "";
  for (int i = 1; i <= 5; i++) {
    response += i + ";" + ax12aPos[i-1];
    if (i < 5) { response += "&"; }
  }
  return response;
}

// Functions to read servo values 
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
