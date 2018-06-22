#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <TimeLib.h>

#define LED_PIN 2
#define NUM_BITS 32

#define TIME_SYNC_INTERVAL 3600

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, LED_PIN, NEO_GRB + NEO_KHZ800);

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

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  timeClient.begin();
  setSyncProvider(getNtpTime);
  setSyncInterval(TIME_SYNC_INTERVAL);
}

void loop() {
  // Serial.println(String(now()));

  for(int i = 0; i < NUM_BITS; i++) {

    if (bitRead(((unsigned long) now()), i)) {
      // INDEX, R, G, B
      strip.setPixelColor(i, 31, 0, 0);
    } else {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }

  strip.show();

  delay(1000);
}
