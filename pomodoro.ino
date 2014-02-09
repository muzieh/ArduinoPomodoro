

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
long counterTime = 60 * 60 * 1000L; 
long minutes = 0;
long startTime =0;
int buttonWorkPin = 8;
int buttonBreakPin = 9;

#define STATUS_STOP 1
#define STATUS_WORK 2
#define STATUS_BREAK 3
#define STTATUS_LONG_BREAK 4

int currentStatus = STATUS_STOP;

int previousState = 0;
void setup() {
  pinMode(buttonWorkPin, INPUT);
  pinMode(buttonBreakPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("idle...");
  lcd.setCursor(0,1);
  lcd.print("ready to work?");
  //Serial.begin(9600);
  //startTime = millis();


}

void updateTime() {

}

void printTime() {
  lcd.clear();
  
  long timeFromStart = millis() - startTime;
  long remaining = counterTime - timeFromStart;  
  lcd.print(remaining);
  //Serial.println(counterTime);
  //Serial.println(remaining);
  //lcd.print(" ");
  //lcd.print(remaining);
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
int currentState() {
 st = digitalRead(buttonWorkPin);
 delay(10);
 if(st == digitalRead(buttonWorkPin))
 {
    return st;
 }
 else
 {
   return 0;
 }
}

void resetTimer() {
  startTime = millis();
}
void loop() {
 
  printTime();

  int cs = currentState();
  if(cs == 1 && previousState == 0)
  {
    resetTimer();
  }
  previousState = cs;
  delay(50);
  Serial.print(digitalRead(buttonWorkPin));
  Serial.write(" ");
  Serial.print(digitalRead(buttonBreakPin));
  Serial.println();
}

