/*
 *Bluetooth test
 *Brian Gravelle
*/

#include <Arduino.h>
#include "rn41_lib_sand.h"


#define masterMAC "000666682E57"
#define slaveMAC "000666682E68"


rn41 bt;


void setup() {
  pinMode(RXPIN, INPUT);
  pinMode(TXPIN, OUTPUT);      
  delay(3500); //delay for 3.5s to let IR sensor get its ish together
  Serial.begin(115200);
  Serial.println("lets start this party \n");
  bt = rn41();
  bt.btInit(); 
}

void loop()
{ 

  delay(1000);

  masterTest();
  
  Serial.println("print this you turd!");
}

void masterTest(){
  Serial.println("--- Start Master Test ---");
  
  char* friendMac = slaveMAC;
  char msg[256];
  
  bt.setAsMaster();
  bt.addFriend(friendMac);
  delay(100);
  
  while(!bt.makeMasterConnection()){
    Serial.println("...");
    delay(10);
  }
  delay(100);
  
  Serial.println("--- Connection Section Complete ---");
    
  bt.sendMsg("Hello world :)");
  delay(100);
  
  Serial.println("--- message sent ---");
  
  bt.recieveMsg(msg);
  Serial.print("message recieved: ");
  Serial.println(msg);
    
}

void slaveTest(){
  Serial.println("--- Start Slave Test ---");
  
  char* friendMac = masterMAC;
  char msg[256];
    
  bt.setAsSlave();
  delay(100);
  bt.sendBtCmd("$$$", false);
  bt.sendBtCmd("W");
  bt.sendBtCmd("---");
  Serial.println("--- set to slave? ---");
  
  bt.addFriend(friendMac);
  delay(1000);
  
  while(!bt.checkConnection()){
    delay(500);
  }
  
  Serial.println("--- Connection Section Complete ---");
  
  bt.sendMsg("what's up dog?");
  
  bt.recieveMsg(msg);
  Serial.print("message recieved: ");
  Serial.println(msg);
    
  bt.sendMsg("yes master?");
  delay(100);
  
  Serial.println("--- message sent ---");
  
}


