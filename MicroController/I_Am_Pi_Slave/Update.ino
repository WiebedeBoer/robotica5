unsigned long maxMillis = 500;

// Distance
String updateDistance() { 
  distance = (6787.0 / (analogRead(IRPin) - 3.0)) - 4.0; 
  return String(distance);
}

// Sound
String updateMicrophone() {
  mic = analogRead(micPin);
  soundL = analogRead(soundLPin);
  soundM = analogRead(soundMPin);
  soundH = analogRead(soundHPin);
  return String(mic)+";"+String(soundL)+";"+String(soundM)+";"+String(soundH);
}

// Speed
void measureSpeed() {
  unsigned long currentMillis = millis();
  if (currentMillis - startMillis > maxMillis) {
    speedL = left * 3;
    speedR = right * 3;
    left = 0, right = 0;
    startMillis = currentMillis;
//    Serial.println(speedL);
//    Serial.println(speedR);
//    Serial.print("Total counts: ");Serial.println(count);
    count = 0;
  } else {
    if (digitalRead(speedRPin) == 0) {
      right++;
    }
    if (digitalRead(speedLPin) == 0) {
      left++;
    }

    count++;
  }
}
