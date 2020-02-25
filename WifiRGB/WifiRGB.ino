#include <WiFiManager.h>
#include <ESP8266WiFi.h>  
#include <FastLED.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>   



#define LED_PIN     5
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

        




// RGB values as selected
byte r = 128;
byte g = 128;
byte b = 255;

char rgbstring[7] = "8080FF";




ESP8266WebServer server(80);
void handleRoot()
{
  handleRGBselect();
}


void handleRGBselect()
{
  String message = "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
                   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                   "<title>WifiRGB</title></head><body>"

                   "<h2>WifiRGB LED setting</h2>"
                   "by <a href=\"http://www.tomscircuits.blogspot.de/\">Tom's Circuits</a><br>"
                   "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAHgAAACCCAMAAAC6s/yfAAAAMFBMVEX///8JCf//CQkJ/wm9vf//xMTE/8RFR/7/jo6P/4/v4vP/RkZF/0V/f//q/+pQlkGnUwC5AAACkElEQVR4Ae3Z0W7jIBCFYQawgRin7/+2K49arYQ0/qOSaXclzkUuYqlfD+ZcJaz8RlZWVupvwfFfhxe84AUveMELbr8FpwX/1/AjcpJ+Pt4s1yNAtPFR33+uW2B4c5g2V04ehfUUEa6Hg8vHmOB1uFVODoU1D5hKgudu9ytVD5QntSUXkysfA3z+0P2qIyw/NKm4AexUuR4BYJdJ6fcMz9+vZ+KIfj7fPKnWA0Qb9/b+yjvDYU/Pt98vrixG4dlJ9YZw68FhUniMoq/DYVJUWbSwx6SgssBzt0kJnIjXpHb5XuGtTk6qyzc3s81NqnVxcHlSz7SLg8uTaj2Ig0uT0u9lwt0qTGr76NpfXs2DXG6ulY+adFJnCa+knNOuohf+OSnJgZNl3tXnj7h93a9cAqfkaVdzxOPvpApVtv+5euvWaOWj4TGG29cRRxcTv6aDF+f2AkZyDTi0z0nZU7l/HtE14CdNCqYU0TVgmhRNKV45DHdvNzBPim99NNy038F7oj+OO4/ojrCm06QkEwyuAdOkzhIIZneEcVL6PcJXquHuzYC/JrV/FJgSxHDTPsJD5d4kw5Qm3RFW9MJzgSnNuyO8Jz3ukmFK0+4Ih566HitOCdJu3ZaslJOmBEmGC79J6HRgSgCTa8DacGZKCV0DDpKnppTQteBcpqaUrnTDzXp+dYQHzJ4S64Y7LGSAs+CUGGZ3hPVCwZQYBteAdUIwJYDBNWBtylOiG9YMN58mHCTjlDiWK9mGc+EpTbgmrGgufq4NZ3lI9nNtOBR5T+Hz1j3ltXzjlgv1DU6/SQi4fjC4fjC4fvCVcueGNgnzDYN4wOD6weD6weD6wVdOb5iz4AU7ZcELPsNrWVlZWVlZWfkDlvQS5+WAX9gAAAAASUVORK5CYII='>"

                   "<form id=\"pickForm\" action=\"setting\">"
                   "Select a color:<br>"

                   "<input type=\"color\" name=\"favcolor\" value=\"#";
  message += rgbstring;
  message += "\" onchange=\"mysubmit()\"><br>"

             "<br><input type=\"submit\" value=\"Submit\"></form><br><br>"

             "<form id=\"slideForm\" action=\"setting_slider\">"
             "Mix a color:<br>"

             "<table>"
             "<tr><td>Red:</td><td><input type=\"range\" name=\"red_sl\" min=\"0\" max=\"255\" value=\"";
  message += r;
  message += "\" onchange=\"slsubmit()\"></td></tr>";

  message += "<tr><td>Green:</td><td><input type=\"range\" name=\"green_sl\" min=\"0\" max=\"255\" value=\"";
  message += g;
  message += "\" onchange=\"slsubmit()\"></td></tr>";

  message += "<tr><td>Blue:</td><td><input type=\"range\" name=\"blue_sl\" min=\"0\" max=\"255\" value=\"";
  message += b;
  message += "\" onchange=\"slsubmit()\"></td></tr>";

  message += "</table>";
  
  message += "<br><input type=\"submit\" value=\"Submit\"></form>";
  message += "<script>function mysubmit() {document.getElementById(\"pickForm\").submit();}</script>";
  message += "<script>function slsubmit() {document.getElementById(\"slideForm\").submit();}</script>";
  message += "</body></html>";

  server.send(200, "text/html", message);
}


