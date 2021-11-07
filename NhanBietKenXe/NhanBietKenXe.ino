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

// Ngưỡng âm thanh bắt đầu ghi nhận, vượt trên mức này mới đánh giá là tiếng còi xe.
#define SOUND_THRESHOLD 250

// Array chứa các analogPin ứng với các sensor mạch đang quản lý.
int soundPins[SOUND_PIN_TOTAL] = {A0, A1};

// Bảng ghi âm thanh, mỗi khi ghi nhận có âm vượt SOUND_THRESHOLD 
// thì bắt đầu ghi 100 giá trị ở mỗi sensor, để tiến hành phân tích.
int soundTable[SOUND_PIN_TOTAL][MAX_SOUND_TRACK];

SMSGSM sms;

// Trạng thái module sim đã khởi động thành công.
boolean sim900a_started = false;

// Array char dùng cho sprintf chuỗi.
char tempt[50];

void reset_sound_table() {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      soundTable[p][t] = UNSET_VALUE;
    }
  }
}

// return cường độ âm của mỗi sensor.
int *read_sound_pins() {
  int *values = new int[SOUND_PIN_TOTAL];
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int pin = soundPins[p];
    int value = analogRead(pin);
    values[p] = value;
  }
  return values;
}

// return index của soundTable mà tại đó chưa được gán giá trị.
int get_unset_index() {
  for (int t = 0; t < MAX_SOUND_TRACK; t++) {
    if (soundTable[0][t] < 0) return t;
  }

  return -1;
}

// ghi giá trị mới tại trackIndex
void set_newSoundTrack(int trackIndex, int *newValues) {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int value = *(newValues + p);
    soundTable[p][trackIndex] = value;
  }
}

// Cho biết ứng với giá trị sound mới thì nên ghi tiếp vào bảng ghi hay nên bỏ qua.
int tracking_status(int *newValues) {
  bool isNotTracking = soundTable[0][0] == UNSET_VALUE;
  if (!isNotTracking) return TRACKING_STATUS_TRACKING; // Tiếp tục ghi vào bảng  ghi.

  int hasHonkValue = false;
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    int value = *(newValues + p);
    if (value >= SOUND_THRESHOLD) {
      hasHonkValue = true;
      break;
    }
  }
  // Nếu có âm thanh đạt cường độ cần quan tâm.
  if (hasHonkValue) return TRACKING_STATUS_NEED_START_TRACKING; // Bắt đầu ghi vào bảng ghi.
  else return TRACKING_STATUS_NOT_TRACKING; // Không cần ghi vào bảng ghi.
}

bool need_start_analyse() {
  int last_sound_index = MAX_SOUND_TRACK - 1;
  return soundTable[0][last_sound_index] >= 0;
}

// Đọc giá trị âm thanh ở sensor.
int read_sound(int si) {
  int pin = soundPins[si];
  return analogRead(pin);
}

void print_all() {
  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    sprintf(tempt, "A%d:", p);
    Serial.print(tempt);

    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      Serial.print(soundTable[p][t]);
      Serial.print(",");
    }
    if(p < SOUND_PIN_TOTAL - 1) {
      Serial.print("\n");
    }
  }
}

// Tiến hành phân tích bảng ghi.
// Hiện tại đang đánh giá sensor nào có giá trị âm lớn nhất (trong 100 giá trị)
// Kêt luận: nguồn âm thanh ở gần sensor đó hơn.
int get_sound_sources() {
  Serial.println("\n ============ DO ANALYSE ============  ");
  print_all();
  int max = -1;
  int sound_source = -1;

  for (int p = 0; p < SOUND_PIN_TOTAL; p++) {
    for (int t = 0; t < MAX_SOUND_TRACK; t++) {
      int value = soundTable[p][t];
      if (value > max) {
        max = value;
        sound_source = p;
      }
    }
  }
  sprintf(tempt, "\nMax at: %d", sound_source);
  Serial.println(tempt);
  // Gửi tin nhắn
  return sound_source;
}

// Call at setup()
void setup_module_sim900a() { 
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

//------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  // reset lại bảng ghi sound.
  reset_sound_table();

  // Thực hiện các init cần thiết của module sim.
  setup_module_sim900a();
}

void loop() {
  // Khi bảng ghi sound đạt đủ 100 giá trị trên mỗi sensor thì tiến hành phân tích bảng ghi sound.
  if (need_start_analyse()) {
    // Tiến hành phân tích nơi phát nguồn âm thanh.
    int sound_source = get_sound_sources();

    // Gửi sms thông báo nơi phát nguồn âm thanh.
    sendSMS(sound_source);

    // reset lại bảng ghi sound.
    reset_sound_table();
  }

  // Đọc giá trị âm tại cái sensor.
  int *sounds = read_sound_pins();

  // Đánh giá các giá trị âm này có cần ghi nhận hay không.
  int status = tracking_status(sounds);

  if (status == TRACKING_STATUS_TRACKING || status == TRACKING_STATUS_NEED_START_TRACKING) {
    // Xác định row trên bảng ghi chưa được ghi giá trị.
    int unset_index = get_unset_index();

    // Ghi giá trị âm vào bảng ghi.
    set_newSoundTrack(unset_index, sounds);
  }

  delay(DELAY_TIME);
}
