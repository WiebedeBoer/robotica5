void setup()  {
  Serial.begin(9600);
  Serial.println("start");
  }
 
 
 
void loop()                     // run over and over again
{
  // kijk of er input is op de serial poort
  while (Serial.available()) {
      //print input zodat deze te zien is met de monitor
      Serial.print((char)Serial.read());
  }
  //wacht 100 ms
  delay(100);
}
