#ifndef PAGE_H
#define PAGE_H

#include "graphic.h"
#include "img.h"
#include "icon.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Encoder.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <TimeLib.h>
#include <data.h>

// https://heydays.no/project/pebble

enum page // page list with int identificatorobs
{
  TIMELINE = -1,
  HOME = 0,
  MENU = 1,
  SETTINGS = 2,
  WATCHFACE = 3,
  ALARM = 4,
  WEATHER = 5,
  STOPWATCH = 6,
  TIMER = 7,
  MUSIC = 8,
  ALERT = 9,
  NOTIFY = 10

};

enum action // action list with int identificator
{
  BACK = 1,
  LEFT = 2,
  SELECT = 3,
  RIGHT = 4,
};

enum wf_title // watchface list with int identificator
{
  DIGITAL = 0
};

bool sound = 1; // TO DO : Add this to settings struct

int last_page = 0; // variable to remember the rendering page
bool alarm_off = 0;


struct Boot
{
  U8G2 *oled;

  Boot(U8G2 *ds) { oled = ds; }

  void render()
  {
    oled->firstPage();
    do
    {
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
  uint8_t bck;
  uint8_t lx;
  uint8_t slc;
  uint8_t rx;
  uint8_t led;
  int d_time = 100;

  // Encoder Object pointer, Select buttun pin, Back button pin

  Navigation(uint8_t back, uint8_t left, uint8_t select, uint8_t right, uint8_t led_pin)
  {
    bck = back;
    lx = left;
    slc = select;
    rx = right;
    led = led_pin;
  }

  int read()
  {

    bool btn_back = digitalRead(bck);
    bool btn_lx = digitalRead(lx);
    bool btn_slc = digitalRead(slc);
    bool btn_rx = digitalRead(rx);
    bool btn[] = {digitalRead(bck), digitalRead(lx), digitalRead(slc), digitalRead(rx)};

    int result = 0;

    for (int i = 0; i < 4; i++)
    {
      if (btn[i])
      {
        result = i + 1;
        delay(d_time);
        i = 4;
      }
    }

    return result;
  }
};

/**
 *  struct : Timeline
 *  description: 
 **/

struct Timeline
{

  U8G2 *oled;
  int index = 0;

  Timeline(U8G2 *ds)
  {
    oled = ds;

  }

  void render(int navigation, int minute, int hour)
  {
    oled->firstPage();
    do
    {
      oled->drawBox(118, 0, 10, 64);
      oled->drawTriangle(118, 15, 118, 25, 113, 20);
    } while (oled->nextPage());

    switch (navigation)
    {
    case action(BACK):
      last_page = page(HOME);
      break;
    }
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

  U8G2 *oled;

  int hour;
  int min;
  int t_day;
  int t_w_day;
  int t_month;
  int t_year;

  // U8G2 pointer to manage the display

  Home(U8G2 *ds)
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
    do
    {

      switch (wtf) // read the setted watchface
      {

      case wf_title(DIGITAL):
        oled->setFontMode(1);

        oled->setDrawColor(1);

        dithering(0, 0, 128, 64, 50, 1, oled);
        oled->drawBox(55, 0, 73, 64);
        oled->setFont(u8g2_font_7x14B_tf);

        char *days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        char *months[14] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

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

    switch (navigation)
    {
    case action(SELECT):
      last_page = page(MENU);
      break;
    case action(LEFT):
      last_page = page(WEATHER);
      break;
    case action(RIGHT):
      last_page = page(TIMELINE);
      break;
    }
  }
};

/**
 *  struct : Settings
 *  description: This struct implement a settings menu
 **/

struct Settings
{
  U8G2 *oled;

  char *s_list[7] = {"CIAO2", "CIAO2", "CIAO3", "CIAO4",
                     "CIAO5", "CIAO6", "CIAO7"}; // page's title list

  /*    JUST TO UNDERSTAND THE PAGE POSITION (m_position+2)

  enum page_list
  {
    SETTINGS = 2,
    WATCHFACE = 3,
    ALARM = 4,
    WEATHER = 5,
    STOPWATCH = 6,
    TIMER = 7,
    MUSIC = 8
  };
  */

  int icon[7] = {
      0X2751, 0X23F2, 0X23F0, 0x2603, 0x23F1, 0x23F3, 0x23F8}; // page's icon list

  int max_val = sizeof(s_list) / sizeof(s_list[0]) - 1; // max value to show

  int s_position = 0;
  int cordinate[4] = {16, 37, 57};

  // U8G2 pointer to manage the display

  Settings(U8G2 *ds) { oled = ds; }

  // this function render the menu ui, manage the input event and redirect to
  // other pages

  void render(int navigation)
  {
    oled->firstPage();
    //oled->setFont(u8g2_font_unifont_t_symbols);
    oled->setFont(u8g2_font_7x14_tf);
    do
    {
      oled->setFontMode(0);

      if (s_position == 0)
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 0, 128, 21);

        for (int i = 0; i < 3; i++)
        {
          (i == 0) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->setFont(u8g2_font_unifont_t_symbols);
          oled->drawGlyph(3, cordinate[i], icon[i]);
          oled->setFont(u8g2_font_7x14_tf);
          oled->setCursor(23, cordinate[i]);
          oled->print(s_list[i]);
        }
      }
      else if (s_position == max_val)
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 42, 128, 21);

        for (int i = max_val, j = 2; i > max_val - 3, j >= 0; i--, j--)
        {
          (i == max_val) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->setFont(u8g2_font_unifont_t_symbols);
          oled->drawGlyph(3, cordinate[j], icon[i]);
          oled->setFont(u8g2_font_7x14_tf);
          oled->setCursor(23, cordinate[j]);
          oled->print(s_list[i]);
        }
      }
      else
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 21, 128, 21);

        for (int i = s_position - 1, j = 0; i < s_position + 2, j < 3;
             i++, j++)
        {
          (i == s_position) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->setFont(u8g2_font_unifont_t_symbols);
          oled->drawGlyph(3, cordinate[j], icon[i]);
          oled->setFont(u8g2_font_7x14_tf);
          oled->setCursor(23, cordinate[j]);
          oled->print(s_list[i]);
        }
      }
    } while (oled->nextPage());

    switch (navigation)
    {
    case action(BACK):
      last_page = page(MENU);
      break;
    case action(RIGHT):
      if (s_position < max_val)
      {
        s_position++;
      }
      break;
    case action(LEFT):
      if (s_position > 0)
      {
        s_position--;
      }
      break;
    case action(SELECT):
      last_page = s_position + 2;
    }
  }
};

