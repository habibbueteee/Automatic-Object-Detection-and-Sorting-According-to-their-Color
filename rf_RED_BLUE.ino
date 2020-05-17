//code for the arduino MEGA board on the robot.
#include <SPI.h>
#include <Servo.h>
#include <SharpIR.h>
#include "nRF24L01.h"
#include "RF24.h"

#define ir1 A0
#define model 1080//SharpIR model

//RF module
int msg[2];
RF24 radio(9,53);
const uint64_t pipe = 0xE8E8F0F0E1LL;//radio pipe

//task complete indicator
volatile int finish=0;

//SERVO
Servo myservo1;
Servo myservo2;
volatile int dis1 = 120;
volatile int picked = 0;
//SharpIR
SharpIR sharp1(ir1, 100, 93, model);
//Wheels
int right_front = 2;
int right_back = 3;
int right_en = 4;//PWM

int left_front = 6;
int left_back = 7;
int left_en = 8;//PWM

//indicator LEDs to show which direction command is being executed
int ls=13;
int fs=11;
int rs=12;
int rn=10;

volatile char a = 0;

void setup(void) {
  
  delay(2000);

   Serial.begin(9600);

  //servo setup
  myservo1.attach(45);
  myservo2.attach(44);

//radio setup
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // Transmission speed
radio.setChannel (100); // Channel number from 0 to 127
radio.setRetries (15,15); // 
radio.openReadingPipe(1,pipe);
radio.startListening();

 //servo initial position
  //remember to use for loop here finally to calculate angle of servo
  for (int i = 20; i >= 5; i = i - 1)
  { myservo1.write(i);
    delay(50);
  }

  myservo2.write(20);
  delay(1000);


 
  pinMode(left_front, OUTPUT);
   pinMode(rn, OUTPUT);


  pinMode(left_back, OUTPUT);
  pinMode(right_front, OUTPUT);
  pinMode(right_back, OUTPUT);
  pinMode(left_en, OUTPUT);
  pinMode(right_en, OUTPUT);
  pinMode(ls, OUTPUT);
  pinMode(fs, OUTPUT);
  pinMode(rs, OUTPUT);
  pinMode(32, OUTPUT);

  pinMode(ir1, INPUT);
}



void loop()
{ if(finish==0){
  //LOOKING FOR RED OBJECT
  dis1 = sharp1.distance();

if(picked==0){

  if (dis1 >= 13 && picked == 0) {
    
    if (radio.available()) {
      radio.read(msg,2);

      switch (msg[0]) {
        case 'F' ://front
          front();
          
          break;
          
        case 'R' ://right front
          rightf();
          break;

        case 'L' ://left front
          leftf();
          break;

        case 'N':
         randomrun();
          break;
          case 'f':
          randomrun();
          break;

          case 'r':
          randomrun();
          break;

          case 'l':
          randomrun();
          break;

         
      }

      
    }//if radio available end


  }//if dis>12 end
  else if (picked == 0 && dis1 < 10) 
  {
    back();
  }
  else if (picked == 0 && dis1 <= 13 && dis1 >= 10)
  { //pickup the object
   pickobject();
  }
}

//for picked ==1
else if(picked==1){

  if (dis1 >= 13 && picked ==1) {
   
    if (radio.available()) {
      


       radio.read(msg,2);

      switch (msg[0]) {
        case 'f' ://front
          front();
          
          break;
          
        case 'r' ://right front
          rightf();
          break;

        case 'l' ://left front
          leftf();
          break;

        case 'N':
          randomrun();
          break;

         
      }

      
    }//if radio available end


  }//if dis>12 end
  else if (picked == 1 && dis1 < 10) 
  {
    back();
  }
  else if (picked == 1 && dis1 <= 13 && dis1 >= 10)
  { //stop when less than 12
   dropobject();
  }
}// if picked==1


}// finish=0
}//loop


