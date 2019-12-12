#include <LiquidCrystal_I2C.h>
#include <Wire.h>
int button_cont[3] = {2,3,18};  // Blue Yellow Red buttons
int pin_LED[3] = {44,45,46};     // for Blue Yellow Red LED

LiquidCrystal_I2C lcd(0x27, 16,2);  // I2C LCD 

void setup() {
  Serial.begin(115200);
  char  i;
  // 버튼이 연결된 핀들의 pin mode를 input으로 설정
  for(i=0; i<3; i++) {
    pinMode(button_cont[i], INPUT);
  }
  // led가 연결된 핀들의 pin mode를 output으로 설정
  for(i=0; i<3; i++) {
    pinMode(pin_LED[i], OUTPUT);
  }
  //버튼이 눌리면 인터럽트 발생!
  attachInterrupt(digitalPinToInterrupt(button_cont[0]),pressedBlue, FALLING);
  attachInterrupt(digitalPinToInterrupt(button_cont[1]),pressedYellow, FALLING);
  attachInterrupt(digitalPinToInterrupt(button_cont[2]),pressedRed, FALLING);

  
  lcd.init();    
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0); //Start at character 3 on line 0
  lcd.print("Question2"); 
  lcd.setCursor(0,1); 
  lcd.print("Memory game"); 
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("see the lights");
  lcd.setCursor(0,1);
  lcd.print("and remember");
  demonstrate();
  lcd.clear();
}

int butwait = 500;  //버튼이 눌러지기 전까지 대기하는 시간
int ledtime =500;   //led 점등 시간
int right = 0; //wheter answer is correct or not
int pressedNum = 0;
char answer[5] = { 'B' , 'Y', 'R', 'B', 'Y'};
char playerAnswer[5];

void loop() {
  if(pressedNum==0){
    lcd.setCursor(0,0);
    lcd.print("now press the");
    lcd.setCursor(0,1);
    lcd.print("button orderly");
  }

  if(pressedNum <5){ //버튼 입력이 완료되면
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("entering...");
  }else if(pressedNum ==5){
    lcd.setCursor(0,0);
    lcd.print("checking...");
    checkAnswer();
  }
}

void checkAnswer(){
  int i;
  int wrong = 0;
  for(i=0; i<5; i++){
    if(answer[i] != playerAnswer[i]){
      wrong =1;
    }
  }
  if(wrong==1){
    lcd.setCursor(0,1);
    lcd.print("FAILED");
  }else {
    lcd.setCursor(0,1);
    lcd.print("NEXT STAGE");
  }
  
}
int demonstrate(){   //문제 출제. answer에 따라 led 깜빡이기
  int i;
  for(i=0; i<5; i++) {
    if(answer[i] == 'B'){
      digitalWrite(pin_LED[0], HIGH);
      delay(1000);
      digitalWrite(pin_LED[0], LOW);
      delay(1000);
    }else if(answer[i] == 'Y'){
      digitalWrite(pin_LED[1], HIGH);
      delay(1000);
      digitalWrite(pin_LED[1], LOW);
      delay(1000);
    }else{
      digitalWrite(pin_LED[2], HIGH);
      delay(1000);
      digitalWrite(pin_LED[2], LOW);
      delay(1000);
    }
    delay(500);
  }
}
//Interrupt 발생
void pressedBlue(){
  playerAnswer[pressedNum] = 'B';
  Serial.print("blue button is pressed");
  Serial.print(playerAnswer);
  //digitalWrite(pin_LED[0],HIGH);
  pressedNum++;
}
void pressedYellow(){
  playerAnswer[pressedNum] = 'Y';
  Serial.print("yellow button is pressed");
  Serial.print(playerAnswer);
  //digitalWrite(pin_LED[1],HIGH);
  pressedNum++;
}
void pressedRed(){
  playerAnswer[pressedNum] = 'R';
  Serial.print("red button is pressed");
  Serial.print(playerAnswer);
  //digitalWrite(pin_LED[2],HIGH);
  pressedNum++;
}
