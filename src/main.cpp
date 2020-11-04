#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#endif
#include <ESPAsyncWiFiManager.h>
#include <ArduinoOTA.h>

#include "html_strings.h"

#include "EEPROM.h"
#define FASTLED_ALL_PINS_HARDWARE_SPI
#define ESP8266_SPI
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"

#define buttonPin 0                                           // input pin to use as a digital input
#include "jsbutton.h"                                         // Nice button routine by Jeff Saltzman

// Fixed definitions cannot change on the fly.
#define LED_DT 13                                             // Data pin to connect to the strip.
#define LED_CK 14                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE APA102                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 144                                          // Number of LED's.
#define DEBUG true

// Definition for the array of routines to display.
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void fadeall(void);
void snakey(void);
void colorbands(void);
void bouncingball(void);
void flashusa(void);
void flashfade(void);
void confetti(void);
void blipouts(void);
void rainbow(void);
void rainbowWithGlitter(void);
void addGlitter(fract8 chanceOfGlitter);
void bpm(void);
void white(void);
void cylon(void);
void landinglight(void);
void fireball(void);
void pulse(void);
void racer(void);
void juggle(void);
void bandcracker(void);
void bandcracker2(void);
void readbutton();

MDNSResponder mdns;
DNSServer dnsServer;
AsyncWebServer webServer(80);
AsyncWiFiManager wifiManager(&webServer, &dnsServer);

//Mode and EEPROM variables
uint8_t maxMode = 20;                                          // Maximum number of display modes. Would prefer to get this another way, but whatever.
int eepaddress = 0;

// Global variables can be changed on the fly.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

int16_t movingled = 0;                                       // variables for moving up and down the LED chain
int16_t movingledA = 0;
int16_t movingledB = 0;
int16_t movingledC = 0;
int16_t movingledD = 0;
int16_t movingledE = 0;
int16_t movingledF = 0;

byte mode = 0;

bool bBounce = 0;
bool bWipe = 0;

bool bLed = 0;
bool bLedA = 0;
bool bLedB = 0;
bool bLedC = 0;
bool bLedD = 0;
bool bLedE = 0;
bool policeCar = 0;

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t gCurrentPatternNumber = 0;                            // Index number of which pattern is current
uint8_t gHue = 0;                                             // rotating "base color" used by many of the patterns
uint8_t gSat = 0;                                             // rotating "saturation value"
uint8_t gBrt = 0;                                             // rotating "brightness value"

typedef void (*SimplePatternList[])();                        // List of patterns to cycle through.  Each is defined as a separate function below.
SimplePatternList gPatterns = {fadeall, snakey, colorbands, bouncingball, flashusa, flashfade, confetti, blipouts, rainbow, rainbowWithGlitter, bpm, white, cylon, landinglight, fireball, pulse, racer, juggle, bandcracker, bandcracker2};


void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);                                  // Set button input pin
  digitalWrite(buttonPin, HIGH );

  Serial.println("Booting");
  Serial.println("Free Heap: " + String(ESP.getFreeHeap()));

  Serial.println("Connecting to WiFi.");
  wifiManager.autoConnect();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for APA102
  //LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 5000);               // FastLED Power management set at 5V, 500mA.

  gCurrentPatternNumber = EEPROM.read(eepaddress);

  if (gCurrentPatternNumber > maxMode) gCurrentPatternNumber = 0;   // A safety in case the EEPROM has an illegal value.


  Serial.print("Starting ledMode: ");
  Serial.println(gCurrentPatternNumber);

  gCurrentPatternNumber = 16;


  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.setHostname("FastChair");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("  Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Setup of OTA complete");
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send( 200, "text/html", tab_manual );
    Serial.println("Sent /");
  });
  webServer.on("/power", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P( 200, "text/html", "" );
  });
  webServer.on("/palette", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P( 200, "text/html", "" );
  });
  webServer.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P( 200, "text/html", "" );
  });
  webServer.on("/program", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P( 200, "text/html", "" );
  });
  webServer.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P( 200, "text/html", "" );
  });
  webServer.on("/value", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P( 200, "text/html", "" );
  });
  webServer.begin();
}

