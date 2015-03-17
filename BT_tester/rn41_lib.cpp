/**
  * @file rn41_lib.cpp
  * @date March 14 2015
  * @organization Manito Security Solutions
  * @author Brian Gravelle (bgravelle@zagmail.gonzaga.edu)
  * implemetation file for communication to rn-41 bluetooth module from arduino
*/

#include "rn41_lib.h"

/*
  @construct rn41 object
  @pre
  @post rn-14 object exists
  @usage
*/
rn41::rn41(){

  isMaster   = false;
  msgToken   = 0;
  numFriends = 0;
  connected  = false;
  bluetooth = new SoftwareSerial(RXPIN, TXPIN);
  bluetooth->begin(BAUDRATE);
  checkConnection();

}


/*
  @sets rn-41 object to master mode for controlling communication with other bt devices
  @pre
  @post set as master and rebooted
  @usage setAsMaster();
*/
void rn41::setAsMaster(){
  sendBtCmd("SM,1");
  reboot();
  isMaster = true;
  delay(100);
}

/*
  @sets rn-41 object to slave mode for controlling communication with other bt devices
  @pre
  @post set as slave and rebooted
  @usage setAsSlave();
*/
void rn41::setAsSlave(){
  sendBtCmd("SM,0");
  reboot();
  isMaster = false;
  delay(100);
}

/*
  @reboots rn-41 module- necessary to change settings
  @pre
  @post rebooted module, settings updated
  @usage reboot(); or bt_obj.reboot();
*/
void rn41::reboot(){
  sendBtCmd("R,1");
}

/*
  @looks for other BT devices (bt inquiry)
  @pre module is set as master
  @post the module should return up to 9 devices in the form:
    <BT address>,<BT name>,<COD> CR <BT address>,<BT name>,<COD> CR "Inquiry Done" CR
  @usage
*/
void rn41::lookForDevices(){\
  sendBtCmd("I,5");
}

/*
  @pulls macs out from BT response to inquiry
  @note this is crap it just grabs the first one
  @pre inquiry complete; module is master; BTMsg holds inquiry response
  @post
  @returns first device found
  @usage getMacFromInquiry(address);
*/
char* rn41::getMacFromInquiry(char* mac){
  int i;
  for(i =0; i<13; i++){
    mac[i] = BTMsg[i];
  }
  return mac;
}

/*
  @tries to enslave some bt device
  @pre module is master; mac is some located device
  @post hopefully connected
  @returns true or false based on connection success
  @usage
*/
bool rn41::offerConnection(char* mac){
  BTCmd[0] = 'C'; //C,address is command to make connection
  BTCmd[1] = ',';

  for(int i = 2; i < 14; i++){
    BTCmd[i] = mac[i - 2];
  }
  BTCmd[15] = '\0';

  //sendBtCmd(BTCmd);
  //sendBtCmd("C,0006666D4320");
    sendBtCmd("SR,0006666D4320");
    reboot();
    sendBtCmd("C");
}

/*
  @adds a bt mac to list of friends
  @pre mac is a 12 char long char*
  @post mac is newest friend; numfriends++
  @usage addFriend(mac);
*/
void rn41::addFriend(char* mac){
  if(numFriends < 7){
    for(int i = 0; i < 13; i++)
      Friends[numFriends][i] = mac[i];
    numFriends++;
  }
}

/*
  @sends command to rn41 object
  @commands https://www.sparkfun.com/datasheets/Wireless/Bluetooth/rn-bluetooth-um.pdf
  @pre BtCmd holds ASCII command, if true or empty add newline after command
  @post command sent to bt module
  @usage sendCmd(BTCmd);
*/
 void rn41::sendBtCmd(char* cmd, bool need_ln){
  delay(1000);
  bluetooth->print('$');
  bluetooth->print('$');
  bluetooth->print('$');
  delay(1000);
  if(need_ln){
    bluetooth->println(cmd);
  } else{
    bluetooth->print(cmd);
  }

  delay(100);
  bluetooth->println("---");
  delay(100);
}

/*
  @gets response from rn-41 (non-blocking)
  @pre command sent to local bt module; expecting response
  @post response in reply
  @param bock should be true for blocking otherwise leave default false
  @usage getReply(BTReply);
*/
void rn41::getReply(char* reply, bool block){
  int i = 0;
  delay(100);
  while( (bluetooth->available() == 0) && block );
  while(bluetooth->available() > 0) 
  {
    reply[i] = (char)bluetooth->read();  
    i++;
  }
  reply[i] = '\0';
  delay(100);
}

/*
  @cleans serial buffer from rn-41
  @usage purgeReply();
*/
void rn41::purgeReply(){
  int i = 0;
  delay(100);
  while(bluetooth->available() > 0)
  {
    bluetooth->read();  
  }
}

/*
 @sends message over BT connection via rn41 object
 @pre
 @post message sent
 @usage sendMsg(BTMsg);
*/
void rn41::sendMsg(char* msg){
  delay(50);
  bluetooth->println(msg);
  delay(50);
}

/*
  @check if mac is friendly
  @pre mac is some BT mac address
  @returns true if mac is in Friends array
  @usage if(isFriend(mac)) {do stuff}
*/
bool rn41::isFriend(char* mac){
  for(int i = 0; i < numFriends; i++){
    if(!strcmp(mac, Friends[i]))
      return true;
    else
      return false;
  }
}

/*
  @check if inquiry is finished
  @pre you did the inquiry thing (lookForDevices)
  @returns true if inquiry has finished
  @usage if(doneLooking()) {do stuff}
*/
bool rn41::doneLooking(){
  getReply(BTReply);
  int msgLen = strlen(BTReply);
  if(msgLen >= 12){
    char* doneMsg = &BTReply[msgLen - 12];
    if(!strcmp(doneMsg, "Inquiry Done"))
      return true;
  }
  return false;
}

/*
  @check if the module is connected
  @pre BTMsg is not in use
  @post updates connected variable
  @returns true if connected otherwise false
  @usage if(checkConnection()) {do things}
*/
bool rn41::checkConnection(){
  delay(100);
  sendBtCmd("GK");  
  getReply(BTReply);
  if(BTReply[0] == '1') connected = true;
  else connected = false;
  return connected;
}


/*
  @find devices and offer connection if friends
  @pre module is set to be master; must find friend mac
  @post
  @returns true if successful connection else false
  @usage makeMasterConnection();
*/
bool rn41::makeMasterConnection(){

  /*
  lookForDevices();    
  BTReply[0] = '\0';
  int i = 0;
  int maxTries = 10;
  while(!connected){
      delay(100);
      getReply(BTReply);
      int msgLen = strlen(BTReply);
      if(doneLooking()){

        //getMacFromInquiry(connectionAdr);
        if(isFriend(connectionAdr)){
          offerConnection(connectionAdr);
          delay(2000);
          checkConnection();
        }
     }
     i++;
     if(i > maxTries) break;
  }
  */
  
  offerConnection("0006666D4320");
  return checkConnection();
}

