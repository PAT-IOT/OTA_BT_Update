#ifndef __PAT_OTA__H
#define __PAT_OTA__H

#include "PAT_Bluetooth.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
////////////////////////////////////// Configuration ////////////////////////////////////////////////
extern wlanStructure wlan[2];
extern BluetoothSerial SerialBT;
char* host = "Novaday";
char* ssid = "PAT_IOT";
char* password = "PAT_IOT123";

//////////////////////////////////// End Configuration //////////////////////////////////////////////




WebServer server(80);
//____________________________________________________________________________________________________________________________________________________
String style =
  "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
  "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
  "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
  "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
  "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
  ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

//____________________________________________________________________________________________________________________________________________________
String loginIndex =
  "<form name=loginForm>"
  "<h1>IOT Controller</h1>"
  "<h5>(SN=ed4578798675as4465)</h5>"
  "<input name=userid placeholder='User ID'> "
  "<input name=pwd placeholder=Password type=Password> "
  "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
  "<script>"
  "function check(form) {"
  "if(form.userid.value=='admin' && form.pwd.value=='admin')"
  "{window.open('/serverIndex')}"
  "else"
  "{alert('Error Password or Username')}"
  "}"
  "</script>"
  + style;
//____________________________________________________________________________________________________________________________________________________
String serverIndex =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
  "<label id='file-input' for='file'>   Choose file...</label>"
  "<input type='submit' class=btn value='Update'>"
  "<br><br>"
  "<div id='prg'></div>"
  "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
  "<script>"
  "function sub(obj){"
  "var fileName = obj.value.split('\\\\');"
  "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
  "};"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  "$.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "$('#bar').css('width',Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!') "
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>"
  + style;
//__________________________________________________________________________________________________________________________________________________
int WiFi_init(void) {
  SerialBT.println("The wifi is being reset ...");
  WiFi.begin(wlan[0].ssid, wlan[0].password);
  // Wait for connection
  int wifidisconnected = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    SerialBT.println("WiFi not Connected");
    wifidisconnected++;
    if (wifidisconnected > 10) return 0;
  }
  SerialBT.println("\nConnected to " + wlan[0].ssid + "\nIP address:  ");
  SerialBT.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) {  //http://esp32.local
    SerialBT.println("Error setting up MDNS responder!");
    return 0;
  }
  SerialBT.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  //------------------------------------------------------------------------
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  //------------------------------------------------------------------------
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  //------------------------------------------------------------------------
  server.on(
    "/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        SerialBT.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  //start with max available size
          Update.printError(SerialBT);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(SerialBT);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {  //true to set the size to the current progress
          SerialBT.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(SerialBT);
        }
      }
    });
  //------------------------------------------------------------------------
  server.begin();
  return 1;
}
//___________________________________________________________________________________________________________________________________________________________________________________________________________________
#endif  //__PAT_OTA__H
