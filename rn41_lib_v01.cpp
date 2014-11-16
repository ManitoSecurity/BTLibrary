/**
  * @file rn41_lib_sand.cpp
  * @version developmental (sandbox)
  * @date 15 November 2014
  * @author Brian Gravelle (bgravelle@zagmail.gonzaga.edu)
  * implemetation file for communication to rn-41 bluetooth module from arduino
*/

#include "rn41_lib_sand.h"

/*
  @construct rn41 object
  @pre
  @post rn-14 object exists
  @usage
*/
rn41(){
  masterPin  = 4;
  isMaster   = 0;
  msgToken   = 0;
  NumFriends = 0;
  connected  = false;
  myAdr      = BTAddress;
}

/*
  @initialize rn-14 for use
  @pre
  @post rn-14 ready to rock and roll
  @usage
*/
void btInit(){
  myAddr = MYADDRESS;
  Serial.begin(BAUDRATE);
  checkConnection();
  sendBtCmd("$$$", false);
  setAsSlave();
}

/*
  @sets rn-41 object to master mode for controlling communication with other bt devices
  @pre
  @post set as master and rebooted
  @usage setAsMaster();
*/
void setAsMaster(){
  sendBtCmd("SM,1");
  sendBtCmd("---");
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
void setAsSlave(){
  sendBtCmd("SM,0");
  sendBtCmd("---");
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
void reboot(){
  sendBtCmd("R,1");
  sendBtCmd("---");
}

/*
  @looks for other BT devices (bt inquiry)
  @pre module is set as master
  @post the module should return up to 9 devices in the form:
        <BT address>,<BT name>,<COD> CR <BT address>,<BT name>,<COD> CR "Inquiry Done" CR
  @usage
*/
void lookForDevices(){
  sendBtCmd("I,5");
  sendBtCmd("---");
}

/*
  @pulls macs out from BT response to inquiry
  @note this is crap it just grabs the first one
  @pre inquiry complete; module is master; BTMsg holds inquiry response
  @post
  @returns first device found
  @usage connectionAdr = getMacFromInquiry();
*/
char* getMacFromInquiry(){
  char[12] mac;
  int i;
  for(i =0; i<12; i++){
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
bool offerConnection(char* mac){
  BTCmd[0] = 'C'; //C,address is command to make connection
  BTCmd[1] = ',';

  for(int i = 2; i < 14; i++){
    BTCmd[i] = mac[i - 2];
  }
  BTCmd[15] = '\0';

  sendBtCmd("$$$", false);
  sendBtCmd(BTCmd);
  sendBtCmd("---");
}

/*
  @sends command to rn41 object
  @commands https://www.sparkfun.com/datasheets/Wireless/Bluetooth/rn-bluetooth-um.pdf
  @pre BtCmd holds ASCII command, if true or empty add newline after command
  @post command sent to bt module
  @usage sendCmd("cmd");
*/
 void sendBtCmd(char* BtCmd, bool need_ln = true){

  if(i_ln){
    Serial.println(BtCmd);
  } else{
    Serial.print(i_pBtCmd);
  }

  delay(100);
  ReceiveBtResponse();
}

/*
  @sets gets response from BT command
  @pre BtMsg is char string with some space
  @post BtMsg has message from bt module in it terminated ny '\0'
  @usage setAsMaster();
*/
void receiveBTResponse(char* BtMsg){
  bool keepReading = true;
  int index = 0;
  char byteIn = '\0';
  BtMsg[0] = '\0';

  while(keepReading){
    keepReading = false;
    if (Serial.available() > 0) {
      byteIn = Serial.read();
      if(byteIn != '\0'){
        BtMsg[index++] = byteIn;
        keepReading = true;
      }
    }
  }
  BtMsg[index] = '\0';
}

/*
 @sends message over BT connection via rn41 object
 @pre
 @post message sent
 @usage sendMsg("Hello World");
*/
void sendMsg(char* msg){
  int i = 0;
  while(msg[i] != '\0'){
    digitalWrite(lightG,LOW);
    delay(50);
    Serial.print(msg[i]);
    i++;
  }
}

/*
  @recieve message from BT connection via rn41 object
  @pre msg is some arrayish thing of char with room for several bytes
  @post message in msg with '\0' at end
  @usage recieveMsg(msg);
*/
void recieveMsg(char* msg){
  int i = 0;
  while(Serial.available() > 0) {
      msg[i] = Serial.read();
      i++;
  }
  msg[i] = '\0';
}

/*
  @check if mac is friendly
  @pre mac is some BT mac address
  @returns true if mac is in Friends array
  @usage if(isFriend(mac)) {do stuff}
*/
bool isFriend(char* mac){
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
bool doneLooking(){
  receiveBTResponse(BTMsg);
  int msgLen = strlen(BTMsg);
  if(msgLen >= 12){
    char* doneMsg = &BTMsg[msgLen - 12];
    if(!strcmp(doneMsg, "Inquiry Done"))
      return true;
  }
  return false;
}

/*
  @fcheck if the module is connected
  @pre BTMsg is not in use
  @post updates connected variable
  @returns true if connected otherwise false
  @usage if(checkConnection()) {do things}
*/
bool checkConnection(){
  int numVal = 0;
  BTMsg = '\0'
  connected = false;
  do{
    delay(1000);
    sendBtCmd("$$$", false);
    sendBtCmd("GK"); //GK is get conncetion status

    receiveBTResponse(BTMsg);

  }while(strlen(BTMsg) > 0)

  numVal= atoi(BTMsg);

  if(numVal == 1){
    connected = true;
  }
  sendBtCmd("---");
  return connected;
}


/*
  @find and connect to given bt MAC
  @pre module is set to be master, mac is mac to be connected to
  @post
  @returns true if successful connection else false
  @usage makeMasterConnection(mac);
*/
bool makeMasterConnection(char* mac){

  lookForDevices();
  BTMsg = '\0';
  int i = 0;
  int maxTries = 10;
  while(!connected){
      delay(100);
      receiveBTResponse(BTMsg);
      int msgLen = strlen(BTMsg);
      if(doneLooking()){

        connectionAdr = getMacFromInquiry();
        if(isFriend(connectionAdr)){
          offerConnection(connectionAdr);
          delay(2000);
          checkConnection();
        }
     }
     i++;
     if(i > maxTries) break;
  }
  return checkConnection();
}

