//By Yixing Qie 
//Date: August 2018
#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#define  CE_PIN 7  
#define  CSN_PIN 8

#define Fmosfet               6 //forward mosfet
#define Bmosfet               3 //backward mosfet
#define FRelay                2 //forward polarity switch
#define BRelay                4 //backward polarity switch
#define Brake                 5 //brake relay

RF24 radio(CE_PIN, CSN_PIN);

byte addresses[][6] = {"test", "2Node"}; // Connection Pipes

//packet of position and button data
struct dataStruct {
  unsigned long _micros;  
  int Xposition;          
  int Yposition;
  bool switch1;          
  bool switch2;
} myData;              

int started_waiting_at;            
boolean timeout = false;                       


void setup()
{
  TCCR2B = (TCCR2B & 0b11111000) | 0x01;
  TCCR0B = TCCR0B & B11111000 | B00000001;
  Serial.begin(115200);
  pinMode(Fmosfet, OUTPUT);
  pinMode(Bmosfet, OUTPUT);
  pinMode(FRelay, OUTPUT);
  pinMode(BRelay, OUTPUT);
  pinMode(Brake, OUTPUT);

  radio.begin();          
  radio.setChannel(109);  
  radio.setDataRate(RF24_2MBPS); // Sends fastest
  radio.setAutoAck(false); //do not want pauses

  radio.setPALevel(RF24_PA_MAX);

  // Opens up connections
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

}//--(end setup )---
unsigned long prev = 0;
boolean found;
int  count = 0;
void loop()  
{

  radio.startListening();
  //used for autostop
//  timeout = false;

  if (radio.available()) {
    while (radio.available())   
    {
      radio.read( &myData, sizeof(myData) );             // Get the data
      count = 0;
    }
    radio.stopListening();
  }


//following code needs to be fixed to implement autostop feature when connection is lost
  /*
  radio.startListening();
  started_waiting_at = micros();
 // Serial.println(prev);
  //Serial.println(myData._micros);
if( prev == myData._micros){
  count++;
  }
    prev = myData._micros;
   
//  while (!radio.available()) {                            
//    if ((micros() - started_waiting_at) > 5000000 ) {           
//      count++;
//      break;
//    }
//  }
//Serial.println(myData._micros);

  if (count > 500) {
    count = 0;
 //   Serial.println("lost");
//    myData.Xposition = 517;         
//    myData.Yposition = 517;
//    myData.switch1 = 1;          
//    myData.switch2 = 1;
  }

*/

  double level  = map(abs(517 - myData.Xposition), 0, 520, 0 , 250);

  if (myData.Xposition >= 518 && myData.switch2 == 1 && myData.switch1 == 1) {
    analogWrite(Fmosfet, level);
    analogWrite(Bmosfet, 0);
    digitalWrite(BRelay, 0);
    digitalWrite(Brake, 0);
    digitalWrite(FRelay, 1);
         // count = 0;
    // Serial.println(F(" FORWARD"));
  } else if (myData.Xposition <= 515 && myData.switch2 == 0 && myData.switch1 == 1) {
    analogWrite(Bmosfet, level);
    analogWrite(Fmosfet, 0);
    digitalWrite(FRelay, 0);
    digitalWrite(Brake, 0);
    digitalWrite(BRelay, 1);
         // count = 0;
    // Serial.println(F(" BACKWARD"));
  } else if (myData.Xposition == 517) {
    analogWrite(Fmosfet, 0);
    analogWrite(Bmosfet, 0);
    digitalWrite(FRelay, 0);
    digitalWrite(BRelay, 0);
    digitalWrite(Brake, 0);
    //Serial.println(F(" NONE"));
  } else if (myData.Xposition <= 515 && myData.switch2 == 1 && myData.switch1 == 1) {
    analogWrite(Fmosfet, 0);
    analogWrite(Bmosfet, 0);
    digitalWrite(FRelay, 0);
    digitalWrite(BRelay, 0);
    digitalWrite(Brake, 1);
       //   count = 0;
    // Serial.println(F(" BRAKE"));
  }
}
