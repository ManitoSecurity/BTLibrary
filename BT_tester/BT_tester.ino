/*
 *Bluetooth test
 *Brian Gravelle
*/

#include <Arduino.h>
#include "rgb_led.h"
#include "rn41_lib.h"


#define slaveMAC  "0006666D4320"
#define masterMAC "0006666D4351"

rn41 *bt;

void setup() {

  init_rgb_led();      
  //delay(3500);
  turn_on_blue();
  bt = new rn41();
}

void loop()
{ 	
   turn_on_red();
   delay(10);
   turn_on_blue();
   delay(10);
   slaveTest();
   //masterTest();
}

void masterTest(){
  char* friendMac = slaveMAC;
  char msg[32];

  turn_on_red();
  delay(1000);
  bt->setAsMaster();
  delay(100);
  bt->sendBtCmd("Q,0");
  
  turn_on_blue();
  while(!bt->makeMasterConnection()){
    turn_on_red();
    delay(100);
    turn_on_blue();
    delay(500);
  }
  
  turn_on_green();
  while(1);
  delay(500);
  bt->sendMsg("A");
  delay(100);
  turn_on_blue();

  bt->getReply(msg, true);
  if(msg[0] == 'B'){
    turn_on_green(); 
    msg[0] = 'X'; 
    msg[1] = '\0';
  }
  turn_on_green();
  delay(1000);
  while(true);
}

void slaveTest(){
  turn_on_red();
  char* friendMac = masterMAC;
  char msg[32];
    
  //bt->setAsSlave();
  delay(100);
  bt->sendBtCmd("Q,0");
  
  //bt->addFriend(friendMac);
  delay(1000);
  
  while(!bt->checkConnection()){
    turn_on_blue();
    delay(500);
    turn_on_red();
    delay(1500);
  }
  turn_on_green();
  while(1);  
  bt->getReply(msg, true);

  if(msg[0] == 'A') turn_on_green();
  msg[0] = 'X';
  delay(1000);
  bt->sendMsg("B");
  turn_on_blue();
  delay(1000);
    
}