void loop() {
  //readbutton();
  ArduinoOTA.handle();                      // Listen for OTA requests

  gPatterns[gCurrentPatternNumber]();                       // Call the current pattern function once, updating the 'leds' array
  FastLED.show();
}


void readbutton() {                                           // Read the button and increase the mode

  uint8_t b = checkButton();

  if (b == 1) {                                               // Just a click event to advance to next pattern
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
    if (gCurrentPatternNumber == 0) {
      gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
    }
    Serial.println(gCurrentPatternNumber);
    bWipe = 1;                                                 // Global mode change boolean
  }

  if (b == 2) {                                               // A double-click event to reset to pattern 0
    gCurrentPatternNumber = 0;
    Serial.println(gCurrentPatternNumber);
  }

  if (b == 3) {                                               // A hold event to write current pattern to EEPROM
    EEPROM.write(eepaddress, gCurrentPatternNumber);
    Serial.print("Writing: ");
    Serial.println(gCurrentPatternNumber);
  }

} // readbutton()




//--------------------[ Effects are below here ]------------------------------------------------------------------------------

void rainbow() {

  fill_rainbow(leds, NUM_LEDS, gHue, 7);                      // FastLED's built-in rainbow generator.
  EVERY_N_MILLISECONDS(20) {                                  // slowly cycle the "base color" through the rainbow
    gHue++;
  }
} // rainbow()



void rainbowWithGlitter() {

  rainbow();                                                  // Built-in FastLED rainbow, plus some random sparkly glitter.
  addGlitter(80);

} // rainbowWithGlitter()



void addGlitter(fract8 chanceOfGlitter) {

  if (random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }

} // addGlitter()



void confetti() {                                             // Random colored speckles that blink in and fade smoothly.

  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
  EVERY_N_MILLISECONDS(20) {                                  // slowly cycle the "base color" through the rainbow
    gHue++;
  }

} // confetti()




/*
void sinelon() {                                              // A colored dot sweeping back and forth, with fading trails.

  fadesparkle();
  //fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,(NUM_LEDS-1));
  leds[pos] += CHSV(gHue, 255, 192);

} // sinelon()
*/



void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
} // fadeall()

void fadeallfast() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(230);
  }
} // fadeallfast()

void fadesparkle(void) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(random8(125, 250));
    leds[i] += leds[i].nscale8(random8(80, 200));
  }
} //fadesparkle

void white() {                                                  //Pick your favorite type of white below, the comments are regarding the use of WS2812 LEDs at full brightness, with no power limiting.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255,168,70);                                 //Shane's favorite.. slightly warm/white
    //leds[i] = CRGB(255,180,80);                               //Still really good, more neutral white
    //leds[i] = CRGB::Tan;                                      //Definitely not "Tan" but better than "White"
    //leds[i] = CRGB::White;                                    //We can call this "BluWhite"
  }
} // white()


void black() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
} // black()


void flashusa() {                                             //USA USA USA USA!
  black();
  uint16_t i = random16(NUM_LEDS);
  leds[i] = CRGB::White;
  if (NUM_LEDS > 100) {
    uint16_t j = random16(NUM_LEDS);
    leds[j] = CRGB::Red;
  }
  if (NUM_LEDS > 200) {
    uint16_t k = random16(NUM_LEDS);
    leds[k] = CRGB::Blue;
  }
}

void flashfade() {
  if (bWipe == 1) {                                           // wipe the display
    black();
    bWipe = 0;
  }
  fadeall();
  uint16_t i = random16(NUM_LEDS);
  leds[i] = CRGB::White;
}

void colorbands() {
  if (bWipe == 1) {
    //movingled = 0;
    black();
    bWipe = 0;
    //bBounce = 0;
  }

  if (bBounce == 0) {
    leds[movingled] = CHSV(gHue, gSat, 255);
    movingled++;
    if (movingled == (NUM_LEDS - 1)) {
      gHue = random8();
      gSat = random8(160, 255);
      bBounce = 1;
    }
  }
  // Now go in the other direction.
  //if ((movingled > 0) && (bBounce == 1)) {
  else {
    leds[movingled] = CHSV(gHue, gSat, 255);
    movingled--;
    if (movingled == 0) {
      gHue = random8();
      gSat = random8(160, 255);
      bBounce = 0;
    }
  }
} // colorbands()


