void setup()  {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("Goodnight moon!");
  }
 
 
 
void loop()                     // run over and over again
{
  bool res = false;
  while (Serial.available()) {
      Serial.print((char)Serial.read());
      res = true;
  }
  if (res)
  Serial.print("\n");
  delay(100);
}
