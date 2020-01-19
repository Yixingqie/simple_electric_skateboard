//By Yixing Qie
//Date: August 2018
#include <SPI.h>   
#include "RF24.h"  
#define  CE_PIN  9   
#define  CSN_PIN 10

#define XPOS   A0  
#define YPOS   A1
#define JOYSTICK_SW  A2  
#define JOYSTICK_SW2  A3
RF24 radio(CE_PIN, CSN_PIN);

byte addresses[][6] = {"test", "test2"}; // pipe passwords


struct dataStruct {
  unsigned long _micros;  
  int Xposition;          
  int Yposition;
  bool switch1;         
  bool switch2;
} myData;                


void setup()   
{
  Serial.begin(115200);  
  
  pinMode(JOYSTICK_SW, INPUT_PULLUP);  
  pinMode(JOYSTICK_SW2, INPUT_PULLUP);

  radio.begin();          
  radio.setChannel(109);  
  radio.setDataRate(RF24_1MBPS); // fastest
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MAX); //max range

  //opens connection
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);



void loop()   //constantly sends data
{
    radio.stopListening();                                    
    myData.XPOSposition = analogRead(XPOS);
    Serial.print( myData.XPOSposition);
    
    myData.Yposition = analogRead(YPOS);
    myData.switch1  = digitalRead(JOYSTICK_SW);  
    myData.switch2 = digitalRead(JOYSTICK_SW2);
  }

  myData._micros = micros();  
  radio.write( &myData, sizeof(myData) );           //sends data
  delay(5);

}