/**
 *  struct : Menu
 *  description: This struct implement an app menu
 **/

struct Menu
{
  U8G2 *oled;

  char *m_list[7] = {"Settings", "WatchFace", "Alarm", "Weather",
                     "Stopwatch", "Timer", "Music"}; // page's title list

  /*    JUST TO UNDERSTAND THE PAGE POSITION (m_position+2)

  enum page_list
  {
    SETTINGS = 2,
    WATCHFACE = 3,
    ALARM = 4,
    WEATHER = 5,
    STOPWATCH = 6,
    TIMER = 7,
    MUSIC = 8
  };
  */

  int icon[7] = {
      0X2751, 0X23F2, 0X23F0, 0x2603, 0x23F1, 0x23F3, 0x23F8}; // page's icon list

  int max_val = sizeof(m_list) / sizeof(m_list[0]) - 1; // max value to show

  int m_position = 0;
  int cordinate[4] = {16, 37, 57};

  // U8G2 pointer to manage the display

  Menu(U8G2 *ds) { oled = ds; }

  // this function render the menu ui, manage the input event and redirect to
  // other pages

  void render(int navigation)
  {
    oled->firstPage();
    //oled->setFont(u8g2_font_unifont_t_symbols);
    oled->setFont(u8g2_font_7x14_tf);
    do
    {
      oled->setFontMode(0);

      if (m_position == 0)
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 0, 128, 21);

        for (int i = 0; i < 3; i++)
        {
          (i == 0) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->setFont(u8g2_font_unifont_t_symbols);
          oled->drawGlyph(3, cordinate[i], icon[i]);
          oled->setFont(u8g2_font_7x14_tf);
          oled->setCursor(23, cordinate[i]);
          oled->print(m_list[i]);
        }
      }
      else if (m_position == max_val)
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 42, 128, 21);

        for (int i = max_val, j = 2; i > max_val - 3, j >= 0; i--, j--)
        {
          (i == max_val) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->setFont(u8g2_font_unifont_t_symbols);
          oled->drawGlyph(3, cordinate[j], icon[i]);
          oled->setFont(u8g2_font_7x14_tf);
          oled->setCursor(23, cordinate[j]);
          oled->print(m_list[i]);
        }
      }
      else
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 21, 128, 21);

        for (int i = m_position - 1, j = 0; i < m_position + 2, j < 3;
             i++, j++)
        {
          (i == m_position) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          oled->setFont(u8g2_font_unifont_t_symbols);
          oled->drawGlyph(3, cordinate[j], icon[i]);
          oled->setFont(u8g2_font_7x14_tf);
          oled->setCursor(23, cordinate[j]);
          oled->print(m_list[i]);
        }
      }
    } while (oled->nextPage());

    switch (navigation)
    {
    case action(BACK):
      last_page = page(HOME);
      break;
    case action(RIGHT):
      if (m_position < max_val)
      {
        m_position++;
      }
      break;
    case action(LEFT):
      if (m_position > 0)
      {
        m_position--;
      }
      break;
    case action(SELECT):
      last_page = m_position + 2;
    }
  }
};

