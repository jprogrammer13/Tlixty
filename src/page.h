#ifndef PAGE_H
#define PAGE_H

#include "graphic.h"
#include "img.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Encoder.h>
#include <SPI.h>
#include <U8g2lib.h>

enum page // page list with int identificator
{
  HOME = 0,
  MENU = 1,
  SETTINGS = 2,
  WATCHFACE = 3,
  WEATHER = 4,
  STOPWATCH = 5,
  TIMER = 6,
  MUSIC = 7

};

enum action // action list with int identificator
{
  RIGHT = 1,
  LEFT = 2,
  SELECT = 3,
  BACK = 4
};

enum wf_title // watchface list with int identificator
{
  DIGITAL = 0
};

bool sound = 1; // TO DO : Add this to settings struct

int last_page = 0; // variable to remember the rendering page

struct Boot
{
  U8G2* oled;

  Boot(U8G2* ds) { oled = ds; }

  void render()
  {
    oled->firstPage();
    do {
      oled->setFont(u8g2_font_ncenB14_tr);
      oled->setBitmapMode(1);
      oled->drawXBMP(33, 2, 61, 60, logo);
    } while (oled->nextPage());
  }
};

/**
 *  struct : Navigation
 *  description: This struct read the input device and return action event
 **/

struct Navigation
{
  Encoder* e;
  uint8_t slc;
  uint8_t bck;
  uint8_t bzr;
  int d_time = 120;

  // Encoder Object pointer, Select buttun pin, Back button pin

  Navigation(Encoder* encoder, uint8_t select, uint8_t back, uint8_t buzzer)
  {
    e = encoder;
    slc = select;
    bck = back;
    bzr = buzzer;
  }

  int read()
  {
    // MAPPA : 1 - RIGHT / 2 - LEFT / 3 - SELECT / 4 - BACK

    int knob = e->read();
    bool btn_slc = !digitalRead(slc);
    bool btn_bck = !digitalRead(bck);

    int result = 0;

    if (knob > 0) {
      result = action(RIGHT);
      delay(d_time);
    } else if (knob < 0) {
      result = action(LEFT);
      delay(d_time);
    } else if (btn_slc) {
      result = action(SELECT);
      (sound) ? tone(bzr, 2000, d_time) : delay(d_time);
    } else if (btn_bck) {
      result = action(BACK);
      (sound) ? tone(bzr, 1800, d_time) : delay(d_time);
      ;
    } else {
      result = 0;
    }

    e->write(0);

    return result;
  }
};

/**
 *  struct : Home
 *  description: This struct is the home page, it renders the watchface, manage
 *               input events and redirect other pages rendering
 **/

struct Home
{

  int wtf; // watchface selected

  U8G2* oled;

  int hour;
  int min;
  int t_day;
  int t_w_day;
  int t_month;
  int t_year;

  // U8G2 pointer to manage the display

  Home(U8G2* ds)
  {
    oled = ds;
    wtf = wf_title(DIGITAL);
    hour = 0;
    min = 0;
    t_w_day = 0;
    t_day = 0;
    t_month = 0;
    t_year = 0;
  }

  // function to update time variables

  void set_time(int m, int h, int t_w_d, int t_d, int t_m, int t_y)
  {
    hour = h;
    min = m;
    t_w_day = t_w_d;
    t_day = t_d;
    t_month = t_m;
    t_year = t_y;
  }

  // function to read input events, render the selected watchface and redirect
  // to other pages

