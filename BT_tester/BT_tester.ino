/*
 *Bluetooth test
 *Brian Gravelle
*/

#include "rgb_led.h"
#include <Arduino.h>
//#include "rn41_lib.h"


#define masterMAC "0006666D4320"
#define slaveMAC  "0006666D4351"


#define LED_G 6
#define LED_R 9
#define LED_B 3

//rn41 bt;


void setup() {
	pinMode(LED_G, OUTPUT);
	pinMode(LED_R, OUTPUT);
	pinMode(LED_B, OUTPUT);
	/*
  pinMode(RXPIN, INPUT);
  pinMode(TXPIN, OUTPUT);      
  delay(3500);
  Serial.begin(BAUDRATE);
  Serial.println("lets start this party \n");
  bt = rn41();
  bt.btInit(); */ 
}

void loop()
{ 
	
  delay(3000);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_B, LOW);

  delay(3000);
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);

  delay(3000);
  digitalWrite(LED_B, HIGH);
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);

  //masterTest();
  
}
/*
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
    
  bt.recieveMsg(msg);
  Serial.print("message recieved: ");
  Serial.println(msg);
    
  bt.sendMsg("yes master?");
  delay(100);
  
  Serial.println("--- message sent ---");
  
}*/