/**
 *  struct : Notification
 *  description: 
 **/

struct Notification
{
  U8G2 *oled;
  uint8 bzr;
  int y_position;
  unsigned long time_start;
  int titolo_size;
  int text_size;
  int animation_x;
  bool animation;
  String id;
  String title;
  String text;
  String linee[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

  Notification(U8G2 *ds, uint8 bz)
  {
    oled = ds;
    y_position = 0;
    time_start = 0;
    bzr = bz;
    titolo_size = 0;
    text_size = 0;
    animation_x = 128;
    animation = true;
  }

  void split_text(int size)
  {
    int counter = 0;
    String buffer = "";      //to store word
    String text_buffer = ""; //to store multiple words

    for (int i = 0; i <= text.length(); i++)
    {
      char carattere = text.charAt(i);

      if (carattere != ' ' & i != text.length())
      {
        buffer += String(carattere);
      }
      else
      {
        if (i == text.length() && counter < 10 && buffer.length() < size)
        {
          if ((text_buffer.length() + buffer.length()) < size)
          {
            if (text_buffer != "")
            {
              text_buffer += " ";
            }
            text_buffer += buffer;
            linee[counter] = text_buffer;
          }
          else
          {
            linee[counter] = text_buffer;
            counter++;
            linee[counter] = buffer;
          }
        }
        else if (buffer.length() > size && counter < 10)
        {
          int division_size = (buffer.length() / size) + (buffer.length() % size > 0);
          for (int i = 0; i < division_size; i++)
          {
            int end = (i <= (division_size - 2)) ? (size * (i + 1)) : buffer.length();
            linee[counter] = buffer.substring(size * i, end);
            counter++;
          }
          buffer = "";
        }
        else if ((text_buffer.length() + buffer.length()) < size && counter < 10)
        {
          if (text_buffer != "")
          {
            text_buffer += " ";
          }
          text_buffer += buffer;
          buffer = "";
        }
        else
        {
          if (counter < 15)
          {
            linee[counter] = text_buffer;
            counter++;
            text_buffer = buffer;
            buffer = "";
          }
          else
          {
            linee[15] = linee[15].substring(0, 5) + "...";
            i = text.length() + 1;
          }
        }
      }
    }
  }

  void cleanText()
  {
    for (int i = 0; i < 16; i++)
    {
      linee[i] = "";
    }
  }

  void app_icon(String id, int icon_selector)
  {
    if (id == "com.whatsapp")
    {
      (icon_selector == 0) ? (oled->drawXBM(42, 12, icon_big_width, icon_big_height, whatsapp_big)) : (oled->drawXBM(2, 25, icon_width, icon_height, whatsapp));
    }
    else
    {
      (icon_selector == 0) ? (oled->drawXBM(42, 12, icon_big_width, icon_big_height, gen_notification_big)) : (oled->drawXBM(2, 25, icon_width, icon_height, gen_notification));
    }
  }

  void notify(unsigned long time, String n_id, String n_title, String n_text)
  {
    time_start = time;
    id = n_id;
    title = n_title;
    text = title + "             " + n_text;
    titolo_size = int(title.length() / 13) + (title.length() % 13 != 0);
    y_position = 0;
    animation_x = 128;
    animation = true;
    cleanText();
    split_text(13);
    text_size = 0;
    for (int i = 0; i < 16; i++)
    {
      if (linee[i] != "")
      {
        text_size++;
      }
    }
    if (sound)
      tone(bzr, 4978, 75);
    last_page = page(NOTIFY);
  }

  void render(int navigation)
  {
    if (millis() - time_start < 20000)
    {
      oled->firstPage();
      do
      {
        if (millis() - time_start < 1200 && animation)
        {
          oled->drawBox(0, 0, animation_x, 64);
          oled->setDrawColor(0);
          app_icon(id, 0);
          oled->setDrawColor(1);
          if (animation_x > 18 && millis() - time_start > 700)
          {
            animation_x--;
          }
        }
        else
        {
          animation = false;
          oled->setDrawColor(1);
          oled->setFont(u8g2_font_7x14_tf);
          oled->drawBox(0, 0, 18, 64);
          oled->setDrawColor(0);
          app_icon(id, 1);
          oled->setDrawColor(1);
          oled->setFont(u8g2_font_7x14B_tf);

          int position = 15;

          for (int i = 0; i < 16; i++)
          {
            if (titolo_size == i)
            {
              position += 5;
              oled->setFont(u8g2_font_7x14_tf);
            }
            oled->drawUTF8(25, y_position + position, linee[i].c_str());
            position += 15;
          }
          //oled->setFont(u8g2_font_unifont_t_symbols);
          oled->setFont(u8g2_font_7x14_tf);
        }
      } while (oled->nextPage());

      switch (navigation)
      {
      case action(BACK):
        y_position = 0;
        last_page = page(HOME);
        break;
      case action(SELECT):
        y_position = 0;
        last_page = page(HOME);
        break;
      case action(RIGHT):
        if (!animation)
        {
          time_start = millis();
        }
        if (y_position > -(text_size * 1.5) * 10)
        {
          y_position -= 10;
        }
        break;
      case action(LEFT):
        if (!animation)
        {
          time_start = millis();
        }
        if (y_position != 0)
        {
          y_position += 10;
          break;
        }
      }
    }
    else
    {
      y_position = 0;
      last_page = page(HOME);
    }
  }
};

struct Alarm
{