void bouncingball() {

  if (bBounce == 0) {
    fadeall();
    leds[movingled] = CHSV(gHue, 255, 255);
    if (movingled > 0) {
      leds[movingled - 1] = CRGB::Black;
    }
    movingled++;
    if (movingled == (NUM_LEDS - 1)) {
      gHue = random8();
      //gSat = random8(160, 255);
      bBounce = 1;
    }
  }
  // Reverse
  else {
    fadeall();
    leds[movingled] = CHSV(gHue, 255, 255);
    if (movingled < (NUM_LEDS - 1)) {
      leds[movingled + 1] = CRGB::Black;
    }
    movingled--;
    if (movingled == 0) {
      gHue = random8();
      //gSat = random8(160, 255);
      bBounce = 0;
    }
  }
} // bouncingball()


void snakey() {
  if (bWipe == 1) {                                             // wipe the display at pattern startup
    black();
    bWipe = 0;
  }

  if (bBounce == 0) {
    leds[movingled] = CHSV(gHue++, 255, 255);
    movingled++;
    if (movingled == (NUM_LEDS - 1)) {
      bBounce = 1;
    }
  }
  // Reverse
  else {
    leds[movingled] = CHSV(gHue++, 255, 255);
    movingled--;
    if (movingled == 0) {
      bBounce = 0;
    }
  }
  fadeall();
} // snakey()


void bpm() {                                                  // Colored stripes pulsing at a defined Beats-Per-Minute.

  uint8_t BeatsPerMinute = 60;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  EVERY_N_MILLISECONDS(20) {                                  // slowly cycle the "base color" through the rainbow
    gHue++;
  }
  for (int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }

} // bpm()


void blipouts() {
  uint8_t blipWidth = NUM_LEDS/30;                              // larger number = smaller width blips

  if (bWipe == 1) {                                             // wipe the display
    black();
    bWipe = 0;
  }

  fadeall();
  if (bBounce == 1) {
    EVERY_N_MILLISECONDS(50) {                                  // we always wait for a blip to finish, but if done early, this is the minimum time before next blip
      bBounce = 0;
      gBrt = 255;
      gSat++;                                                   // borrowing the unused (in this effect) gSat variable for rotation of color wheel
      gHue = (gSat + random8(40));                              // randomize the displayed hue by 40
      movingled = random16(NUM_LEDS);
      movingledB = movingled;
      movingledC = movingled;
      leds[movingled] = CHSV(gHue, 255, gBrt);
    }
  }
  if (gBrt > (255/blipWidth)) {
    gBrt -= (255/blipWidth);
  }
  if (movingledB < (NUM_LEDS - 2)) {
    if (movingledB < (movingled + blipWidth)) {
      movingledB++;
      leds[movingledB] = CHSV(gHue, 255, gBrt);
    } else {
      bBounce = 1;
    }
  }
  if (movingledC > 0) {
    if (movingledC > (movingled - blipWidth)) {
      movingledC--;
      leds[movingledC] = CHSV(gHue, 255, gBrt);
    } else {
      bBounce = 1;
    }
  }
}


void fireball() {

  uint8_t i = 30;                      //chance of trailing sparkles 0-255
  int8_t j = 1;

  fadesparkle();

  if (bBounce == 0) {
    leds[movingled] = CRGB::Orange;
    //leds[movingled] = CHSV(gHue++, 255, 255);
    if (movingled > 0) {
      leds[movingled - 1].nscale8(80);
    }
    j+random8(5);
    if ((random8() < i) && ((movingled - j) >= 0)) {
      movingledB = (movingled - j);
      leds[ movingledB ] = CRGB::Tan;
    }
    movingled++;
    if (movingled == (NUM_LEDS-1)) {
      bBounce = 1;
    }
  }
  // Now go in the other direction.
  else {
    leds[movingled] = CRGB::Orange;
    //leds[movingled] = CHSV(gHue++, 255, 255);
    if (movingled < (NUM_LEDS - 1)) {
      leds[movingled + 1].nscale8(80);
    }
    j+random8(5);
    if ((random8() < i) && ((movingled + j) < NUM_LEDS)) {
      movingledB = (movingled + j);
      leds[ movingledB ] = CRGB::Tan;
    }
    movingled--;
    if (movingled == 0) {
      bBounce = 0;
    }
  }
} // fireball()



