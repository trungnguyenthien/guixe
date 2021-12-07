
#include "Arduino.h"
#include "SIM900.h"
#include "sms.h"
#include "DFRobotDFPlayerMini.h"
#include<SoftwareSerial.h>
SoftwareSerial mySoftwareSerial(11, 10); 
DFRobotDFPlayerMini myDFPlayer;
#define DELAY_TIME 1000
char sdt[]="+84377670064";

char SMS1[50]="Canh bao Palette 1 co tieng coi";
char SMS2[50]="Canh bao Palette 1 co khoi";
char SMS3[50]="Canh bao Palette 1 co chuyen dong";

char SMS4[50]="Canh bao Palette 2 co tieng coi";
char SMS5[50]="Canh bao Palette 2 co khoi";
char SMS6[50]="Canh bao Palette 2 co chuyen dong";

char SMS7[50]="Canh bao Palette 3 co tieng coi";
char SMS8[50]="Canh bao Palette 3 co khoi";
char SMS9[50]="Canh bao Palette 3 co chuyen dong";

char SMS10[50]="Canh bao Palette 4 co tieng coi";
char SMS11[50]="Canh bao Palette 4 co khoi";
char SMS12[50]="Canh bao Palette 4 co chuyen dong";

SMSGSM sms;
boolean started=false; //trạng thái modul sim 

void setup() {
  Serial.begin(9600);
  Serial.println("START SETUP");
  //cd muc 1 ,am vs gas muc 0
  //palete1
//  pinMode(2, INPUT); //cd
  pinMode(4, INPUT); //gas
  pinMode(5, INPUT); //am
  //palete2
//  pinMode(6, INPUT);//cd
  pinMode(14, INPUT);//am
  pinMode(15, INPUT);//gas
  //palete3
  pinMode(16, INPUT);//am
  pinMode(17, INPUT);//gas
//  pinMode(18, INPUT);//cd
  //palete4
  pinMode(19, INPUT);//am
//  pinMode(9, INPUT);//cd
  pinMode(13, INPUT);//gas

  pinMode(3,OUTPUT);
  pinMode(0,OUTPUT);
  digitalWrite(3,LOW);
  digitalWrite(0,HIGH);
  delay(7000);

    // put your setup code here, to run once:
  if (gsm.begin(2400)) {
    Serial.println("\nstatus=READY");
    started = true;
  } else Serial.println("\nstatus=IDLE");
  
  mySoftwareSerial.begin(9600);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while(true){
      Serial.println("Wait for player ...");
      delay(100); // Code to compatible with ESP8266 watch dog.
    }
  }
  
  myDFPlayer.volume(30);
  Serial.println("END SETUP");
//  SendMessage(SMS1);
}

void mp3(int x){
  mySoftwareSerial.listen();
  myDFPlayer.play(x);
}
void warn(){
//  digitalWrite(3,HIGH);
//  delay(200);
//  digitalWrite(3,LOW);
//  delay(200);
//  digitalWrite(3,HIGH);
//  delay(200);
//  digitalWrite(3,LOW);
//  delay(200);
//  digitalWrite(3,HIGH);
//  delay(1000);
//  digitalWrite(3,LOW);
//  delay(1000);
}
void pump(){
  digitalWrite(0,LOW);
  delay(5000);
  digitalWrite(0,HIGH);
}
void loop() {
  //palette 1
  Serial.println(digitalRead(5));
  if(!digitalRead(5)){
    Serial.println(SMS1);
    
    warn();
    mp3(1);
    SendMessage(SMS1);
    delay(DELAY_TIME);
    return;
  }
  if(!digitalRead(4)){
    Serial.println(SMS2);
    
    warn();
    mp3(5);
    pump();
    SendMessage(SMS2);
    delay(DELAY_TIME);
    return;
  }
//  if(digitalRead(2)){ //CD
//    warn();
//    mp3(9);
//    delay(5000);
//    mp3(13);
//    SendMessage(SMS3);
//  }
  //palette 2
  if(!digitalRead(14)){
    Serial.println(SMS4);
    
    warn();
    mp3(2);
    SendMessage(SMS4);
    delay(DELAY_TIME);
    return;
  }
  if(!digitalRead(15)){
    Serial.println(SMS5);
    
    warn();
    mp3(6);
    pump();
    SendMessage(SMS5);
    delay(DELAY_TIME);
    return;
  }
//  if(digitalRead(6)){
//    warn();
//    mp3(10);
//    delay(5000);
//    mp3(13);
//    SendMessage(SMS6);
//  }
  //palette 3
  if(!digitalRead(16)){
    Serial.println(SMS7);
    
    warn();
    mp3(3);
    SendMessage(SMS7);
    delay(DELAY_TIME);
    return;
  }
  if(!digitalRead(17)){
    Serial.println(SMS8);
    
    warn();
    mp3(7);
    pump();
    SendMessage(SMS8);
    delay(DELAY_TIME);
    return;
  }
//  if(digitalRead(18)){
//    warn();
//    mp3(11);
//    delay(5000);
//    mp3(13);
//    SendMessage(SMS9);
//  }
  //palette 4
  if(!digitalRead(19)){
    
    Serial.println(SMS10);
    warn();
    mp3(4);
    SendMessage(SMS10);
    delay(DELAY_TIME);
    return;
  }
//  if(!digitalRead(13)){
//    Serial.println(SMS11);
//    warn();
//    mp3(8);
//    pump();
//    SendMessage(SMS11);
//  }
//  if(digitalRead(9)){
//    warn();
//    mp3(12);
//    delay(5000);
//    mp3(13);
//    SendMessage(SMS12);
//  } 

  delay(2);
}
void SendMessage(char SMS[50])
{
  if (started) {
    Serial.println("SEND SMS");
        char sdt[] = "+84903540608";
    char msg[] = "noi dung tin nhan";
    sms.SendSMS(sdt, msg);
  }
}
