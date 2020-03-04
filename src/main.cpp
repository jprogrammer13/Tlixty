#include "page.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Encoder.h>
#include <NTPClient.h>
#include <SPI.h>
#include <TimeLib.h>
#include <U8g2lib.h>
#include <WiFiUdp.h>
#include <time.h>

// SIMULATOR FOR U8G2
// https://p3dt.net/u8g2sim/

#define slc D3
#define bck D4
#define bzr D8

// U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ D5, /* data=*/ D7, /*
// CS=*/ D2, /*RS=*/ D0);
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, D2, D0);

const char* ssid = "TIM-19861131";
const char* password = "BussolaGay";

WiFiUDP ntpUDP;
HTTPClient http_client;
NTPClient timeClient(ntpUDP, "0.it.pool.ntp.org", 3600 /*OFFSET*/);

Encoder myEnc(D1, D2);

Navigation navigation = Navigation(&myEnc, slc, bck,bzr); // navigator declaration

Boot bootanimation = Boot(&u8g2);
Home home = Home(&u8g2); // Home declaration
Menu menu = Menu(&u8g2);
Weather weather = Weather(&u8g2, &http_client);

PageSystem page_render =
  PageSystem(&home, &menu, &weather); // Page Render System declaration

int h = 0;
int m = 0;
int t_day = 0;
int t_w_day = 0;
int t_month = 0;
int t_year = 0;
int epoch = 0;

void
setup()
{
  u8g2.begin();

  bootanimation.render(); // bootanimation

  Serial.begin(115200);

  pinMode(slc, INPUT_PULLUP);
  pinMode(bck, INPUT_PULLUP);
  pinMode(bzr, OUTPUT);

  WiFi.begin(ssid, password);

  unsigned long start_c = millis();
  unsigned long soglia =
    300000; // soglia di controllo per passare il AP (default 25s)

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start_c < soglia) {
      delay(500);
    } else {
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    timeClient.begin();
    timeClient.update();
    h = timeClient.getHours();
    m = timeClient.getMinutes();
    t_w_day = timeClient.getDay();
    epoch = timeClient.getEpochTime();
    t_day = day(epoch);
    t_month = month(epoch);
    t_year = year(epoch);
    weather.get_data();
  }
}

unsigned long time_start = millis();

void
loop()
{
  if ((millis() - time_start) >= 10000) {
    timeClient.update();
    h = timeClient.getHours();
    m = timeClient.getMinutes();
    t_w_day = timeClient.getDay();
    epoch = timeClient.getEpochTime();
    t_day = day(epoch);
    t_month = month(epoch);
    t_year = year(epoch);
    time_start = millis();
    weather.get_data();
  }

  int nav_event = navigation.read(); // navigation event reading

  page_render.render(nav_event,
                     m,
                     h,
                     t_w_day,
                     t_day,
                     t_month,
                     t_year); // render page system with navigation event
}