  int wtf; // watchface selected

  U8G2 *oled;
  uint8 bzr;

  int hour[3] = {8, 9, 15};
  int min[3] = {15, 15, 30};
  String t_w_day[3] = {"12345", "0123456", "12345"};

  int max_val = sizeof(hour) / sizeof(hour[0]) - 1; // max value to show

  int m_position = 0;
  int cordinate[4] = {16, 37, 57};

  // U8G2 pointer to manage the display

  Alarm(U8G2 *ds, uint8 bz)
  {
    oled = ds;
    bzr = bz;
  }

  void alert(int navigation)
  {

    if (!alarm_off)
    {
      randomSeed(analogRead(D0));
      unsigned int random_tone = random(1000, 20000);
      tone(bzr, random_tone, 500);
    }

    oled->firstPage();
    do
    {
      //oled->setFont(u8g2_font_unifont_t_symbols);
      oled->setFont(u8g2_font_7x14_tf);

      oled->drawGlyph(15, 25, 0X23F0);
      oled->setCursor(40, 25);
      oled->print("SVEGLIAA!");
      oled->setCursor(5, 45);
      oled->print("E`giunta l'ora!");

    } while (oled->nextPage());

    // manage the input events - redirect to other pages

    switch (navigation)
    {
    case action(SELECT):
      last_page = page(HOME);
      alarm_off = 1;
      break;
    }
  }

