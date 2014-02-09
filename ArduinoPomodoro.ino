#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
long counterTime = 60 * 60 * 1000L; 
long minutes = 0;
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
  lcd.print("idle...");
  lcd.setCursor(0,1);
  lcd.print("ready to work?");
}

int currentMillis = 0;
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

void startCounting(int countType) {
 switch(countType) {
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
  
  currentStatus = countType;
  startTime = millis();
}

void printTime() {    
  lcd.clear();
  
  long timeFromStart = millis() - startTime;
  long remaining = counterTime - timeFromStart;  
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
  lcd.print(remaining);
  lcd.setCursor(0,1);
  
  int innerStart = remaining / 1000;
  if(remaining < 0 )
  {
    innerStart = abs(remaining) / 1000;
  }
  minutes = innerStart / 60;
  if(minutes < 10 )
    lcd.print("0");
  if(remaining < 0) {
    lcd.print("-");
  }
  lcd.print(minutes);
  lcd.print(":");
  int seconds = innerStart - minutes * 60;
  if(seconds < 10)
    lcd.print("0");
  lcd.print(seconds);
}

int st= 0;
int currentState(int pin) {
 st = digitalRead(pin);
 delay(10);
 if(st == digitalRead(pin))
 {
    return st;
 }
 else
 {
   return 0;
 }
}


void loop() {
 
  updateTime();
  printTime();

  int cs = currentState(buttonWorkPin);
  if(cs == 1 && previousWorkButtonState == 0)
  {
    startCounting(STATUS_WORK);
  }
  previousWorkButtonState = cs;
  
  cs = currentState(buttonBreakPin);
  if(cs == 1 && previousBreakButtonState == 0)
  {
    startCounting(STATUS_BREAK);
  }
  previousBreakButtonState = cs;
  delay(50);
  Serial.print(digitalRead(buttonWorkPin));
  Serial.write(" ");
  Serial.print(digitalRead(buttonBreakPin));
  Serial.write(" status");
  Serial.print(currentStatus);
  Serial.println();
}

