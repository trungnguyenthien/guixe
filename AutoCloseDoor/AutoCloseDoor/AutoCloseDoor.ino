                                                       
#include "SUltraSonic.h"
#include <Stepper.h>
#include "Stepper28BYJ48.h"

// TRIG=D7, ECHO=D8
SUltraSonic ss(2, 3);
Stepper28BYJ48 st(8, 9, 10, 11);

void setup()
{
  // put your setup code here, to run once:
  ss.setup();
  Serial.begin(9600);
}

void loop()
{
  ss.check();
  int distance = ss.distanceByCm();
  if (distance < 4) {
    st.run(0.5, 2);
  }
  delay(200);
}