  void alarm_check(int m, int h, int t_w_d, int navigation)
  {
    bool is_time = 0;
    bool is_day = 0;

    for (int i = 0; i < sizeof(hour) / sizeof(hour[0]); i++)
    {
      //Serial.println(String(hour[i]) + ":" + String(min[i]));

      if (hour[i] == h)
      {
        if (min[i] == m)
        {
          is_time = 1;

          is_day = 0;
          for (int j = 0; j < t_w_day[i].length(); j++)
          {
            if (t_w_day[i].charAt(j) == char(48 + t_w_d))
            {

              is_day = 1;
              j = 5;
            }

            if (is_time && is_day)
            {
              if (!alarm_off)
              {
                last_page = page(ALERT);
              }
            }
          }
        }
      }
    }
  }

  // function to read input events, render the selected watchface and redirect
  // to other pages

  void render(int navigation)
  {
    oled->firstPage();
    do
    {
      oled->setFontMode(0);
      String sveglia = "";

      if (m_position == 0)
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 0, 128, 21);

        for (int i = 0; i < 3; i++)
        {
          sveglia = String(hour[i]) + ":" + String(min[i]);
          (i == 0) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          for (int j = 0; j < t_w_day[i].length() - 1; j++)
          {
            oled->drawDisc(50 + (13 * j), cordinate[i] - 5, 4, U8G2_DRAW_ALL);
          }
          //oled->drawGlyph(0, cordinate[i], icon[i]);
          oled->setCursor(2, cordinate[i]);
          oled->print(sveglia);
        }
      }
      else if (m_position == max_val)
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 42, 128, 21);

        for (int i = max_val, j = 2; i > max_val - 3, j >= 0; i--, j--)
        {
          sveglia = String(hour[i]) + ":" + String(min[i]);
          (i == max_val) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          for (int j = 0; j < t_w_day[i].length() - 1; j++)
          {
            oled->drawDisc(50 + (13 * j), cordinate[i] - 5, 4, U8G2_DRAW_ALL);
          }
          //oled->drawGlyph(0, cordinate[j], icon[i]);
          oled->setCursor(2, cordinate[j]);
          oled->print(sveglia);
        }
      }
      else
      {
        oled->setDrawColor(1); /* color 1 for the box */
        oled->drawBox(0, 21, 128, 21);

        for (int i = m_position - 1, j = 0; i < m_position + 2, j < 3;
             i++, j++)
        {
          sveglia = String(hour[i]) + ":" + String(min[i]);
          (i == m_position) ? oled->setDrawColor(0) : oled->setDrawColor(1);
          for (int j = 0; j < t_w_day[i].length() - 1; j++)
          {
            oled->drawDisc(50 + (13 * j), cordinate[i] - 5, 4, U8G2_DRAW_ALL);
          }
          //oled->drawGlyph(0, cordinate[j], icon[i]);
          oled->setCursor(2, cordinate[j]);
          oled->print(sveglia);
        }
      }

    } while (oled->nextPage());

    // manage the input events - redirect to other pages

    switch (navigation)
    {
    case action(RIGHT):
      if (m_position < max_val)
      {
        m_position++;
      }
      break;
    case action(LEFT):
      if (m_position > 0)
      {
        m_position--;
      }
      break;
    case action(BACK):
      last_page = page(MENU);
      break;
    }
  }
};

struct Weather
{
  U8G2 *oled;
  HTTPClient *client;

  String city[2] = {"Pescantina,IT", "Trento,IT"};

  String meteo[11] = {"clear sky", "few clouds", "scattered clouds",
                      "broken clouds", "shower rain", "rain",
                      "light rain", "thunderstorm", "snow",
                      "mist"};

  int city_selector = 0;
  int key_condition = 0;

  int d_icon[10] = {0x0103, 0x07f, 0x07c, 0x07c, 0x00f1,
                    0x00f1, 0x00f1, 0x07d, 0x009b, 0x00bf};

