#include <Nextion.h>

NexText t0 = NexText(0, 3, "t0"); //Joystick Left

NexButton b0 = NexButton(0, 2, "b0"); //Pitch Button -- Same as SPitch

NexTouch *nex_listen_list[] = 
{
  &t0,
  &b0,
  NULL
};

char buffer[100] = {0};

String curMode = "";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13, OUTPUT);
  
  Serial.println("Starting init");
  nexInit();
  Serial.println("init done");
  b0.attachPush(updateMode, &b0);
}

String text;
void loop() {
  nexLoop(nex_listen_list);
  if (curMode != "") {
    Serial.println(curMode);
  }

//  text = "AAA";
//  text.toCharArray(buffer, text.length());
//  t0.setText(buffer);
//  delay(5000);
//
//  text = "BBB";
//  text.toCharArray(buffer, text.length());
//  t0.setText(buffer);
//  delay(5000);
}

void updateMode(void *fun) {
  curMode = "yay";
  Serial.println("pog");
}