void front() {
//
digitalWrite(fs,1);
digitalWrite(ls,0);
digitalWrite(rs,0);
digitalWrite(rn,0);

  //delay(20);
  digitalWrite(left_back, 0);
  digitalWrite(left_front, 1);
  digitalWrite(right_front, 1);
  digitalWrite(right_back, 0);
  
  analogWrite(left_en, 195);
  analogWrite(right_en, 220);
  //the difference between two PWMs is that 
  //due to mechanical error,the two wheels 
  //of the robot turned with different speeds,
  //hence the PWMs are calibrated
  
  delay(100);

  //delay(500);
  //delay(20);

}

void back() {

  //delay(20);
  digitalWrite(left_back, 1);
  digitalWrite(left_front, 0);
  digitalWrite(right_front, 0);
  digitalWrite(right_back, 1);
  analogWrite(left_en, 160);
  analogWrite(right_en, 160);
  delay(300);

}


void rightf() {
 
  
digitalWrite(fs,0);
digitalWrite(ls,0);
digitalWrite(rs,1);
digitalWrite(rn,0);
  digitalWrite(left_back, 0);
  digitalWrite(left_front, 1);
  digitalWrite(right_front, 1);
  digitalWrite(right_back, 0);
  analogWrite(left_en, 220);
  analogWrite(right_en, 155);
  delay(100);

  //delay(50);
}


void leftf() {

digitalWrite(fs,0);
digitalWrite(ls,1);
digitalWrite(rs,0);
digitalWrite(rn,0);
  //delay(20);
  digitalWrite(left_back, 0);
  digitalWrite(left_front, 1);
  digitalWrite(right_front, 1);
  digitalWrite(right_back, 0);
  analogWrite(left_en, 150);
  analogWrite(right_en, 230);
  //delay(180);

  delay(100);
}


void randomrun() {
  //delay(20);
  digitalWrite(fs,0);
digitalWrite(ls,0);
digitalWrite(rs,0);
digitalWrite(rn,1);
  digitalWrite(left_back, 1);
  digitalWrite(left_front, 0);
  digitalWrite(right_front, 1);
  digitalWrite(right_back, 0);
  analogWrite(left_en, 130);
  analogWrite(right_en, 130);
  delay(100);




}

void STOP() {

  digitalWrite(left_back, 0);
  digitalWrite(left_front, 0);
  digitalWrite(right_front, 0);
  digitalWrite(right_back, 0);
  analogWrite(left_en, 0);
  analogWrite(right_en, 0);
  delay(50);

}

void pickobject()
{
   STOP();
   
    for (int j = 20; j < 170; j=j+2) {
      myservo2.write(j);
      delay(10);
    }
    delay(500);


    for (int k = 5; k < 179; k=k+2) {
      myservo1.write(k);
      delay(10);
    }

    delay(500);

    for (int l = 169; l > 20; l--) {
      myservo2.write(l);
      delay(20);
    }
    delay(500);

    for (int m = 178; m >= 5; m--) {
      myservo1.write(m);
      delay(10);
    }
    delay(500);


    //up


    picked = 1;
    back();
  }


  void dropobject(){

    STOP();

    for (int j = 5; j <=155; j++) {
      myservo1.write(j);
      delay(10);
    }
    delay(500);


    for (int k = 20; k < 169; k++) {
      myservo2.write(k);
      delay(10);
    }

    delay(500);

    for (int l = 155; l >= 5; l--) {
      myservo1.write(l);
      delay(20);
    }
    delay(500);

    for (int m = 169; m >= 20; m--) {
      myservo2.write(m);
      delay(10);
    }
    delay(500);

    picked = 0;
    dropback();
    STOP();
    
    finish=1;
    digitalWrite(32,1);//BUZZER sounds
    delay(1000);
    digitalWrite(32,0);
  }

  void dropback()
  {
    digitalWrite(left_back, 1);
    digitalWrite(left_front, 0);
    digitalWrite(right_front, 0);
    digitalWrite(right_back, 1);
    analogWrite(left_en,200);
    analogWrite(right_en,200);
    delay(680);

    digitalWrite(left_back, 0);
    digitalWrite(left_front, 0);
    digitalWrite(right_front, 0);
    digitalWrite(right_back, 0);
    analogWrite(left_en,0);
    analogWrite(right_en,0);
    delay(20);
    }



