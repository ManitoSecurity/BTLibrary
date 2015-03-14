/**
  * @file rn41_lib_sand.h
  * @date 14 November 2014
  * @author Brian Gravelle (bgravelle@zagmail.gonzaga.edu)
  * header file for communication to rn-41 bluetooth module from arduino
  * requires use of software serial
  * RX need PWM Pin
*/

#ifndef N41_h
#define N41_h

#include <Arduino.h>
#include "SoftwareSerial.h"
//this might be different depending on where you have things and what os


//adjust to your hardware design
#define TXPIN 4
#define RXPIN 5
#define BAUDRATE 9600

class rn41
{
  public:
    /*
    @construct rn41 object
    */
    rn41();

    /*
    @initialize rn-14 for use
    @pre
    @post rn-14 ready to rock and roll
    @usage
    */
    void btInit();

    /*
    @sets rn-41 object to master mode for controlling communication with other bt devices
    @pre
    @post set as master and rebooted
    @usage setAsMaster();
    */
    void setAsMaster();

    /*
    @sets rn-41 object to slave mode for controlling communication with other bt devices
    @pre
    @post set as slave and rebooted
    @usage setAsSlave();
    */
    void setAsSlave();

    /*
    @reboots rn-41 module- necessary to change settings
    @pre
    @post rebooted module, settings updated
    @usage reboot(); or bt_obj.reboot();
    */
    void reboot();

    /*
      @looks for other BT devices (bt inquiry)
      @pre module is set as master
      @post the module should return up to 9 devices in the form:
            <BT address>,<BT name>,<COD> CR <BT address>,<BT name>,<COD> CR "Inquiry Done" CR
      @usage
    */
    void lookForDevices();

    /*
      @pulls macs out from BT response to inquiry
      @note this is crap it just grabs the first one
      @pre inquiry complete; module is master; BTMsg holds inquiry response
      @post
      @returns
      @usage connectionAdr = getMacFromInquiry(char* mac);
    */
    char* getMacFromInquiry(char* mac);

    /*
      @tries to enslave some bt device
      @pre module is master; mac is some located device
      @post hopefully connected
      @returns true or false based on connection success
      @usage
    */
    bool offerConnection(char* mac);

    /*
      @adds a bt mac to list of friends
      @pre mac is a 12 char long char*
      @post mac is newest friend; numfriends++
      @usage addFriend(mac);
    */
    void addFriend(char* mac);

    /*
    @sends command to rn41 object
    @commands https://www.sparkfun.com/datasheets/Wireless/Bluetooth/rn-bluetooth-um.pdf
    @pre
    @post command sent to bt module
    @usage sendCmd("cmd");
    */
    void sendBtCmd(char* BtCmd, bool need_ln = true);

    /*
    @gets response from rn-41
    @pre command sent to local bt module; expecting response
    @post response in BTMsg
    @usage getReply(BTMsg);
    */
    void getReply(char* BTReply);

    /*
    @cleans serial buffer from rn-41
    @usage purgeReply();
    */
    void purgeReply();

    /*
    @sends message over BT connection via rn41 object
    @pre
    @post message sent
    @usage sendMsg("Hello World");
    */
    void sendMsg(char* BTmsg);

    /*
      @check if mac is friendly
      @pre mac is some BT mac address
      @returns true if mac is in Friends array
      @usage if(isFriend(mac)) {do stuff}
    */
    bool isFriend(char* mac);

    /*
      @check if inquiry is finished
      @pre you did the inquiry thing (lookForDevices)
      @returns true if inquiry has finished
      @usage if(doneLooking()) {do stuff}
    */
    bool doneLooking();

    /*
      @fcheck if the module is connected
      @pre BTMsg is not in use
      @post updates connected variable
      @returns true if connected otherwise false
      @usage if(checkConnection()) {do things}
    */
    bool checkConnection();

    /*
    @find BT device and offer connection if friend
    @pre module is set to be master
    @post
    @returns true if successful connection else false
    @usage makeMasterConnection();
    */
    bool makeMasterConnection();

  private:
    int msgToken;

    SoftwareSerial bluetooth(RXPIN, TXPIN);

    char BTMsg[256];
    char BTCmd[256];
    char BTReply[256];

    static const int MaxNumFriends = 7;
    int numFriends;
    char* Friends[MaxNumFriends];

    char* myAddr ="000666682E57";  //fix this
    bool isMaster;
    bool connected;
    char connectionAdr[13]; //bt address is 12 then \0
};


#endif
