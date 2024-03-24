

#include "PAT_OTA.h"
#include "PAT_Bluetooth.h"
int WiFi_initialized = 0;
//______________________________________________________________________________________________________________________________
/* setup function */
void setup(void) {
  Serial.begin(115200);
  delay(1000);
  Bluetooth_init();
  WiFi_initialized = WiFi_init();
}

//______________________________________________________________________________________________________________________________
void loop(void) {
  switch (Bluetooth_Refresh()) {
      //--------------------------
    case en_error:

      break;
      //--------------------------
    case en_responsed:

      break;
      //--------------------------
    case en_nocomment:

      break;
      //--------------------------
    case en_restart_wifi:
      WiFi_initialized = WiFi_init();
      break;
      //--------------------------
    default:
      break;
  }


  if (WiFi_initialized) server.handleClient();
}
//______________________________________________________________________________________________________________________________
