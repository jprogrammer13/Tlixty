#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Encoder.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <time.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include "page.h"

#define slc D3
#define bck D4

//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ D5, /* data=*/ D7, /* CS=*/ D2, /*RS=*/ D0);
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, D2, D0);

const char *ssid = "Dimesse11";
const char *password = "benvenutoDimesse001";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.it.pool.ntp.org", 3600 /*OFFSET*/);

Encoder myEnc(D1, D2);


Navigation navigation = Navigation(&myEnc, slc, bck); //navigator declaration

Home home = Home("Demo", &u8g2);  // Home declaration
Menu menu = Menu(&u8g2);
Weather weather = Weather(&u8g2);

PageSystem page_render = PageSystem(&home, &menu, &weather);   // Page Render System declaration



int h = 0;
int m = 0;


void setup()
{
  u8g2.begin();

  u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0, 15, "<<<< BOOT >>>");
    } while (u8g2.nextPage());

  
  Serial.begin(115200);

  pinMode(slc, INPUT_PULLUP);
  pinMode(bck, INPUT_PULLUP);

  WiFi.begin(ssid, password);

  unsigned long start_c = millis();
  unsigned long soglia = 300000; // soglia di controllo per passare il AP (default 25s)

  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - start_c < soglia)
    {
      delay(500);
    }
    else
    {
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    timeClient.begin();
    timeClient.update();
    h = timeClient.getHours();
    m = timeClient.getMinutes();
  }
}

unsigned long time_start = millis();


void loop()
{
  if ((millis() - time_start) >= 10000)
  {
    timeClient.update();
    h = timeClient.getHours();
    m = timeClient.getMinutes();
    time_start = millis();
  }

  int nav_event = navigation.read(); // navigation event reading

  page_render.render(nav_event, m, h); // render page system with navigation event
}