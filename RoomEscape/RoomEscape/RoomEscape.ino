#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//GAME SETTING - STEP 0
#define start_btn 31
#define buzzer 32 //BUZZER
#define quiz1Led 41
#define quiz2Led 42
#define quiz3Led 43 // LED for quiz
int step = 0; // for step of game

//STEP 1 - START
#define elecMagnet 33


//STEP 2 - QUIZ 1. RFID

//STEP 3 - QUIZ 2. MORSE
#define touchPinL 2
#define touchPinS 3
int touchValL; // for long note
int touchValS; // for short note
LiquidCrystal_I2C lcdM(0x27, 16,2);  // I2C LCD 객체 선언 - pin 20 21
char answer[10] = "---...---";       // answer for Morse
char touching[10];                   // touching status
int touchedTime = 0;                 // touched time to check answer
//STEP 4 - QUIZ 3. LED

//STEP 5 - ESCAPED


void setup() {

  //STEP 0
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);  
  pinMode(start_btn, INPUT);
  pinMode(quiz1Led, OUTPUT);pinMode(quiz2Led, OUTPUT); pinMode(quiz3Led, OUTPUT); 

  //STEP 1 - START
  pinMode(elecMagnet, OUTPUT);

  //STEP 2 - 
  
  //STEP 3 - QUIZ 2. MORSE
   pinMode(touchPinL, INPUT);
   pinMode(touchPinS, INPUT);
   //touch sensor interrupt
  // attachInterrupt(digitalPinToInterrupt(touchPinL), touchedL, FALLING);
  // attachInterrupt(digitalPinToInterrupt(touchPinS), touchedS, FALLING);
   
}

void loop() {
  if(step == 0){
    int start = digitalRead(start_btn);
    if(start == 1){
      init_step1(); // start step 1;
    }
  }
  if(step == 1){ // STEP 1. start setting
    //nothing
    
    
  }
  else if(step == 2){// STEP 2 
    //get rfid tag read
    
  }
  else if(step == 3){// STEP 3
    if(touchedTime>0){ // write touching record in lcd
    lcdM.setCursor(0,1);
    lcdM.print(touching);
    }
    if(touchedTime==9){ // check if answer is right
      checkMorse();
      touchedTime =0;
    }
  }
  else if(step == 4){// STEP 4
    
  }else if(step == 5){// STEP 5
    
  }else{
    
  }
}
///////////////////////////////// FUNCTIONS /////////////////////////////////////////
/***************** STEP 0 ***********************/

void failed(){ //alert BUZZER, if available, time decrease
  digitalWrite(buzzer, HIGH);
  digitalWrite(buzzer, LOW);
  
}
/***************** STEP 1 ***********************/

void init_step1(){
  Serial.print("STEP 1");
  digitalWrite(elecMagnet, HIGH);
  
  
}
/***************** STEP 2 ***********************/

void init_step2(){
  Serial.print("STEP 2");
  
}
/***************** STEP 3 ***********************/

void init_step3(){ // INITIALIZE STEP 3
  Serial.print("STEP 3");
  //lit led 1
  lcdM.init();    
  // Print a message to the LCD.
  lcdM.backlight();

  lcdM.setCursor(0,0); //Start on line 0
  lcdM.print("Question1. Morse"); 
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

void checkMorse(){ // check the answer
  int diff = 0;
    for(int i=0;i<9;i++){
      if(answer[i] != touching[i]){diff = 1;}
    }
    if(diff==1){
      lcdM.setCursor(0,1);
      lcdM.print("FAILED");
      failed();
    }
    else{
      lcdM.setCursor(0,0);
      lcdM.print("ANS : SOS");
      lcdM.setCursor(0,1);
      lcdM.print("NEXT STAGE"); 
      init_step4();
    }
}
/***************** STEP 4 ***********************/
void init_step4(){
  Serial.print("STEP 4");
  
}
/***************** STEP 5 ***********************/
void init_step5(){
  Serial.print("STEP 2");
  
}