void handleSetting()
{

  long int tmp_int;

  String message = "<!DOCTYPE html><html><head>";
  message += "<meta http-equiv=\"refresh\" content=\"0; URL=/\">";
  message += "<title>WifiRGB</title></head><body>";
  message += "Setting RGB LED.<br>";

  if (server.args() != 1)
  {
    message += "Wrong arguments. Start from root.";
  }
  else
  {
    server.arg(0).substring(1, 7).toCharArray(rgbstring, 7);

    tmp_int = strtol(rgbstring, NULL, 16);
    //    message += tmp_int;
    //    message += "<br>";

    r = tmp_int >> 16;
    g = (tmp_int >> 8) & 0xFF;
    b = tmp_int & 0xFF;

    analogWrite(1, r << 2);   // TXD pin
    analogWrite(0, b << 2);   // GPIO 0
    analogWrite(2, g << 2);   // GPIO 2
  };

  message += "</body></html>";
  server.send(200, "text/html", message);
}


void handleSettingSlider()
{
  char tmp_string[4] = "128";

  String message = "<!DOCTYPE html><html><head>";
  message += "<meta http-equiv=\"refresh\" content=\"0; URL=/\">";
  message += "<title>WifiRGB</title></head><body>";
  message += "Setting RGB LED.<br>";

  if (server.args() != 3)
  {
    message += "Wrong arguments. Start from root.";
  }
  else
  {
    server.arg(0).toCharArray(tmp_string, 4);
    r = strtol(tmp_string, NULL, 10);

    server.arg(1).toCharArray(tmp_string, 4);
    g = strtol(tmp_string, NULL, 10);

    server.arg(2).toCharArray(tmp_string, 4);
    b = strtol(tmp_string, NULL, 10);

    sprintf(rgbstring, "%02x%02x%02x", r, g, b);

    analogWrite(1, r << 2);   // TXD pin
    analogWrite(0, b << 2);   // GPIO 0
    analogWrite(2, g << 2);   // GPIO 2
  };

  message += "</body></html>";
  server.send(200, "text/html", message);
}


void handleReset() {
  WiFiManager wifi;
  String message = "Deleting Wifi credentials...\n\n";
  server.send(200, "text/plain", message);
  delay(500);
  wifi.resetSettings();
  delay(500);
  ESP.reset();
  delay(5000);
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}



void setup()
{
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
  
  
  
  // put your setup code here, to run once:
  Serial.begin(115200);

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
  if (!wifi.autoConnect("WifiRGB"))
  {
    Serial.println("failed to connect and hit timeout");
    delay(5000);
    //reset and try again
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected.");

  // pages

  server.on("/", handleRoot);
  server.on("/rgb", handleRGBselect);
  server.on("/setting", handleSetting);
  server.on("/setting_slider", handleSettingSlider);
  server.on("/reset", handleReset);
  server.onNotFound(handleNotFound);


  server.begin();
  Serial.println("HTTP server started");

  analogWrite(1, 0);    // TXD pin
  analogWrite(0, 0);    // GPIO 0
  analogWrite(2, 0);    // GPIO 2
  delay(100);

  analogWrite(1, 10);
  delay(1000);

  analogWrite(0, 10);
  delay(1000);

  analogWrite(2, 10);
  delay(1000);

  analogWrite(1, 0);
  delay(1000);

  analogWrite(0, 0);
  delay(1000);

  analogWrite(2, 0);
  delay(1000);
}

void loop()
{
  server.handleClient();

    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(100 / UPDATES_PER_SECOND);

  
}





void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};



// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
