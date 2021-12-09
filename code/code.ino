#include "Arduino.h"
#include "SIM900.h"
#include "sms.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
SoftwareSerial mySoftwareSerial(11, 10);
DFRobotDFPlayerMini myDFPlayer;
#define DELAY_TIME 1000
#define ENABLE_SOUND false
#define ENABLE_GAS true
#define ENABLE_IR false
#define ENABLE_SMS false

char sdt[20] = "+84377670064";
char SMS1[50] = "Canh bao Palette 1 co tieng coi";
char SMS2[50] = "Canh bao Palette 1 co khoi";
char SMS3[50] = "Canh bao Palette 1 co chuyen dong";

char SMS4[50] = "Canh bao Palette 2 co tieng coi";
char SMS5[50] = "Canh bao Palette 2 co khoi";
char SMS6[50] = "Canh bao Palette 2 co chuyen dong";

char SMS7[50] = "Canh bao Palette 3 co tieng coi";
char SMS8[50] = "Canh bao Palette 3 co khoi";
char SMS9[50] = "Canh bao Palette 3 co chuyen dong";

char SMS10[50] = "Canh bao Palette 4 co tieng coi";
char SMS11[50] = "Canh bao Palette 4 co khoi";
char SMS12[50] = "Canh bao Palette 4 co chuyen dong";

SMSGSM sms;
boolean started = false; // trạng thái modul sim

void pause_after_speak()
{
  delay(1000);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("START SETUP");
  // cd muc 1 ,am vs gas muc 0
  // palete1
  pinMode(2, INPUT); // cd
  pinMode(4, INPUT); // gas
  pinMode(5, INPUT); // am
  // palete2
  pinMode(6, INPUT);  // cd
  pinMode(14, INPUT); // am
  pinMode(15, INPUT); // gas
  // palete3
  pinMode(16, INPUT); // am
  pinMode(17, INPUT); // gas
  pinMode(18, INPUT); // cd
  // palete4
  pinMode(19, INPUT); // am
  pinMode(9, INPUT);  // cd
  pinMode(13, INPUT); // gas

  pinMode(3, OUTPUT);
  pinMode(8, OUTPUT);
  
  digitalWrite(3, LOW);
  digitalWrite(8, HIGH);
  
  delay(7000);

  initSMS();

  mySoftwareSerial.begin(9600);

  if (!myDFPlayer.begin(mySoftwareSerial))
  { // Use softwareSerial to communicate with mp3.
    while (true)
    {
      Serial.println("Wait for player ...");
      delay(100); // Code to compatible with ESP8266 watch dog.
    }
  }

  myDFPlayer.volume(30);
  Serial.println("END SETUP");
}

void mp3(int x)
{
  // mySoftwareSerial.listen();
  myDFPlayer.play(x);
  delay(4000);
}

void pump()
{
  digitalWrite(8, LOW);
  delay(5000);
  digitalWrite(8, HIGH);
}

void loop()
{
  /////////////////////////////////////// palette 1 /////////////////////////////////////
  Serial.println(digitalRead(5));
  if (!digitalRead(5) && ENABLE_SOUND)
  {
    Serial.println(SMS1);
    mp3(1);
    SendMessage(SMS1);
    pause_after_speak();
    return;
  }
  else if (digitalRead(4) == LOW && ENABLE_GAS)
  {
    Serial.println(SMS2);
    mp3(5);
    pump();
    SendMessage(SMS2);
    pause_after_speak();
    return;
  }
  else if (digitalRead(2) && ENABLE_IR)
  {
    Serial.println(SMS3);
    mp3(9);
    SendMessage(SMS3);
    pause_after_speak();
    return;
  }
  /////////////////////////////////////// palette 2 /////////////////////////////////////
  if (!digitalRead(14) && ENABLE_SOUND)
  {
    Serial.println(SMS4);
    mp3(2);
    SendMessage(SMS4);
    pause_after_speak();
    return;
  }
  else if (digitalRead(15) == LOW && ENABLE_GAS)
  {
    Serial.println(SMS5);
    mp3(6);
    pump();
    SendMessage(SMS5);
    pause_after_speak();
    return;
  }
  else if (digitalRead(6) && ENABLE_IR)
  {
    Serial.println(SMS6);
    mp3(10);
    SendMessage(SMS6);
    pause_after_speak();
    return;
  }
  /////////////////////////////////////// palette 3 /////////////////////////////////////
  if (!digitalRead(16) && ENABLE_SOUND)
  {
    Serial.println(SMS7);
    mp3(3);
    SendMessage(SMS7);
    pause_after_speak();
    return;
  }
  else if (digitalRead(17) == LOW && ENABLE_GAS)
  {
    Serial.println(SMS8);
    mp3(7);
    pump();
    SendMessage(SMS8);
    pause_after_speak();
    return;
  }
  else if (digitalRead(18) && ENABLE_IR)
  {
    Serial.println(SMS9);
    mp3(11);
    SendMessage(SMS9);
    pause_after_speak();
    return;
  }
  /////////////////////////////////////// palette 4 /////////////////////////////////////
  if (!digitalRead(19) && ENABLE_SOUND)
  {
    Serial.println(SMS10);
    mp3(4);
    SendMessage(SMS10);
    pause_after_speak();
    return;
  }
  else if (digitalRead(13) == HIGH && ENABLE_GAS)
  {
    Serial.println(SMS11);
    mp3(8);
    pump();
    SendMessage(SMS11);
    pause_after_speak();
    return;
  }
  else if (digitalRead(9) && ENABLE_IR)
  {
    Serial.println(SMS12);
    mp3(12);
    SendMessage(SMS12);
    pause_after_speak();
    return;
  }

  delay(2);
}
void SendMessage(char *SMS)
{
  if (!ENABLE_SMS)
  {
    return;
  }
  if (started)
  {
    Serial.println("SEND SMS ");
    sms.SendSMS(sdt, SMS);
    Serial.println("Send Completed");
    delay(2000);
  }
}


void initSMS()
{
  if (!ENABLE_SMS)
  {
    return;
  }
  // put your setup code here, to run once:
  if (gsm.begin(2400))
  {
    Serial.println("\nstatus=READY");
    started = true;
  }
  else
    Serial.println("\nstatus=IDLE");
}
