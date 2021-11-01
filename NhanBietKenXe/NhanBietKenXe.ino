
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"

/////////////// GLOBAL VARIABLE ///////////////
#define MAX_SOUND_TRACK 100
#define SOUND_PIN_TOTAL 2
#define DELAY_TIME 10 // milisecond
#define UNSET_VALUE -1

#define TRACKING_STATUS_NOT_TRACKING 0
#define TRACKING_STATUS_NEED_START_TRACKING 1
#define TRACKING_STATUS_TRACKING 2
#define HONK_MIN 200

int soundPins[SOUND_PIN_TOTAL] = {A0, A1};
int soundValues[SOUND_PIN_TOTAL][MAX_SOUND_TRACK];
bool allowTrack = false;

void reset_sound_value() {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    for (int s = 0; s < MAX_SOUND_TRACK; s++) {
      soundValues[p][s] = UNSET_VALUE;
    }
  }
}

int *read_sound_array() {
  int *values = new int[SOUND_PIN_TOTAL];
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int pin = soundPins[p];
    int value = analogRead(pin);
    values[p] = value;
  }
  return values;
}


int tracking_status(int *newValues) {
  bool isNotTracking = soundValues[0][0] == UNSET_VALUE;
  if (isNotTracking) return TRACKING_STATUS_TRACKING;

  int hasHonkValue = false;
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int value = *(newValues + p);
    if (value >= HONK_MIN) {
      hasHonkValue = true;
      break;
    }
  }

  if (hasHonkValue) return TRACKING_STATUS_NEED_START_TRACKING;
  else return TRACKING_STATUS_NOT_TRACKING;
}

bool need_start_analyse() {
  return soundValues[0][SOUND_PIN_TOTAL - 1] != UNSET_VALUE;
}

int read_sound(int si) {
  int pin = soundPins[si];
  return analogRead(pin);
}

void print_sound_bar(int si, int value) {
  int bar = int(value / 10);
  Serial.print("A");
  Serial.print(si);
  Serial.print(":");
  for (int bi = 0; bi < value; bi++) {
    Serial.print("=");
  }
  Serial.println("");
}

void do_analyse() {
  
}

///////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  if (need_start_analyse()) {
    do_analyse();
    reset_sound_value();
  }
  
  int *sounds = read_sound_array();
  int status = tracking_status(sounds);

  switch(status) {
  case TRACKING_STATUS_NOT_TRACKING: break;
  case TRACKING_STATUS_NEED_START_TRACKING: break;
  case TRACKING_STATUS_TRACKING: break;
  default: break;
  }
  
  delay(DELAY_TIME);
}
