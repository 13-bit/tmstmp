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
#define COLOR_CHANGE_INTERVAL 20

CRGB leds[NUM_BITS];

CRGB colors[4] = {CRGB::OrangeRed, CRGB::LimeGreen, CRGB::SkyBlue, CRGB::Amethyst};
int colorIndex = -1;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

time_t nextColorChangeTime = 0;

time_t getNtpTime() {
  timeClient.update();

  return (time_t) timeClient.getEpochTime();
}

CRGB updateColor() {
  if (now() >= nextColorChangeTime) {
    nextColorChangeTime += COLOR_CHANGE_INTERVAL;

    colorIndex++;

    if (colorIndex > 3) {
      colorIndex = 0;
    }
  }

  return colors[colorIndex];
}

void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.autoConnect("13bit UNIX");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_BITS).setCorrection(TypicalSMD5050);

  timeClient.begin();
  setSyncProvider(getNtpTime);
  setSyncInterval(TIME_SYNC_INTERVAL);

  nextColorChangeTime = now();
}

void loop() {
  CRGB color = updateColor();

  for(int i = 0; i < NUM_BITS; i++) {

    if (bitRead(((unsigned long) now()), i)) {
      leds[i] = color;
      leds[i].fadeLightBy(224);
    } else {
      leds[i] = CRGB::Black;
    }
  }

  FastLED.show();

  delay(1000);
}

void adjustColor() {

}
