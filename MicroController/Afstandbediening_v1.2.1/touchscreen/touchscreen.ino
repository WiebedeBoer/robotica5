#include "Nextion.h"

NexButton b0 = NexButton(0, 2, "b0");
NexText t0 = NexText(0, 3, "t0");

int count = 0;

NexTouch *listen_list[] = 
{
  &b0,
  NULL
};

void setup()
{
  nexInit();
  Serial1.begin(9600);

  b0.attachPush(b0PushCallback, &b0);
  b0.attachPop(b0PopCallback, &b0);
}

bool answer = false;

void loop()
{

  if(digitalRead(0) != 1) {
    Serial.println(digitalRead(0));
    Serial.println("Got something");
  }

  if (answer) { 
    Serial.println("boeee"); 
  }
    
  nexLoop(listen_list);
}

void b0PushCallback(void *ptr) {
  Serial.println("button is called");
  answer = true;
}

void b0PopCallback(void *ptr) {
  Serial.println("button is popped");
  answer = true;
}