  void render(int navigation)
  {
    oled->firstPage();
    do {

      switch (wtf) // read the setted watchface
      {

        case wf_title(DIGITAL):
          oled->setFontMode(1);

          oled->setDrawColor(1);

          dithering(0, 0, 128, 64, 50, 1, oled);
          oled->drawBox(55, 0, 73, 64);
          oled->setFont(u8g2_font_7x14B_tf);

          char* days[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
          char* months[14] = { "",    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                               "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

          oled->drawRBox(3, 45, 35, 14, 7);

          oled->setDrawColor(0);

          oled->setFont(u8g2_font_helvR24_te);
          oled->drawStr(
            60,
            28,
            (((hour < 10) ? ("0" + String(hour)) : String(hour)).c_str()));
          oled->drawStr(
            90, 58, (((min < 10) ? ("0" + String(min)) : String(min)).c_str()));

          oled->setFont(u8g2_font_7x14B_tf);

          oled->drawStr(10, 57, months[t_month]);
          oled->drawStr(105, 13, days[t_w_day]);
          oled->drawStr(
            112,
            26,
            (((t_day < 10) ? ("0" + String(t_day)) : String(t_day)).c_str()));

          oled->setFontDirection(1);
          oled->drawStr(57, 32, String(t_year).c_str());
          oled->setFontDirection(0);

          oled->setDrawColor(1);

          break;
      }

    } while (oled->nextPage());

    // manage the input events - redirect to other pages

    switch (navigation) {
      case action(SELECT):
        last_page = page(MENU);
        break;
      case action(LEFT):
        last_page = page(WEATHER);
        break;
    }
  }
};

/**
 *  struct : Menu
 *  description: This struct implement an app menu
 **/

struct Menu
{
  U8G2* oled;

  char* m_list[6] = { "Settings",  "WatchFace", "Weather",
                      "Stopwatch", "Timer",     "Music" }; // page's title list

  /*    JUST TO UNDERSTAND THE PAGE POSITION (m_position+2)

  enum page_list
  {
    SETTINGS = 2,
    WATCHFACE = 3,
    WEATHER = 4,
    STOPWATCH = 5,
    TIMER = 6,
    MUSIC = 7
  };
  */

  int icon[6] = {
    0X2751, 0X23F2, 0x2603, 0x23f1, 0x23f3, 0x23f8
  }; // page's icon list

  int max_val = sizeof(m_list) / sizeof(m_list[0]) - 1; // max value to show

  int m_position = 0;
  int cordinate[4] = { 16, 37, 57 };

  // U8G2 pointer to manage the display

  Menu(U8G2* ds) { oled = ds; }

  // this function render the menu ui, manage the input event and redirect to
  // other pages

  void render(int navigation)
  {
    oled->firstPage();
    oled->setFont(u8g2_font_unifont_t_symbols);
    do {
      oled->setFontMode(0);

      if (m_position == 0) {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 0, 128, 21);

        for (int i = 0; i < 3; i++) {
          (i == 0) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->drawGlyph(0, cordinate[i], icon[i]);
          oled->setCursor(17, cordinate[i]);
          oled->print(m_list[i]);
        }
      } else if (m_position == max_val) {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 42, 128, 21);

        for (int i = max_val, j = 2; i > max_val - 3, j >= 0; i--, j--) {
          (i == max_val) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->drawGlyph(0, cordinate[j], icon[i]);
          oled->setCursor(17, cordinate[j]);
          oled->print(m_list[i]);
        }
      } else {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 21, 128, 21);

        for (int i = m_position - 1, j = 0; i < m_position + 2, j < 3;
             i++, j++) {
          (i == m_position) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->drawGlyph(0, cordinate[j], icon[i]);
          oled->setCursor(17, cordinate[j]);
          oled->print(m_list[i]);
        }
      }
    } while (oled->nextPage());

    switch (navigation) {
      case action(BACK):
        last_page = page(HOME);
        break;
      case action(RIGHT):
        if (m_position < max_val) {
          m_position++;
        }
        break;
      case action(LEFT):
        if (m_position > 0) {
          m_position--;
        }
        break;
      case action(SELECT):
        last_page = m_position + 2;
    }
  }
};

struct Weather
{
  U8G2* oled;
  HTTPClient* client;

  String city[3] = { "Pescantina,IT", "Trento,IT" };

  String meteo[11] = { "clear sky",     "few clouds",   "scattered clouds",
                       "broken clouds", "shower rain",  "rain",
                       "light rain",    "thunderstorm", "snow",
                       "mist" };

