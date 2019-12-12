#include "WiFiEsp.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
//#include <MFRC522.h>

//GAME SETTING - STEP 0
#define wifiRX 14
#define wifiTX 15
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial3(wifiRX, wifiTX);
#endif
#define start_btn 31
#define buzzer 32 //BUZZER
#define elecMagnet 33
#define quiz1Led 41
#define quiz2Led 42
#define quiz3Led 43 // LED for quiz

int step = 4; // for step of game
char ssid[] = "Te";            // WIFI SSID
char pass[] = "25672567";        // WIFI PASSWORD
int status = WL_IDLE_STATUS; 
WiFiEspServer server(80);
//STEP 1 - START
#define RST_PIN         5           
#define SS_PIN          53         
//MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//STEP 2 - QUIZ 1. 타이머

//STEP 3 - QUIZ 2. MORSE
#define touchPinL 2
#define touchPinS 3
int touchValL; // for long note
int touchValS; // for short note
LiquidCrystal_I2C lcd(0x27, 16,2);  // I2C LCD 객체 선언 - pin 20 21
char answer1[11] = "..----.-..";       // answer for Morse - IOTRE
char touching[11];                   // touching status
int ans_len = 10;
int touchedTime = 0;                 // touched time to check answer

//STEP 4 - QUIZ 3. Memory Game
int button_cont[3] = {2, 3, 18};  // Blue Yellow Red buttons
int pin_LED[3] = {44, 45, 46};     // for Blue Yellow Red LED
int butwait = 500;  //버튼이 눌러지기 전까지 대기하는 시간
int ledtime =500;   //led 점등 시간
int right = 0; //whether answer is correct or not
int pressedNum = 0;
char answer2[6] = { 'B' , 'Y', 'R', 'B', 'Y'};
char playerAnswer[6]="     ";

//STEP 5 - ESCAPED


void setup() {

  //STEP 0
  Serial.begin(115200);
  Serial3.begin(115200);
  //WiFi.init(&Serial3);
  pinMode(buzzer, OUTPUT);  
  pinMode(start_btn, INPUT);
  pinMode(quiz1Led, OUTPUT);
  pinMode(quiz2Led, OUTPUT);
  pinMode(quiz3Led, OUTPUT); 
  
 /*
  if (WiFi.status() == WL_NO_SHIELD) {  // check for the presence of the shield
    Serial.println("WiFi shield not present");
    // don't continue
  }
  while ( status != WL_CONNECTED) {  // attempt to connect to WiFi network
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("You're connected to the network");
    printWifiStatus();
  server.begin();// start the web server on port 80
  */
  
  //STEP 1 - START
  pinMode(elecMagnet, OUTPUT);

  //STEP 2 - 타이머
  
  //STEP 3 - QUIZ 2. MORSE
   pinMode(touchPinL, INPUT);
   pinMode(touchPinS, INPUT);
   //touch sensor interrupt
   //attachInterrupt(digitalPinToInterrupt(touchPinL), touchedL, FALLING);
   //attachInterrupt(digitalPinToInterrupt(touchPinS), touchedS, FALLING);
   

  //STEP 4 - QUIZ 3. Memory Game
  init_step4();
  //버튼이 눌리면 인터럽트 발생!
  pinMode(button_cont[0], INPUT);
  pinMode(button_cont[1], INPUT);
  pinMode(button_cont[2], INPUT);
  attachInterrupt(digitalPinToInterrupt(button_cont[0]),pressedBlue, FALLING);
  attachInterrupt(digitalPinToInterrupt(button_cont[1]),pressedYellow, FALLING);
  attachInterrupt(digitalPinToInterrupt(button_cont[2]),pressedRed, FALLING);
}

