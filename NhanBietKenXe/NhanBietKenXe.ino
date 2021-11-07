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
#define HONK_MIN 250

int soundPins[SOUND_PIN_TOTAL] = {A0, A1};
int soundValues[SOUND_PIN_TOTAL][MAX_SOUND_TRACK];
SMSGSM sms;
boolean sim900a_started = false; //trạng thái module sim
char tempt[50];

void reset_sound_value() {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      soundValues[p][t] = UNSET_VALUE;
    }
  }
}

int *read_sound_pins() {
  int *values = new int[SOUND_PIN_TOTAL];
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int pin = soundPins[p];
    int value = analogRead(pin);
    values[p] = value;
  }
  return values;
}

int get_unset_index() {
  for (int t = 0; t < MAX_SOUND_TRACK; t++) {
    if (soundValues[0][t] < 0) return t;
  }

  return -1;
}

void set_newSoundTrack(int trackIndex, int *newValues) {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int value = *(newValues + p);
    soundValues[p][trackIndex] = value;
  }
}

int tracking_status(int *newValues) {
  bool isNotTracking = soundValues[0][0] == UNSET_VALUE;
  if (!isNotTracking) return TRACKING_STATUS_TRACKING;

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
  int last_sound_index = MAX_SOUND_TRACK - 1;
  return soundValues[0][last_sound_index] >= 0;
}

int read_sound(int si) {
  int pin = soundPins[si];
  return analogRead(pin);
}

void print_all() {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    sprintf(tempt, "A%d:", p);
    Serial.print(tempt);

    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      Serial.print(soundValues[p][t]);
      Serial.print(",");
    }
    if(p < SOUND_PIN_TOTAL - 1) {
      Serial.print("\n");
    }
  }
}

void do_analyse() {
  Serial.println("\n ============ DO ANALYSE ============  ");
  print_all();
  int max = -1;
  int soundMax = -1;

  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      int value = soundValues[p][t];
      if (value > max) {
        max = value;
        soundMax = p;
      }
    }
  }
  sprintf(tempt, "\nMax at: %d", soundMax);
  Serial.println(tempt);

  sendSMS(soundMax);
}

void setup_module_sim900a() { // Call at setup()
  if (gsm.begin(2400)) {
    Serial.println("\nsim900a: READY");
    sim900a_started = true;
  } else {
    Serial.println("\nsim900a: IDLE");
  }
}

void sendSMS(int soundIndex) {
  if (!sim900a_started) return;

  char phone_number[] = "+84377670064";
  sprintf(tempt, "Sound from sensor at %d", soundIndex);
  // sms.SendSMS(phone_number, tempt);
}

///////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  reset_sound_value();
  setup_module_sim900a();
}

void loop() {
  if (need_start_analyse()) {
    do_analyse();
    reset_sound_value();
  }

  int *sounds = read_sound_pins();
  int status = tracking_status(sounds);

  if (status == TRACKING_STATUS_TRACKING || status == TRACKING_STATUS_NEED_START_TRACKING) {
    int nullIndex = get_unset_index();
    set_newSoundTrack(nullIndex, sounds);
  }

  delay(DELAY_TIME);
}
