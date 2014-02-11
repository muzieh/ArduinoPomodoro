#include <LiquidCrystal.h>
#include <stdio.h>
#include "button.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
long counterTime = 60 * 60 * 1000L; 
long startTime =0;
int buttonWorkPin = 8;
int buttonBreakPin = 9;

#define STATUS_STOP 1
#define STATUS_WORK 2
#define STATUS_BREAK 3
#define STATUS_LONG_BREAK 4

int currentStatus = STATUS_STOP;

Button workButton;
Button breakButton;

void setup() {
  pinMode(buttonWorkPin, INPUT);
  pinMode(buttonBreakPin, INPUT);

  workButton.buttonId = 543;  
  workButton.buttonState = BUTTON_OFF;
  workButton.pin = buttonWorkPin;
  workButton.transientState = BUTTON_OFF;
  workButton.previousState = BUTTON_OFF;
  
  breakButton.buttonState = BUTTON_OFF;
  breakButton.pin = buttonBreakPin;
  breakButton.transientState = BUTTON_OFF;
  breakButton.previousState = BUTTON_OFF;
  
  
  Serial.begin(9600);
  lcd.begin(16, 2);
}

int currentMillis = 0;

void startCounting(int countType) {
  currentStatus = countType;
  switch(currentStatus) {
  case STATUS_WORK:
    counterTime = 25 * 60 * 1000L;
    break;
  case STATUS_BREAK:
    counterTime = 5 * 60 * 1000L;
    break;
  case STATUS_LONG_BREAK:
    counterTime = 20 * 60 * 1000L;
    break;
  }
  startTime = millis();
}

void printHeaderLine() {
  switch(currentStatus) {
  case STATUS_STOP:
    lcd.print("idle...");
    lcd.setCursor(0,1);
    lcd.print("ready to work?");
    return;
  case STATUS_WORK:
    lcd.print("Working");
    break;
  case STATUS_BREAK:
    lcd.print("Break");
    break;
  case STATUS_LONG_BREAK:
    lcd.print("Long break");
    break;
  }
}

void printTime() {    
  lcd.clear();

  long timeFromStart = millis() - startTime;
  long remaining = counterTime - timeFromStart;

  printHeaderLine();  
  if(currentStatus == STATUS_STOP)
    return;

  //lcd.print(remaining);
  lcd.setCursor(0,1);

  int remainingSeconds = abs(remaining) / 1000;
  if(remaining < 0 )
  {
    lcd.print("-");
  }

  int minutes = remainingSeconds / 60;
  int seconds = remainingSeconds - minutes * 60;

  char lineBuffer[16];
  sprintf(lineBuffer,"%02d:%02d", minutes, seconds);
  lcd.write(lineBuffer);
}

int updateButtonState(Button *button) 
{
  button->previousState = button->buttonState;
  int state = digitalRead(button->pin);
  delay(10);
  if(state == digitalRead(button->pin))
  {
    button->buttonState = state;
  }
  /*
  Serial.print("Button after ");
  Serial.print(button->buttonId);
  Serial.print(" ");
  Serial.print(button->previousState);
  Serial.print(" ");
  Serial.println(button->buttonState);
  */
  
  if ( button->previousState == BUTTON_OFF  && button->buttonState == BUTTON_ON )
  {
    button->onTime = millis();
  }
  else if ((button->previousState == BUTTON_ON) && (button->buttonState == BUTTON_OFF))
  {
    int onTime = millis() - button->onTime;
    if( onTime > 1000) {
      return PRESS_LONG;
    }
    else {
      return PRESS_SHORT;
    }
  }
  return PRESS_OFF;
}


void loop() {

  // updateTime();
  printTime();


  switch(updateButtonState(&workButton))
  {
    case PRESS_LONG:
      currentStatus = STATUS_STOP;
      Serial.println("trigger long work");
      break;
    case PRESS_SHORT:
      startCounting(STATUS_WORK);
      Serial.println("trigger short work");
      break;
    case PRESS_OFF:
      //Serial.println("trigger nothing");
      break;
  }
  
  switch(updateButtonState(&breakButton))
  {
    case PRESS_LONG:
      startCounting(STATUS_LONG_BREAK);
      Serial.println("trigger long work");
      break;
    case PRESS_SHORT:
      startCounting(STATUS_BREAK);
      Serial.println("trigger short work");
      break;
    case PRESS_OFF:
      //Serial.println("trigger nothing");
      break;
  }
  
  /*
  Serial.print("Button ---- ");
  Serial.print(workButton.buttonId);
  Serial.print(" ");
  Serial.print(workButton.previousState);
  Serial.print(" ");
  Serial.println(workButton.buttonState);
  */
  //if(workButton.buttonState == BUTTON_ON && workButton.previousState == BUTTON_OFF)
  //{
    
  //}

/*
  updateButtonState(&breakButton);
  if(breakButton.buttonState == 1 && breakButton.previousState == 0)
  {
    startCounting(STATUS_BREAK);
  }
  */
  delay(50);
  
  /*
  Serial.print(digitalRead(buttonWorkPin));
   Serial.write(" ");
   Serial.print(digitalRead(buttonBreakPin));
   Serial.write(" status");
   Serial.print(currentStatus);
   Serial.println();
   */
}