void juggle() {                                               // Eight colored dots, weaving in and out of sync with each other.

  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;

  for(int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,(NUM_LEDS-1))] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
} // juggle()



void cylon() {
  uint8_t eyeSize = NUM_LEDS / 5;
  uint8_t tempBrt;
  gHue = 0;
  gSat = 255;
  gBrt = 255;
  tempBrt = gBrt;

  if ( bBounce == 0 ) {
    if((NUM_LEDS/50) > 1)
      movingled += (NUM_LEDS/50);
    else
      movingled++;
    if ( movingled >= (NUM_LEDS - 1) ) {
      bBounce = 1;
      movingled = (NUM_LEDS - 1);
    }
  } else {
    if((NUM_LEDS/50) > 1 )
      movingled -= (NUM_LEDS/50);
    else
      movingled--;
    if ( movingled <= 0 ) {
      bBounce = 0;
      movingled = 0;
    }
  }

  black();
  leds[movingled] = CHSV(gHue, gSat, gBrt);

  for( int i = movingled+1; i < (movingled+1)+(eyeSize/2); i++ ) {
    if( i < NUM_LEDS ) {
      tempBrt -= (gBrt/(eyeSize/2));
      leds[i] = CHSV(gHue, gSat, tempBrt);
    }
  }

  tempBrt = gBrt;
  for( int i = movingled-1; i > (movingled-1)-(eyeSize/2); i-- ) {
    if( i >= 0 ) {
      tempBrt -= (gBrt/(eyeSize/2));
      leds[i] = CHSV(gHue, gSat, tempBrt);
    }
  }
} // cylon



void landinglight() {
  uint8_t numStrobes = 30;                                      // how many strobes do you want?
  uint8_t spacing = (NUM_LEDS/numStrobes);
  if (spacing < 1) {
    spacing = 1;
  }
  if (bWipe == 1) {                                             // wipe the display at pattern startup
    black();
    bWipe = 0;
  }

  if (bBounce == 1) {
    black();
    EVERY_N_MILLISECONDS(500) {
      bBounce = 0;
      movingled = 0;
    }
  }

  if (bBounce == 0) {
    black();
    if (movingled < (NUM_LEDS - 1)) {
      leds[movingled] = CRGB::White;
      if ((movingled + spacing) < (NUM_LEDS - 1)) {
        movingled += spacing;
      } else {
        bBounce = 1;
      }
    }
  }
} // landinglight


void pulse() {

  if (bWipe == 1) {                                             // wipe the display
    black();
    bWipe = 0;
  }

  uint8_t BPM = 60;
  uint8_t beat = beatsin8(BPM, 0, 255);
  EVERY_N_MILLISECONDS(100) {
    gHue++;
  }
  if(DEBUG) { Serial.println(beat); }
  for (uint16_t i = 0; i <= ((NUM_LEDS)/2); i++) {
    leds[i] = CHSV(gHue, 255, ((i*beat)/NUM_LEDS)*2);
    leds[(NUM_LEDS-1) - i] = CHSV(gHue, 255, ((i*beat)/NUM_LEDS)*2);
  }
} // pulse()


