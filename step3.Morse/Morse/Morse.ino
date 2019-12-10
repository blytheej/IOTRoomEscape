#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define touchPinL 2
#define touchPinS 3

int touchValL; // long note
int touchValS; // short note
int step = 2;
//LCD
LiquidCrystal_I2C lcd(0x27, 16,2);  // I2C LCD 객체 선언


void setup() {
   Serial.begin(115200);
   pinMode(touchPinL, INPUT);
   pinMode(touchPinS, INPUT);
   //touch sensor interrupt
   attachInterrupt(digitalPinToInterrupt(touchPinL), touchedL, FALLING);
   attachInterrupt(digitalPinToInterrupt(touchPinS), touchedS, FALLING);
   
  lcd.init();    
  // Print a message to the LCD.
  lcd.backlight();

  lcd.setCursor(0,0); //Start at character 3 on line 0
  lcd.print("Question1. Morse"); // move to step3 init

}

char answer[10] = "---...---";
char touching[10];
int touchedTime = 0;

void loop() {
  if(step == 2){
  
  if(touchedTime>0){
    lcd.setCursor(0,1);
    lcd.print(touching);
  }
  if(touchedTime==9){
    checkMorse();
    touchedTime =0;
  }
  }
  delay(1000);
}
void checkMorse(){
  int diff = 0;
    for(int i=0;i<9;i++){
      if(answer[i] != touching[i]){diff =1;}
    }
    if(diff==1){
      lcd.setCursor(0,1);
      lcd.print("FAILED");
    }
    else{
      lcd.setCursor(0,0);
      lcd.print("ANS : SOS");
      lcd.setCursor(0,1);
      lcd.print("NEXT STAGE"); 
      // next stage init() - step4();
    }
}
void touchedL(){
  touching[touchedTime] = '-';
  touchedTime++;
  Serial.print('-');
}

void touchedS(){
  touching[touchedTime] = '.';
  touchedTime++;
  Serial.print('.');
}