void loop() {
  if(step == 0){
    int start = digitalRead(start_btn);
    if(start == 0){
      init_step1(); // start step 1;
    }    
  }
  if(step == 1){ // STEP 1. start setting
    //wifi
    // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");
          
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
            "\r\n");
          client.print("<!DOCTYPE html><head><meta charset=\"utf-8\"></head><body style=\"text-align:center;background: black;color:white; font-size:60px;padding:300px 0;\"> <div style=\"font-weight: bold;\"><div> 문이 닫힙니다</div><div style=\"color:red\">      문제를 풀고 방을 탈출하세요.    </div>    <div >       주어진 시간 10분    </div>  </div> </body></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1000);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
    
    
  }
  else if(step == 2){// STEP 2 
    //get rfid tag read
    
  }
  else if(step == 3){// STEP 3
    if(touchedTime>0){ // write touching record in lcd
    lcd.setCursor(0,1);
    lcd.print(touching);
    }
    if(touchedTime==ans_len){ // check if answer is right
      checkMorse();
      touchedTime =0;
    }
  }
  else if(step == 4){// STEP 4
    if(pressedNum == 0){
      lcd.setCursor(0,0);
      lcd.print("now press the");
      lcd.setCursor(0,1);
      lcd.print("button orderly");
    }
    else if(pressedNum < 5){ //버튼 입력이 완료되면
      lcd.setCursor(0,0);
      lcd.print("entering...");
      lcd.setCursor(0,1);
      lcd.print(playerAnswer);
    }
    else if(pressedNum >= 5){
      lcd.setCursor(0,0);
      lcd.print("checking...");
      checkAnswer();
      pressedNum  = 0;
    }
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
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
/***************** STEP 1 ***********************/

void init_step1(){
  Serial.println("\nSTEP 1");
  digitalWrite(elecMagnet, HIGH);
  step=1;
  //timer start
  
  
}
/***************** STEP 2 ***********************/

void init_step2(){
  Serial.println("\nSTEP 2");
  step=2;
  
}
/***************** STEP 3 ***********************/

void init_step3(){ // INITIALIZE STEP 3
  Serial.println("\nSTEP 3");
  digitalWrite(quiz1Led, HIGH);//lit led 1
  lcd.init();    
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0); //Start on line 0
  lcd.print("Question1. Morse"); 
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
    for(int i=0;i<ans_len;i++){
      if(answer1[i] != touching[i]){diff = 1;}
      touching[i] = ' ';
    }
    if(diff==1){
      lcd.setCursor(0,1);
      lcd.print("FAILED");
      Serial.println("\nFAILED"); 
      failed();
    }
    else{
      lcd.setCursor(0,0);
      lcd.print("ANS : IOTRE");
      lcd.setCursor(0,1);
      lcd.print("NEXT STAGE"); 
      Serial.println("\nANS : IOTRE");
      Serial.println("NEXT STAGE");
      init_step4(); // go to next step -> 4
    }
}
/***************** STEP 4 ***********************/
void init_step4(){
   Serial.println("\nSTEP 4");
   step=4;
   digitalWrite(quiz2Led, HIGH);//lit led 1
   //////////
   lcd.init();    
   lcd.backlight();
   ///////////////////
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
int demonstrate(){   //문제 출제. answer에 따라 led 깜빡이기
  int i;
  for(i=0; i<5; i++) {
    if(answer2[i] == 'B'){
      digitalWrite(pin_LED[0], HIGH);
      delay(1000);
      digitalWrite(pin_LED[0], LOW);
      delay(1000);
    }else if(answer2[i] == 'Y'){
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
  Serial.println("blue button is pressed");
  Serial.println(playerAnswer);
  //digitalWrite(pin_LED[0],HIGH);
  pressedNum++;
}
void pressedYellow(){
  playerAnswer[pressedNum] = 'Y';
  Serial.println("yellow button is pressed");
  Serial.println(playerAnswer);
  //digitalWrite(pin_LED[1],HIGH);
  pressedNum++;
}
void pressedRed(){
  playerAnswer[pressedNum] = 'R';
  Serial.println("red button is pressed");
  Serial.println(playerAnswer);
  //digitalWrite(pin_LED[2],HIGH);
  pressedNum++;
}
void checkAnswer(){
  int wrong = 0;
  for(int i=0; i<5; i++){
    if(answer2[i] != playerAnswer[i]){
      wrong = 1;
    }
    playerAnswer[i] = ' ';
  }
  if(wrong == 1){
    lcd.setCursor(0,1);
    lcd.print("FAILED");
    failed();
    demonstrate();
  }else {
    lcd.setCursor(0,1);
    lcd.print("NEXT STAGE");
  }
}
/***************** STEP 5 ***********************/
void init_step5(){
   Serial.println("\nSTEP 5");
   digitalWrite(quiz3Led, HIGH);//lit led 1
   digitalWrite(elecMagnet, LOW); // turn of magnet
   //타이머 멈춤 
   lcd.setCursor(0,0);
   lcd.print("SUCCESS!");
   lcd.setCursor(0,1);
   lcd.print("GO OUT");
  
}
