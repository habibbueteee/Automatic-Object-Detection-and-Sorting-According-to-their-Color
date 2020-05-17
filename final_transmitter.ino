
//transmitter of remote control fan/light
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg[2];
RF24 radio(7,8);
const uint64_t pipe = 0xE8E8F0F0E1LL; 
volatile int recValue=0;

//int speed=0;
// int regu=A0;
 
void setup(void){
  Serial.begin(9600);
  
  radio.begin();
  radio.setDataRate (RF24_250KBPS); // Transmission speed
radio.setChannel (100); // Channel number from 0 to 127
radio.setRetries (15,15); 

  radio.openWritingPipe(pipe);
//pinMode(light,INPUT_PULLUP);
//pinMode(fan,INPUT_PULLUP);
}
 
void loop()
{ 
    if(Serial.available())
    {
    recValue=Serial.read();
    //Serial.println(recValue);
    msg[0]=recValue;
    msg[1]=msg[0];

    
  radio.write (msg,2);
    }
   
 }

