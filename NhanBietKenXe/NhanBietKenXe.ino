#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include <stdio.h>

/////////////// GLOBAL VARIABLE ///////////////
#define MAX_SOUND_TRACK 100
#define SOUND_PIN_TOTAL 2
#define DELAY_TIME 10 // milisecond
#define UNSET_VALUE -1

#define TRACKING_STATUS_NOT_TRACKING 0
#define TRACKING_STATUS_NEED_START_TRACKING 1
#define TRACKING_STATUS_TRACKING 2
#define HONK_MIN 100

int soundPins[SOUND_PIN_TOTAL] = {A0, A1};
int soundValues[SOUND_PIN_TOTAL][MAX_SOUND_TRACK];
SMSGSM sms;
boolean started = false; //trạng thái modul sim

void reset_sound_value() {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      soundValues[p][t] = UNSET_VALUE;
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


int null_soundtrack_index() {
  for (int t = 0; t < MAX_SOUND_TRACK; t++) {
    if (soundValues[0][t] < 0) return t;
  }

  return -1;
}

void set_newSoundTrack(int trackIndex, int *newValues) {
  // Serial.println("");
  // Serial.print("trackIndex: ");
  // Serial.print(trackIndex);
  // Serial.println("");
  // Serial.print("newSoundTrack: ");
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int value = *(newValues + p);
    soundValues[p][trackIndex] = value;

    // Serial.print(value);
    // Serial.print(" ");
  }
}

int tracking_status(int *newValues) {
  bool isNotTracking = soundValues[0][0] == UNSET_VALUE;
  if (!isNotTracking) return TRACKING_STATUS_TRACKING;

  int hasHonkValue = false;
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int value = *(newValues + p);
    // print_sound_bar(p, value);
    if (value >= HONK_MIN) {
      hasHonkValue = true;
      break;
    }
  }

  if (hasHonkValue) return TRACKING_STATUS_NEED_START_TRACKING;
  else return TRACKING_STATUS_NOT_TRACKING;
}

bool need_start_analyse() {
  return soundValues[0][MAX_SOUND_TRACK - 1] >= 0;
}

int read_sound(int si) {
  int pin = soundPins[si];
  return analogRead(pin);
}

void print_sound_bar(int si, int value) {
  int bar = int(value / 10);
  Serial.println("");
  Serial.print("A");
  Serial.print(si);
  Serial.print(":");
  for (int bi = 0; bi < bar; bi++) {
    Serial.print("x");
  }
  Serial.print(value);
}

void print_all() {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    Serial.println("");
    Serial.print("A");
    Serial.print(p);
    Serial.print(":");
    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      Serial.print(soundValues[p][t]);
      Serial.print(",");
    }
  }
}

void do_analyse() {
  Serial.println("");
  Serial.print("DO ANALYSE ============  ");
  print_all();
  int max = -1;
  int soundMax = -1;
  for (int p = 0; p < 2; p++) {
    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      int value = soundValues[p][t];
      if (value > max) {
        max = value;
        soundMax = p;
      }
    }
  }
  Serial.println("");
  Serial.print("SOUND AT ");
  Serial.print(soundMax);
  Serial.println("");
  sendSMS(soundMax);
}

void sendSMS(int soundIndex) {
  if (started) {
    char sdt[] = "+84377670064";
    char msg[20];
    sprintf(msg, "From %d", soundIndex);
    //sms.SendSMS(sdt, msg);
  }
}

///////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  reset_sound_value();

  if (gsm.begin(2400)) {
    Serial.println("\nstatus=READY");
    started = true;
  } else Serial.println("\nstatus=IDLE");
}

void loop() {
  if (need_start_analyse()) {
    do_analyse();
    reset_sound_value();
  }

  int *sounds = read_sound_array();
  int status = tracking_status(sounds);

  // Serial.println("");
  // Serial.print("STATUS  ");
  // Serial.print(status);
  // Serial.println("");
  if (status == TRACKING_STATUS_TRACKING || status == TRACKING_STATUS_NEED_START_TRACKING) {
    int nullIndex = null_soundtrack_index();
    // Serial.println("nullIndex = ");
    // Serial.print(nullIndex);
    set_newSoundTrack(nullIndex, sounds);
  }


  delay(DELAY_TIME);
}