  int meteo_inf[5] = {0 /*main_temp*/,
                      0 /*main_humidity*/,
                      0 /*main_temp_min*/,
                      0 /*main_temp_max*/};

  String payload = "";
  String description = "";

  Weather(U8G2 *ds, HTTPClient *http)
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

    if (payload == "" && httpCode > 0)
    {

      payload = client->getString(); // Get the request response payload
      client->end();                 // Close connection

      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) +
                              2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) +
                              JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) +
                              JSON_OBJECT_SIZE(13) + 280;
      DynamicJsonDocument doc(capacity);

      deserializeJson(doc, payload);

      JsonObject weather_0 = doc["weather"][0];
      const char *weather_0_description = weather_0["description"];
      description = String(weather_0_description);
      JsonObject main = doc["main"];
      meteo_inf[0] = int(main["temp"]);
      meteo_inf[1] = main["humidity"];      // 44
      meteo_inf[2] = int(main["temp_min"]); // 25.56
      meteo_inf[3] = int(main["temp_max"]); // 28.89

      JsonObject sys = doc["sys"];
      long sys_sunrise = sys["sunrise"]; // 1600664556
      long sys_sunset = sys["sunset"];   // 1600708588


      events_name[sunrise_h] = "Sunrise";
      events_time[sunrise_h] = String(hour(time_t(sys_sunrise)))+":"+String(minute(time_t(sys_sunrise)));


      events_name[sunset_h] = "Sunset";
      events_time[sunset_h] = String(hour(time_t(sys_sunset)))+":"+String(minute(time_t(sys_sunset)));

      while (key_condition < 9)
      {
        if (String(weather_0_description) == meteo[key_condition])
        {
          break;
        }
        else
        {
          key_condition++;
        }
      }
    }
  }

  void render(int navigation)
  {
    oled->firstPage();
    do
    {

      //oled->setFont(u8g2_font_unifont_t_symbols);
      oled->setFont(u8g2_font_7x14_tf);

      oled->setCursor(0, 13);
      oled->print(city[city_selector]);
      oled->drawHLine(0, 16, 128);
      oled->setFont(u8g2_font_helvR24_tf);
      oled->setCursor(0, 45);
      oled->print(String(meteo_inf[0]));
      oled->setFont(u8g2_font_6x10_tf);
      oled->setCursor(0, 55);
      oled->print("LW:" + String(meteo_inf[2]));
      oled->setCursor(40, 55);
      oled->print("HI:" + String(meteo_inf[3]));
      oled->setCursor(0, 64);
      oled->print(description);

      oled->setFont(u8g2_font_open_iconic_all_4x_t);
      oled->drawGlyph(90, 60, d_icon[key_condition]);

    } while (oled->nextPage());

    switch (navigation)
    {
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

  Timeline *timeline;
  Home *home;
  Menu *menu;
  Settings *settings;
  Weather *weather;
  Alarm *alarm;
  Notification *notification;

  PageSystem(Timeline *t, Home *h, Menu *m, Settings *s, Alarm *a, Weather *w, Notification *n)
  {
    last_page = page(HOME);
    timeline = t;
    home = h;
    menu = m;
    settings = s;
    alarm = a;
    weather = w;
    notification = n;
  }

  void render(int navigation,
              int minute,
              int hour,
              int t_w_day,
              int t_day,
              int t_month,
              int t_year)
  {
    alarm->alarm_check(minute, hour, t_w_day, navigation);

    switch (last_page)
    {
    case TIMELINE:
      timeline->render(navigation, minute, hour);
      break;
    case HOME:
      home->set_time(minute, hour, t_w_day, t_day, t_month, t_year);
      home->render(navigation);
      break;
    case MENU:
      menu->render(navigation);
      break;
    case SETTINGS:
      settings->render(navigation);
      break;
    case ALARM:
      alarm->render(navigation);
      break;
    case ALERT:
      alarm->alert(navigation);
      break;
    case WEATHER:
      weather->render(navigation);
      break;
    case NOTIFY:
      notification->render(navigation);
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
