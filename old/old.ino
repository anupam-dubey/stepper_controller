#include<mwc_stepper.h>
#include <LiquidCrystal.h>
#define CLK A0
#define DT A1
#define SW A2
#define tb66_step 8
#define tb66_dir 9
#define tb66_ena 10
#define RPM 60
#define PULSE 3200
#define ClOCKWISE 1
#define OTHERWISE 0
// modes
// 0   rotation Cw  CCw contineous
// 1   quarter mode cw -ccw on keypress A5 pin 28
// 2   manual mode cw ccw on rotatry dial

// level 0

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
int mode =0;
unsigned long lastButtonPress = 0;
MWCSTEPPER nema23(tb66_ena, tb66_dir, tb66_step);
int readrot()
{ int st=0;
    currentStateCLK = digitalRead(CLK);
  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
     if (digitalRead(DT) != currentStateCLK) {
      st=-1;
    } else {
      // Encoder is rotating CW so increment
      st=1;
     
  }
  }
  lastStateCLK = currentStateCLK;
  return st;  
}
int show_menu()
{
  int x=0;
  lcd.clear();
  switch(x)
  {
    case 0:lcd.print(F(" Rotation Mode  "));break;
    case 1:lcd.print(F(" Quarter  Mode  "));break;
    case 2:lcd.print(F(" Manual   Mode  "));break;
  }lastStateCLK = digitalRead(CLK);
  while(digitalRead(SW)==HIGH)
  {
    x=x+readrot();
    if(x>2)
    x=2;
    if(x<0)
    x=0;
    switch(x)
  {
    case 0:lcd.setCursor(0, 0);lcd.print(F(" Rotation Mode  "));break;
    case 1:lcd.setCursor(0, 0);lcd.print(F(" Quarter  Mode  "));break;
    case 2:lcd.setCursor(0, 0);lcd.print(F(" Manual   Mode  "));break;
  }
  }
  return x;
}
void rot_menu()
{
  int x=0;
  bool lastdir=1;
  bool curdir=0;
  lastStateCLK = digitalRead(CLK);
  while(digitalRead(SW)==HIGH)
  { digitalWrite(tb66_ena,LOW);
    x=readrot();
   if(x==1)
   {
    nema23.set(OTHERWISE, RPM, PULSE);
    nema23.run(); 
    curdir=OTHERWISE;
   }
   else if(x==-1)
   {
    nema23.set(ClOCKWISE, RPM, PULSE);
    nema23.run(); 
    curdir=ClOCKWISE;
   }
   else 
   {nema23.set(curdir, RPM, PULSE);
    nema23.run();
   }
   if(lastdir!=curdir)
   {
    lcd.setCursor(0, 1);
    if(curdir==0)
    lcd.print(F("   Clockwise    "));
    else
    lcd.print(F("ANTI Clockwise  "));
    lastdir=curdir;
   }
   digitalWrite(tb66_ena,LOW);
  }
 }
 void quat_menu()
{
  int x=0;
  bool lastdir=1;
  bool curdir=0;
  lastStateCLK = digitalRead(CLK);
  while(digitalRead(SW)==HIGH)
  { digitalWrite(tb66_ena,LOW);
    x=readrot();
   if(x==1)
   {
    nema23.set(OTHERWISE, RPM, PULSE);
    for (size_t i = 0; i < 800; i++)
   {
    nema23.run(); 
   }
    curdir=OTHERWISE;
   }
   else if(x==-1)
   {
    nema23.set(ClOCKWISE, RPM, PULSE);
    for (size_t i = 0; i < 800; i++)
   {
    nema23.run(); 
   } 
    curdir=ClOCKWISE;
   }
   else 
   {//nema23.run();
   }
   if(lastdir!=curdir)
   {
    lcd.setCursor(0, 1);
    if(curdir==0)
    lcd.print(F("   Clockwise    "));
    else
    lcd.print(F("ANTI Clockwise  "));
    lastdir=curdir;
   }
   digitalWrite(tb66_ena,LOW);
  }
 }
  void manual_menu()
{
  int x=0;
  bool lastdir=1;
  bool curdir=0;
  lastStateCLK = digitalRead(CLK);
  while(digitalRead(SW)==HIGH)
  { digitalWrite(tb66_ena,LOW);
    x=readrot();
   if(x==1)
   {
    nema23.set(OTHERWISE, RPM, PULSE);
    for (size_t i = 0; i < 50; i++)
   {
    nema23.run(); 
   }
    curdir=OTHERWISE;
   }
   else if(x==-1)
   {
    nema23.set(ClOCKWISE, RPM, PULSE);
    for (size_t i = 0; i < 50; i++)
   {
    nema23.run(); 
   }
    curdir=ClOCKWISE;
   }
   else 
   {//nema23.run();
   }
   if(lastdir!=curdir)
   {
    lcd.setCursor(0, 1);
    if(curdir==0)
    lcd.print(F("   Clockwise    "));
    else
    lcd.print(F("ANTI Clockwise  "));
    lastdir=curdir;
   }
   digitalWrite(tb66_ena,LOW);
  }
 }
void setup() {
 pinMode(CLK,INPUT);
 pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP); 
  pinMode(tb66_step,OUTPUT);
pinMode(tb66_dir,OUTPUT);
pinMode(tb66_ena,OUTPUT);
digitalWrite(tb66_step,LOW);
digitalWrite(tb66_dir,LOW);
digitalWrite(tb66_ena,HIGH);
 lcd.begin(16, 2);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print(F("Stepper Control"));
 lcd.setCursor(0, 1);
 lcd.print(F("EX2019-23 GGITS"));
 delay(2500);
 /*digitalWrite(tb66_ena,LOW);
  nema23.set(OTHERWISE, RPM, PULSE);
  for (size_t i = 0; i < 3200; i++)
  {
    nema23.run();
  }*/
 
  digitalWrite(tb66_ena,HIGH);
  mode=show_menu();
  delay(500);
  lcd.clear();
   lcd.print(mode);
}

void loop() {
  switch(mode)
  {
   case 0:lcd.clear();lcd.print(F("*Rotation Mode *"));rot_menu();break;
   case 1:lcd.clear();lcd.print(F("*Quarter  Mode *"));quat_menu();break;
   case 2:lcd.clear();lcd.print(F("*MANUAL   Mode *"));manual_menu();break;
  }
}
