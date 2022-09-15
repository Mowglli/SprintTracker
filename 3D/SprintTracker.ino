 /*
 * HC-SR04 VCC - > TTGO 5V
 * HC-SR04 GND - > TTGO GND
 * HC-SR04 TRIG - > TTGO 32
 * HC-SR04 ECHO - > TTGO 33
 * 
 * Buzzer + -> TTGO 17
 * Buzzer - -> TTGO GND
 * 
 * Buttons: 37, 38, 36
 */
#include "Free_Fonts.h" // Include the header file attached to this sketch
#include "SPI.h"
#include "TFT_eSPI.h"

//Based on TTGO
#define button1 26 
#define button2 27 
#define start_run 25
#define TRIG 32
#define ECHO 33
#define A0 17                                               
 

unsigned long runTimeStart;
unsigned long runTimeMs;
int runNumber = 0;
char buf[4];
unsigned int distance = 100;
bool timeMeasuring = false;
bool timesDisplayed = false;

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

unsigned long drawTime = 0;

void setup(void) {
  //Display
  tft.begin();
  tft.init();
  tft.setRotation(1);
  
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(start_run, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  tft.fillScreen(TFT_BLACK);
  DisplayStartMenu(distance);
}

void loop() {
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  if(digitalRead(start_run) == HIGH){
  if(timesDisplayed == false){
  PrepareForRun();
  }else{
    timesDisplayed = false;
    DisplayStartMenu(distance);
    delay(1000);
  }
}

  if(digitalRead(button2) == HIGH){
  if(distance > 50){
  distance-=50;
  UpdateDistance(distance);
  delay(200);
  }
}
if(digitalRead(button1) == HIGH){
  if(distance < 500){
  distance+=50;
  UpdateDistance(distance);
  delay(200);
  }
}   

}

void UpdateDistance(int _distance){
  tft.fillScreen(TFT_BLACK);
  String text = itoa(_distance, buf, 10);
  text += "m";
  tft.setFreeFont(FF2);
  tft.drawString("Distance: "+ text, 120, 40, GFXFF);  
}

void DisplayStartMenu(int _distance){
  String text = itoa(_distance, buf, 10);
  text += "m";
  tft.setFreeFont(FF2);
  tft.drawString("Distance: "+ text, 30, 33, GFXFF);
  }

void PrepareForRun(){
  int countdown = distance/5;
  digitalWrite(A0, HIGH);
    delay(200);
    digitalWrite(A0, LOW);
    delay(200);
    digitalWrite(A0, HIGH);
    delay(200);
    digitalWrite(A0, LOW);
    delay(200);
  for(int a = 0; a < distance/5; a++){
    --countdown; 
    String text = itoa(countdown, buf, 10);
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(FF2);
    tft.drawString("Go to start: "+ text, 120, 40, GFXFF);
   delay(1000);
  }
  for(int b = 3; b > 0; b--){
    String text = itoa(b, buf, 10);
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(FF2);
    tft.drawString("Time: "+ text, 120, 40, GFXFF);
    digitalWrite(A0, HIGH);
    delay(500);
    digitalWrite(A0, LOW);
    delay(500);
  }
  digitalWrite(A0, HIGH);
  delay(2000);
  tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(FF2);
    tft.drawString("RUN!", 120, 40, GFXFF);
  digitalWrite(A0, LOW);
  MeasureRunTime();
  }

void MeasureRunTime(){
  timeMeasuring = true;
  runTimeStart = millis();
  while(MeasureDistance() > 100){
    delay(10);
  }
  runTimeMs = millis() - runTimeStart;
  double timer = (double)runTimeMs/1000;
  Serial.print("Time: ");
  Serial.println((double)runTimeMs/1000);
  timeMeasuring = false;
  DisplayTimes(timer);
}

float MeasureDistance(){
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);

  float t = pulseIn(ECHO, HIGH);
  float distance = t*0.017015;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}

void DisplayTimes(int time_number){
  String text = itoa(time_number, buf, 10);
  tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(FF2);
    tft.drawString("Time: "+ text +" s", 120, 40, GFXFF);
  }

#ifndef LOAD_GLCD//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif
