/* File BT_tester
 * Designed to try out the bluetooth library
 * 17 Nov 2014
 * Brian Gravelle
*/

#include <Arduino.h>
#include <rn41_lib_sand.h> //this needs to be put in the ardunio library directory

#define masterMAC "123443211234"
#define slaveMAC "123443211234"

rn41 bt;

void masterTest(){
  Serial.println("--- Start Master Test ---");
  
  char* friendMac = slaveMAC;
  char msg[256];
  
  bt.setAsMaster();
  bt.addFriend(friendMac);
  delay(100);
  
  bt.makeMasterConnection();
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
  bt.addFriend(friendMac);
  delay(100);
  
  while(!bt.checkConnection()){}
  
  Serial.println("--- Connection Section Complete ---");
  
  bt.recieveMsg(msg);
  Serial.print("message recieved: ");
  Serial.println(msg);
    
  bt.sendMsg("yes master?");
  delay(100);
  
  Serial.println("--- message sent ---");
}

void setup() {
  bt = rn41();
  bt.btInit(); 
  Serial.begin(9600); 
}

void loop() 
{  
  //make two files when you're ready and then comment one out in each
  masterTest();
  slaveTest();
  delay(10000);
}

