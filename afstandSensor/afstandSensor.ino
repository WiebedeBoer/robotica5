// Include files.
#include <SPI.h>      // Ethernet shield uses SPI-interface
#include <Ethernet.h> // Ethernet library use Ethernet2.h for new ethernet shield v2

bool BijBakje;
int AfstandsSensor = 0;

void setup() {
  // put your setup code here, to run once:
  //readSensors();
}

void loop() {
  // put your main code here, to run repeatedly:

  //afstandsensor lezen
  AfstandsSensor = analogRead(A0);
  Serial.println(AfstandsSensor);
      delay(150);

//void readSensors(){ 

//drempel waardes
if(AfstandsSensor <300){
    BijBakje = true;
}
else{
    BijBakje = false;
}

//}

}
