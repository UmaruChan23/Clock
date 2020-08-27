#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(7,6,5,4,3,2);

#include <Arduino.h>

#include "Segment.h"
#include "Digit.h"
#include "Display.h"
boolean button_state=false;
int temp;
DS3231 clock;
RTCDateTime dt;
String format[4];
unsigned char weather[4];

void setup() {
  Serial.begin(57600);
 lcd.begin(16,2);
 lcd.setCursor(0,0);
  for (int i = 0; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  //Serial.write(Serial1.read());
  String txt="";
  dt = clock.getDateTime(); 
if(dt.hour<10 and dt.minute<10){
  txt="0"+String(dt.hour)+"."+"0"+String(dt.minute);
}
else if(dt.hour<10 and dt.minute>=10){
  txt="0"+String(dt.hour)+"."+String(dt.minute);
}
else if(dt.hour>=10 and dt.minute>=10){
  txt=String(dt.hour)+"."+String(dt.minute);
} 
else if(dt.hour>=10 and dt.minute<10){
  txt=String(dt.hour)+"."+"0"+String(dt.minute);
}
  
 //int k=0;
 lcd.setCursor(0,0);
 lcd.print(txt);
 if(Serial.available()){
  for(int i=0;i<4;i++){
    weather[i]=Serial.read();
  }
   int8_t a = weather[0];
   if(a<60 && (int)a>-60){
    format[0]=(String)a+"C "+"  ";
   }
  int test = word(weather[1], weather[2]);
  if(test<900 && test >600){
    format[1]=(String)test+"mPa"+"  ";
  }
  int8_t b=weather[3];
  if(b>=0){
    format[2]=(String)b+"m/s"+"  ";
  }
  lcd.setCursor(0,1);
  lcd.print(format[0]);
  lcd.setCursor(7,0);
  lcd.print(format[1]);
  lcd.setCursor(8,1);
  lcd.print(" ");
  lcd.setCursor(9,1);
  lcd.print(format[2]);
 }

 while(Serial.available()){
char r = Serial.read();
}
 
}
