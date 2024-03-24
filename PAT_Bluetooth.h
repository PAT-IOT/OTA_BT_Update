
#ifndef __PAT_Bluetooth__H
#define __PAT_Bluetooth__H

#include "BluetoothSerial.h"

#include "PAT_OTA.h"
////////////////////////////////////// Configuration ////////////////////////////////////////////////
String device_name = "IOT Controller";
//////////////////////////////////// End Configuration //////////////////////////////////////////////
struct wlanStructure {
  String ssid;
  String password;
};
wlanStructure wlan[2];
String str;    // = String(Serial.read());
String strBT;  // = String(SerialBT.read());

enum BTenum { en_error,
              en_responsed,
              en_nocomment,
              en_restart_wifi,
};

BluetoothSerial SerialBT;
int iwconfig = 0;
//___________________________________________________________________________________________________________________________________________________________________________________________________________________

void Bluetooth_init(void) {
  strBT.reserve(60);
  SerialBT.begin(device_name);  //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
}

//___________________________________________________________________________________________________________________________________________________________________________________________________________________
int Bluetooth_Refresh(void) {
  /*if (Serial.available()) {
      SerialBT.write(Serial.read());
    }*/
  //--------------------------------------------------------
  if (SerialBT.available()) {
    strBT = "";
    do {
      char c = SerialBT.read();
      strBT += c;
    } while (SerialBT.available());
    strBT.replace("\n", "");
    Serial.println(strBT);
    //--------------------------------------------------------
    if (!iwconfig) {
      if (strBT.substring(0, 5) == "wlan0") iwconfig = 1;
      else if (strBT.substring(0, 8) == "ifconfig") {
        SerialBT.println("wlan0 ???");
      } else if (strBT.substring(0, 13) == "restart wlan0") {
        SerialBT.println("wlan0 is resetting");
        return en_restart_wifi;
      } else SerialBT.println("Error: I could not find your command");
    }
    //--------------------------------------------------------
    if (iwconfig) {
      switch (iwconfig) {
          //--------------------------
        case 1:
          SerialBT.print("SSID:  ");
          break;
          //--------------------------
        case 2:
          wlan[0].ssid = strBT.substring(0, strBT.length() - 2);
          SerialBT.print("Pass Word:  ");
          break;
          //--------------------------
        case 3:
          wlan[0].password = strBT.substring(0, strBT.length() - 2);
          SerialBT.println("created wlan0 ->\nSSID: " + String(wlan[0].ssid) + "\nPassWord: " + String(wlan[0].password));
          break;
          //--------------------------
        default:
          break;
      }
      iwconfig++;
      if (iwconfig > 4) iwconfig = 0;
    }
    return en_responsed;
  } else return en_nocomment;
  return en_nocomment;
}
//___________________________________________________________________________________________________________________________________________________________________________________________________________________
#endif  //__PAT_Bluetooth__H