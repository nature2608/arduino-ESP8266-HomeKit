#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_



#include <strings_en.h>
#include <WiFiManager.h>
#include "FS.h"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>


void wifi_connect() {
    WiFiManager wifiManager;
   wifiManager.setConfigPortalTimeout(240);
   if(!wifiManager.autoConnect("HomeKit", ""))
  {
    Serial.println(F("Failed to connect. Reset and try again. . ."));
    delay(3000);
    //重置并重试
    ESP.reset();
    delay(5000);
  }
  else 
  WiFi.mode(WIFI_STA);
}

#endif /* WIFI_INFO_H_ */
