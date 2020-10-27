#include "page.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Encoder.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <SPI.h>
#include <TimeLib.h>
#include <U8g2lib.h>
#include <WiFiUdp.h>
#include <time.h>
#include <AsyncDelay.h>
#include <ESP8266WebServer.h>

ESP8266WiFiClass wifi;

// FPS MANAGER

AsyncDelay fps_manager;
const int fps = 1000 / 30; //30FPS

// SERVER

ESP8266WebServer server(1518);
void new_notification();

// SIMULATOR FOR U8G2
// https://p3dt.net/u8g2sim/

#define slc D3
#define bck D4
#define bzr D6

// U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ D5, /* data=*/ D7, /*
// CS=*/ D2, /*RS=*/ D0);
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, D8, D0);

const char *ssid = "TIM-19861131";
const char *password = "BussolaGay";

WiFiUDP ntpUDP;
HTTPClient http_client;
NTPClient timeClient(ntpUDP, "0.it.pool.ntp.org", 3600 /*OFFSET*/);
Encoder myEnc(D1, D2);
Navigation navigation = Navigation(&myEnc, slc, bck, bzr); // navigator declaration

Boot bootanimation = Boot(&u8g2);
Timeline timeline = Timeline(&u8g2);
Home home = Home(&u8g2);
Menu menu = Menu(&u8g2);
Settings settings = Settings(&u8g2, &wifi);
Alarm alarm = Alarm(&u8g2, bzr);
Weather weather = Weather(&u8g2, &http_client);
Notification notification = Notification(&u8g2, bzr);

PageSystem page_render =
    PageSystem(&timeline, &home, &menu, &settings, &alarm, &weather, &notification); // Page Render System declaration

int h = 0;
int m = 0;
int t_day = 0;
int t_w_day = 0;
int t_month = 0;
int t_year = 0;
int epoch = 0;

void setup()
{
  clean_events();
  u8g2.begin();
  fps_manager.start(fps, AsyncDelay::MILLIS);
  bootanimation.render(); // bootanimation

  Serial.begin(115200);

  pinMode(slc, INPUT_PULLUP);
  pinMode(bck, INPUT_PULLUP);
  pinMode(bzr, OUTPUT);

  wifi.mode(WIFI_STA);
  wifi.begin(ssid, password);

  const unsigned long start_c = millis();
  const unsigned long soglia = 300000; // soglia di controllo per passare il AP (default 25s)

  while (wifi.status() != WL_CONNECTED)
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

  if (wifi.status() == WL_CONNECTED)
  {
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

    // WEBSERVER
    Serial.println(wifi.macAddress());
    Serial.println(wifi.hostname().c_str());
    Serial.println(wifi.localIP().toString().c_str());

    server.on("/notification", new_notification);
    server.begin();
  }
}

unsigned long time_start = millis();
unsigned long time_allert = millis();

void loop()
{
  server.handleClient();

  if ((millis() - time_start) >= 10000)
  {
    timeClient.update();
    h = timeClient.getHours();
    m = timeClient.getMinutes();
    t_w_day = timeClient.getDay();
    epoch = timeClient.getEpochTime();
    t_day = day(epoch);
    t_month = month(epoch);
    t_year = year(epoch);
    time_start = millis();
  }

  if ((millis() - time_allert) >= 120000)
  {
    time_allert = millis();
    alarm_off = 0;
    weather.get_data();
  }

  if (fps_manager.isExpired())
  {
    int nav_event = navigation.read(); // navigation event reading

    page_render.render(nav_event,
                       m,
                       h,
                       t_w_day,
                       t_day,
                       t_month,
                       t_year); // render page system with navigation event
    fps_manager.repeat();
  }
}

void new_notification()
{
  server.send(200, "text/plain", "Notifica ricevuta!");
  Serial.println(server.arg("notification"));

  const size_t capacity = JSON_OBJECT_SIZE(3) + 768;
  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, server.arg("notification"));
  if (last_page != NOTIFY)
  {
    notification.multiple_notification = false;
    notification.notify(millis(), doc["id"], doc["title"], doc["text"]);
  }
  else
  {
    notification.multiple_notification = true;
  }
}
