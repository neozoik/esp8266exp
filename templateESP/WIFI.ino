void WIFIsetup()
{
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifi;


  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifi.setTimeout(300);

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifi.autoConnect("AI"))
  {
    Serial.println("failed to connect and hit timeout");
    delay(5000);
    //reset and try again
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected.");


}

void handleReset() 
{
  WiFiManager wifi;
  String message = "Deleting Wifi credentials...\n\n";
  server.send(200, "text/plain", message);
  delay(500);
  wifi.resetSettings();
  delay(500);
  ESP.reset();
  delay(5000);
}

void handleNotFound() 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) 
   {
     message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
   }
  server.send(404, "text/plain", message);
}