void racer(void) {

  black();
  uint8_t redBarLength = NUM_LEDS / 8;
  uint8_t blueBarLength = NUM_LEDS / 22;
  uint8_t whiteBarLength = NUM_LEDS / 15;
  uint8_t carSize = NUM_LEDS/30;

  if (bWipe == 1) {                                             // wipe the display and initialize
    black();
    movingled = 0;
    movingledA = 0;
    movingledB = 0;
    movingledC = 0;
    movingledD = 0;
    movingledE = 0;
    movingledF = 0;
    bLed = 0;
    bLedA = 0;
    bLedB = 0;
    bLedC = 0;
    bLedD = 0;
    bLedE = 0;
    bWipe = 0;
    bBounce = 0;
  }


  if (bLedC == 0 && bLedD == 0 && bLedE == 0) {                                     // Start red sequence immediately if none are running          DEBUG FIX!
    bLedC = 1;
  }

  if (bLed == 0) {                                                                  // Start white movingled randomly
    EVERY_N_MILLISECONDS(100) {
      if (random8(1, 100) <= 1) {
        bLed = 1;
      }
    }
  }

  if ((bLedA == 0 && movingledB > NUM_LEDS/4) || (bLedA == 0 && bLedB == 0)) {      // Start blue movingledA
    EVERY_N_MILLISECONDS(20) {
      if (random8(1, 100) <= 1) {
        bLedA = 1;
      }
    }
  }

  if (bLedB == 0 && movingledA > NUM_LEDS/4) {                                      // Start blue movingledB
    EVERY_N_MILLISECONDS(20) {
      if (random8(1, 100) <= 1) {
        bLedB = 1;
      }
    }
  }

  if (bLedC == 0 && movingledE > NUM_LEDS/4) {                                      // Start red movingledC
    EVERY_N_MILLISECONDS(20) {
      if (random8(1, 100) <= 1) {
        bLedC = 1;
      }
    }
  }

    if (bLedD == 0 && movingledC > NUM_LEDS/4) {                                      // Start red movingledD
    EVERY_N_MILLISECONDS(20) {
      if (random8(1, 100) <= 1) {
        bLedD = 1;
      }
    }
  }

    if (bLedE == 0 && movingledD > NUM_LEDS/4) {                                      // Start red movingledE
    EVERY_N_MILLISECONDS(20) {
      if (random8(1, 100) <= 1) {
        bLedE = 1;
      }
    }
  }

  if (bLedC == 1) {                                                                   //sequence movingledC here
    for (int i = 0; i <= redBarLength; i++) {
      if ((movingledC-i) >= 0) {
        if(movingledC-i < NUM_LEDS) {
          leds[movingledC-i] = CRGB::Red;
        }
      }
    }
    if ((movingledC - redBarLength) >= NUM_LEDS) {                                       // Reset
      bLedC = 0;
      movingledC = 0;
    }
    EVERY_N_MILLISECONDS(30) {
      movingledC++;
    }
  }

  if (bLedD == 1) {                                                                   //sequence movingledD here
    for (int i = 0; i <= redBarLength; i++) {
      if ((movingledD-i) >= 0) {
        if(movingledD-i < NUM_LEDS) {
          leds[movingledD-i] = CRGB::Red;
        }
      }
    }
    if ((movingledD - redBarLength) >= NUM_LEDS) {                                       // Reset
      bLedD = 0;
      movingledD = 0;
    }
    EVERY_N_MILLISECONDS(30) {
      movingledD++;
    }
  }

  if (bLedE == 1) {                                                                   //sequence movingledE here
    for (int i = 0; i <= redBarLength; i++) {
      if ((movingledE-i) >= 0) {
        if(movingledE-i < NUM_LEDS) {
          leds[movingledE-i] = CRGB::Red;
        }
      }
    }
    if ((movingledE - redBarLength) >= NUM_LEDS) {                                       // Reset
      bLedE = 0;
      movingledE = 0;
    }
    EVERY_N_MILLISECONDS(30) {
      movingledE++;
    }
  }

  if (bLedA == 1) {                                                                   //sequence movingledA here
    for (int i = 0; i <= blueBarLength; i++) {
      if ((movingledA-i) >= 0) {
        if(movingledA-i < NUM_LEDS) {
          leds[movingledA-i] = CRGB::Blue;
        }
      }
    }
    if ((movingledA - blueBarLength) >= NUM_LEDS) {                                       // Reset
      bLedA = 0;
      movingledA = 0;
    }
    EVERY_N_MILLISECONDS(10) {
      movingledA++;
    }
  }

  if (bLedB == 1) {                                                                   //sequence movingledB here
    for (int i = 0; i <= blueBarLength; i++) {
      if ((movingledB-i) >= 0) {
        if(movingledB-i < NUM_LEDS) {
          leds[movingledB-i] = CRGB::Blue;
        }
      }
    }
    if ((movingledB - blueBarLength) >= NUM_LEDS) {                                       // Reset
      bLedB = 0;
      movingledB = 0;
    }
    EVERY_N_MILLISECONDS(10) {
      movingledB++;
    }
  }

  if (bLed == 1) {                                                                   //sequence movingled here
    for (int i = 0; i <= whiteBarLength; i++) {
      if ((movingled-i) >= 0) {
        if(movingled-i < NUM_LEDS) {
          leds[movingled-i] = CRGB(255,180,80);
        }
      }
    }

    if (policeCar == true) {
      for (int i = 0; i <= carSize; i++) {
        if ((movingledF-i) >= 0) {
          if(movingledF-i < NUM_LEDS) {
            if (bBounce == 0) {
              if (i < (carSize/3)) {
                leds[movingledF-i] = CRGB::Blue;
              } else {
                leds[movingledF-i] = CHSV(34,255,100);
              }
            } else {
               if (i > ((carSize/3) * 2)) {
                leds[movingledF-i] = CRGB::Red;
              } else {
                leds[movingledF-i] = CHSV(34,255,100);
              }
            }
          }
        }
        if (i == carSize) {
          EVERY_N_MILLISECONDS(100) {
            bBounce = !bBounce;
          }
        }
      }
    }

    if ((movingledF - carSize) >= NUM_LEDS) {                             // reset white and police car
      bLed = 0;
      movingled = 0;
      movingledF = 0;
      if (random8(1) < 1) {                                               //randomly turn police car on and off
        policeCar = !policeCar;
      }
    }
    movingled += (NUM_LEDS/60);
    movingledF = (((movingled) / 2) - (whiteBarLength + (NUM_LEDS / 10))) ;
  }

} //racer


