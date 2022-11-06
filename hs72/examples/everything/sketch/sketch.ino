// sketch.ino

#include <Arduino.h>
#include <freertos/task.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiManager.h> // force inclusion to fix pio LDF WebServer.h issue
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include "private.h"
#include "unphone.h"
#include <Adafruit_EPD.h>

unPhone u = unPhone();

String apSSID = String("everthing-");    // SSID of the AP
String apPassword = _DEFAULT_AP_KEY;     // passkey for the AP
char BUILD_TIME[] = (__DATE__ " at " __TIME__); // build data

bool useWifi = true;
WiFiMulti wifiMulti;
HTTPClient http;
int firmwareVersion = 1; // keep up-to-date! (used to check for updates)
static uint32_t loopIter = 0;   // time slicing iterator
void wifiSetup();               // TODO move to unPhone?
void wifiConnectTask(void *);   // TODO move to unPhone?
void initWebServer();           // TODO move to unPhone?

void setup() { ///////////////////////////////////////////////////////////////
  // say hi, init, blink etc.
  Serial.begin(115200);
  Serial.printf("Starting build from: %s\n", BUILD_TIME);
  u.begin();
  u.store(BUILD_TIME);
  apSSID.concat(u.getMAC()); // add the MAC to the AP SSID

  // if all three buttons pressed, go into factory test mode
  // if(u.button1() && u.button2() && u.button3()) {
  //   u.factoryTestMode(false);
  //   u.factoryTestSetup();
  //   return;
  // }

  // power management
  u.printWakeupReason(); // what woke us up?
  u.checkPowerSwitch();  // if power switch is off, shutdown
  Serial.printf("battery voltage = %3.3f\n", u.batteryVoltage());
  Serial.printf("enabling expander power\n");
  u.expanderPower(true); // turn expander power on

  // flash the internal RGB LED and then the IR_LEDs
  u.ir(true);
  u.rgb(0, 1, 1); delay(300); u.rgb(1, 0, 1); delay(300);
  u.expanderPower(false); // turn expander power off
  u.rgb(1, 1, 0); delay(300); u.rgb(0, 1, 1); delay(300);
  u.expanderPower(true); // turn expander power on
  u.ir(false);
  u.rgb(1, 0, 1); delay(300); u.rgb(1, 1, 0); delay(300);
  for(uint8_t i = 0; i<4; i++) {
    u.ir(true);  delay(300);
    u.expanderPower(false); // turn expander power on
    u.ir(false); delay(300);
    u.expanderPower(true); // turn expander power on
  }

  // Fixed Rotation. Fucks up his code but we probably shouldnt care
  // Nevermind printing text is fucked up otherwise
  u.rgb(0, 0, 0);
  u.rgb(1, 1, 1);

  // buzz a bit -- screw this
  // for(int i = 0; i < 3; i++) {
  //   u.vibe(true);  delay(150);
  //   u.vibe(false); delay(150);
  // }

  u.printStore(); // print out stored messages

  // get a connection
  if(useWifi) {
    // run the wifi connection task
    Serial.println("trying to connect to wifi...");
    wifiSetup();
    xTaskCreate(wifiConnectTask, "wifi connect task", 4096, NULL, 1, NULL);
  }
  u.provisioned();

  Serial.println("done with setup()");
}

void loop() { ////////////////////////////////////////////////////////////////
  if(u.factoryTestMode()) { u.factoryTestLoop(); return; }

  // send a couple of TTN messages for testing purposes
  if(loopIter++ == 0)
    u.loraSend("first time: UNPHONE_SPIN=%d MAC=%s", UNPHONE_SPIN, u.getMAC());
  else if(loopIter == 20000000)
    u.loraSend("20000000: UNPHONE_SPIN=%d MAC=%s", UNPHONE_SPIN, u.getMAC());

  if(loopIter % 25000 == 0) // allow IDLE; 100 is min to allow it to fire:
    delay(100);  // https://github.com/espressif/arduino-esp32/issues/6946
}

void wifiSetup() { ///////////////////////////////////////////////////////////
// TODO move these to a credentials store, manage with WifiMgr
#ifdef _MULTI_SSID1
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID1);
  wifiMulti.addAP(_MULTI_SSID1, _MULTI_KEY1);
#endif
#ifdef _MULTI_SSID2
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID2);
  wifiMulti.addAP(_MULTI_SSID2, _MULTI_KEY2);
#endif
#ifdef _MULTI_SSID3
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID3);
  wifiMulti.addAP(_MULTI_SSID3, _MULTI_KEY3);
#endif
#ifdef _MULTI_SSID4
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID4);
  wifiMulti.addAP(_MULTI_SSID4, _MULTI_KEY4);
#endif
#ifdef _MULTI_SSID5
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID5);
  wifiMulti.addAP(_MULTI_SSID5, _MULTI_KEY5);
#endif
#ifdef _MULTI_SSID6
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID6);
  wifiMulti.addAP(_MULTI_SSID6, _MULTI_KEY6);
#endif
#ifdef _MULTI_SSID7
  Serial.printf("wifiMulti.addAP %s\n", _MULTI_SSID7);
  wifiMulti.addAP(_MULTI_SSID7, _MULTI_KEY7);
#endif
#ifdef _MULTI_SSID8
  Serial.printf("wifiMulti.addAP 8\n");
  wifiMulti.addAP(_MULTI_SSID8, _MULTI_KEY8);
#endif
}

static bool wifiConnected = false; ///////////////////////////////////////////
void wifiConnectTask(void *param) {
  while(true) {
    bool previousWifiState = wifiConnected;
    if(wifiMulti.run() == WL_CONNECTED)
      wifiConnected = true;
    else
      wifiConnected = false;

    // call back to UI controller if state has changed
    if(previousWifiState != wifiConnected) {
      previousWifiState = wifiConnected;
      u.provisioned();
    }

    delay(1000);
  }
}
