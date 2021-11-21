
#include "SUltraSonic.h"

// TRIG=D7, ECHO=D8
SUltraSonic ss(7,8); 
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  ss.setup();
}

void loop()
{
  ss.check();
  int distance = ss.distanceByCm();
  delay(200);
}
