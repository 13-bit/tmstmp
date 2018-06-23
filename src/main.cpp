#include <Arduino.h>
#include <FastLED.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <TimeLib.h>

#define DATA_PIN 2
#define NUM_BITS 32

#define TIME_SYNC_INTERVAL 3600

CRGB leds[NUM_BITS];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

time_t getNtpTime() {
  timeClient.update();

  return (time_t) timeClient.getEpochTime();
}

void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.autoConnect("13bit UNIX");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_BITS);
  
  timeClient.begin();
  setSyncProvider(getNtpTime);
  setSyncInterval(TIME_SYNC_INTERVAL);
}

void loop() {
  // Serial.println(String(now()));

  for(int i = 0; i < NUM_BITS; i++) {

    if (bitRead(((unsigned long) now()), i)) {
      leds[i] = CRGB::OrangeRed;
      leds[i].fadeLightBy(224);
    } else {
      leds[i] = CRGB::Black;
    }
  }

  FastLED.show();

  delay(1000);
}