void bandcracker() {

  if (bWipe == 1) {                                             // wipe the display
    black();
    bWipe = 0;
    gHue = 0;
    movingledA = (NUM_LEDS/2);
    movingledB = (NUM_LEDS/2);
    bBounce = 0;
  }

  if (bBounce == 1) {
    gHue += (random8(16, 48));
    movingledA = (NUM_LEDS/2);
    movingledB = (NUM_LEDS/2);
    leds[movingledA] = CHSV(gHue, 255, 255);
    bBounce = 0;
  }

  if (movingledA < (NUM_LEDS - 2)) {
    movingledA++;
    leds[movingledA] = CHSV(gHue, 255, 255);
  } else {
    bBounce = 1;
  }
  if (movingledB > 0) {
    movingledB--;
    leds[movingledB] = CHSV(gHue, 255, 255);
  } else {
    bBounce = 1;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(random8(125, 250));
    leds[i] += leds[i].nscale8(random8(105, 244));
  }
}


void bandcracker2() {

  if (bWipe == 1) {                                             // wipe the display
    black();
    bWipe = 0;
    gHue = 0;
    movingledA = 0;
  }

  uint16_t mark = movingledA;

  if (movingledA == 0 || movingledA == (NUM_LEDS-1)) {
    gHue += (random8(24, 64));
  }

  movingledA = beatsin16(30, 0, NUM_LEDS - 1, NUM_LEDS/2);
  if ((movingledA - mark ) > 1) {
    for (int j = mark; j < movingledA; j++) {
      leds[j] = CHSV(gHue, 255, 255);
    }
  } else {
    leds[movingledA] = CHSV(gHue, 255, 255);
  }
  if ((mark - movingledA) > 1) {
    for (int j = movingledA; j < mark; j++) {
      leds[j] = CHSV(gHue, 255, 255);
    }
  } else {
    leds[movingledA] = CHSV(gHue, 255, 255);
  }


  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(random8(125, 250));
    leds[i] += leds[i].nscale8(random8(105, 244));
  }
}