  int city_selector = 0;
  int key_condition = 0;

  int d_icon[10] = { 0x0103, 0x07f,  0x07c, 0x07c,  0x00f1,
                     0x00f1, 0x00f1, 0x07d, 0x009b, 0x00bf };

  int meteo_inf[5] = { 0 /*main_temp*/,
                       0 /*main_humidity*/,
                       0 /*main_temp_min*/,
                       0 /*main_temp_max*/ };

  String payload = "";

  Weather(U8G2* ds, HTTPClient* http)
  {
    oled = ds;
    client = http;
  }

  void get_data()
  {
    key_condition = 0;
    payload = "";

    String meteo_url = "http://api.openweathermap.org/data/2.5/weather?q=" +
                       city[city_selector] +
                       "&appid=6f3d3af7da198e596a118561c69807db&units=metric";

    client->begin(meteo_url);

    int httpCode = client->GET(); // Send the request

    if (payload == "" && httpCode > 0) {

      payload = client->getString(); // Get the request response payload
      client->end();                 // Close connection

      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) +
                              2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) +
                              JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) +
                              JSON_OBJECT_SIZE(13) + 280;
      DynamicJsonDocument doc(capacity);

      deserializeJson(doc, payload);

      JsonObject weather_0 = doc["weather"][0];
      const char* weather_0_description = weather_0["description"];
      JsonObject main = doc["main"];
      meteo_inf[0] = int(main["temp"]);
      meteo_inf[1] = main["humidity"];      // 44
      meteo_inf[2] = int(main["temp_min"]); // 25.56
      meteo_inf[3] = int(main["temp_max"]); // 28.89

      while (key_condition < 9) {
        if (String(weather_0_description) == meteo[key_condition]) {
          break;
        } else {
          key_condition++;
        }
      }
    }
  }

  void render(int navigation)
  {
    oled->firstPage();
    do {

      /*
      oled->setFont(u8g2_font_unifont_t_symbols);
      int m_percent = map(meteo_inf[1], 0, 100, 0, 130);
      oled->drawRBox(-2, 18, m_percent, 4, 1);

      oled->setCursor(0, 10);
      oled->print(city[city_selector]);

      oled->setFont(u8g2_font_helvR24_tf);
      oled->setCursor(55, 60);
      oled->print(String(meteo_inf[0]));
      oled->setFont(u8g2_font_open_iconic_all_4x_t);
      oled->drawGlyph(10, 60, d_icon[key_condition]);
      */

      oled->setFontMode(0);
      oled->setDrawColor(1);
      dithering(85, 0, 43, 64, 50, 1, oled);
      dithering(0, 0, 128, 21, 75, 1, oled);
      oled->drawHLine(0, 21, 128);

      oled->setDrawColor(0);

      oled->drawRBox(3, 3, 85, 16, 8);
      oled->setDrawColor(1);

    } while (oled->nextPage());

    switch (navigation) {
      case action(BACK):
        last_page = page(HOME);
        break;
      case action(SELECT):
        city_selector =
          (city_selector == ((sizeof(city) / sizeof(city[0]) - 1)))
            ? 0
            : city_selector += 1;

        get_data();

        break;
    }
  }
};

struct PageSystem
{

  Home* home;
  Menu* menu;
  Weather* weather;

  PageSystem(Home* h, Menu* m, Weather* w)
  {
    last_page = page(HOME);
    home = h;
    menu = m;
    weather = w;
  }

  void render(int navigation,
              int minute,
              int hour,
              int t_w_day,
              int t_day,
              int t_month,
              int t_year)
  {
    switch (last_page) {
      case HOME:
        home->set_time(minute, hour, t_w_day, t_day, t_month, t_year);
        home->render(navigation);
        break;
      case MENU:
        menu->render(navigation);
        break;
      case WEATHER:
        weather->render(navigation);
        break;
      default:
        home->set_time(minute, hour, t_w_day, t_day, t_month, t_year);
        home->render(navigation);
        Serial.println(page(last_page));
        break;
    }
  }
};

#endif
