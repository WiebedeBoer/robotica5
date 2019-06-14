#include "Nextion.h"

NexButton b0 = NexButton(0, 2, "b0");
NexText t0 = NexText(0, 3, "t0");

int count = 0;

NexTouch *listen_list[] = 
{
  &b0
};

void setup()
{
  Serial1.begin(9600);

  b0.attachPush(b0PushCallback, &b0);
}

bool bla = false;

void loop()
{   
  count++;
  if (count == 201) {
    count = 0;
  }

  Serial1.print("t0.txt=");
  Serial1.print("\"");
  Serial1.print(count);
  Serial1.print("\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  if (bla) { Serial.println("boeee"); }
  nexLoop(listen_list);
}

void b0PushCallback(void *ptr) {
  bla = true;
}

void b0PopCallback(void *ptr) {
  bla = true;
}
