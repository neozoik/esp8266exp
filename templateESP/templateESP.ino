
 #include <FastLED.h>
 #include <ESP8266WebServer.h>
 #include <WiFiManager.h>   
 #include <ArduinoOTA.h>

 ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  LEDsetup();
  WIFIsetup();
  WEBsetup();
  OTAsetup();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  LEDloop();
}
