#include <LiquidCrystal.h>
#include <stdio.h>

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

int previousWorkButtonState = 0;
int previousBreakButtonState = 0;

void setup() {
  pinMode(buttonWorkPin, INPUT);
  pinMode(buttonBreakPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
//  lcd.print("idle...");
//  lcd.setCursor(0,1);
//  lcd.print("ready to work?");
}

int currentMillis = 0;
/*
void updateTime() {
  switch(currentStatus) {
    case STATUS_WORK:
    case STATUS_BREAK:
    case STATUS_LONG_BREAK:
      break;
  }
  currentMillis = millis();

  //previousMillis = currentMillis();
}
*/
void startCounting(int countType) {
 switch(countType) {
    case STATUS_WORK:
      counterTime = 25 * 60 * 1000L;
      break;
    case STATUS_BREAK:
      counterTime = 1 * 10 * 1000L;
      break;
    case STATUS_LONG_BREAK:
      counterTime = 20 * 60 * 1000L;
      break;
  }
  
  currentStatus = countType;
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
      lcd.print("Work");
      break;
    case STATUS_BREAK:
      lcd.print("Break");
      break;
    case STATUS_LONG_BREAK:
      lcd.print("LBreak");
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
    
  int innerStart = abs(remaining) / 1000;
  if(remaining < 0 )
  {
    lcd.print("-");
  }

  int minutes = innerStart / 60;
  int seconds = innerStart - minutes * 60;

  char lineBuffer[16];
  sprintf(lineBuffer,"%02d:%02d", minutes, seconds);
  lcd.write(lineBuffer);
}

int getButtonState(int pin) {
 int st = digitalRead(pin);
 delay(10);
 if(st == digitalRead(pin))
    return st;
    
 return 0;
}


void loop() {
 
 // updateTime();
  printTime();

  int buttonState = getButtonState(buttonWorkPin);
  if(buttonState == 1 && previousWorkButtonState == 0)
  {
    startCounting(STATUS_WORK);
  }
  previousWorkButtonState = buttonState;
  
  buttonState = getButtonState(buttonBreakPin);
  if(buttonState == 1 && previousBreakButtonState == 0)
  {
    startCounting(STATUS_BREAK);
  }
  previousBreakButtonState = buttonState;
  delay(50);
  Serial.print(digitalRead(buttonWorkPin));
  Serial.write(" ");
  Serial.print(digitalRead(buttonBreakPin));
  Serial.write(" status");
  Serial.print(currentStatus);
  Serial.println();
}

