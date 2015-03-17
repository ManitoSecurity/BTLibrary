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
  turn_on_red();
  delay(1000);
  char* friendMac = slaveMAC;
  char msg[32];

  bt->setAsMaster();
  turn_on_green();
  delay(500);
  //bt->addFriend(friendMac);
  turn_on_red();
  delay(100);
  
  while(!bt->makeMasterConnection()){
    turn_on_blue();
    delay(100);
  }
  
  turn_on_green();
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
    
  bt->setAsSlave();
  delay(100);
  //bt->sendBtCmd("W");
  bt->sendBtCmd("Q,1");
  
  //bt->addFriend(friendMac);
  delay(1000);
  
  while(!bt->checkConnection()){
    turn_on_blue();
    delay(100);
    turn_on_red();
    delay(100);
  }
      
  bt->getReply(msg, true);

  if(msg[0] == 'A') turn_on_green();
  msg[0] = 'X';
  delay(1000);
  bt->sendMsg("B");
  turn_on_blue();
  delay(1000);
    
}


