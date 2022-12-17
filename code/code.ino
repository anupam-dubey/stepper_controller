#include<mwc_stepper.h>
#include <LiquidCrystal.h>
#define CLK A0
#define DT A1
#define SW A2
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
void setup() {
 pinMode(CLK,INPUT);
 pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP); 
 lcd.begin(16, 2);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Hello Hackster");
 // Read the initial state of CLK
 lastStateCLK = digitalRead(CLK);
}

void loop() {
 // Read the current state of CLK
 currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir ="CW";
    }

    //Serial.print("Direction: ");
    lcd.setCursor(0,1);
    lcd.print(currentDir);lcd.print("  ");
  //  Serial.print(" | Counter: ");
    lcd.setCursor(6,1);
    lcd.print(counter);lcd.print("  ");
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      lcd.setCursor(15,1);
      lcd.print("B");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
 
 
  

}
