### Library

* sim900.zip

http://arduino.vn/bai-viet/854-sim900a-arduino-gui-va-nhan-sms

### Nối chân

<img src="https://tva1.sinaimg.cn/large/008i3skNgy1gvy5jm73udj30o00o4acz.jpg" alt="image-20211031064334798" style="zoom:50%;" />

### Code

```c
#include "SIM900.h"
#include <SoftwareSerial.h> 
#include "sms.h"

SMSGSM sms;
boolean started=false; //trạng thái modul sim 

void setup() {
  Serial.begin(9600);
  Serial.println("Gui va nhan tin nhan");
  // put your setup code here, to run once:
  if (gsm.begin(2400)) {
    Serial.println("\nstatus=READY");
    started = true;
  } else Serial.println("\nstatus=IDLE");

  if (started) {
    char sdt[] = "+84903540608";
    char msg[] = "noi dung tin nhan";
    sms.SendSMS(sdt, msg);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
```

