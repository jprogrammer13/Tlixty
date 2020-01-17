#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

enum page
{
  HOME,
  MENU,
  WEATHER
};

enum action
{
  RIGHT = 1,
  LEFT = 2,
  SELECT = 3,
  BACK = 4
};

int last_page = 0;

struct Navigation
{
  Encoder *e;
  uint8_t slc;
  uint8_t bck;

  Navigation(Encoder *encoder, uint8_t select, uint8_t back)
  {
    e = encoder;
    slc = select;
    bck = back;
  }

  int read()
  {
    // MAPPA : 1 - RIGHT / 2 - LEFT / 3 - SELECT / 4 - BACK

    int knob = e->read();
    bool btn_slc = !digitalRead(slc);
    bool btn_bck = !digitalRead(bck);

    int result = 0;

    if (knob > 0) 
    {
      result = action(RIGHT);
      delay(100);
    }
    else if (knob < 0) 
    {
      result = action(LEFT);
      delay(100);
    }
    else if (btn_slc)
    {
      result = action(SELECT);
      delay(100);
    }
    else if (btn_bck)
    {
      result = action(BACK);
      delay(100);
    }
    else
    {
      result = 0;
    }

    e->write(0);

    return result;
  }
};

struct Home
{
  char *title = new char[256];
  U8G2 *oled;

  int hour;
  int min;

  Home(char *titolo, U8G2 *ds)
  {
    oled = ds;
    title = titolo;
    hour = 0;
    min = 0;
  }

  void set_time(int m, int h)
  {
    hour = h;
    min = m;
  }

  void render(int navigation)
  {
    oled->firstPage();
    do
    {
      oled->setFont(u8g2_font_ncenB14_tr);

      oled->drawStr(0, 15, (String(hour) + ":" + String(min)).c_str());
      oled->drawCircle(63, 32, hour, U8G2_DRAW_ALL);
    } while (oled->nextPage());

    switch (navigation)
    {
    case action(SELECT):
      last_page = page(MENU);
      break;
    case action(LEFT):
      last_page = page(WEATHER);
      break;
    }
  }
};

struct Menu
{
  U8G2 *oled;

  Menu(U8G2 *ds)
  {
    oled = ds;
  }

  void render(int navigation)
  {
    oled->firstPage();
    do
    {
      oled->setFont(u8g2_font_ncenB14_tr);
      oled->drawStr(0, 15, "MENU");
    } while (oled->nextPage());

    switch (navigation)
    {
    case action(BACK):
      last_page = page(HOME);
      break;
    }
  }
};

struct Weather
{
  U8G2 *oled;

  Weather(U8G2 *ds)
  {
    oled = ds;
  }

  void render(int navigation)
  {
    oled->firstPage();
    do
    {
      oled->setFont(u8g2_font_ncenB14_tr);
      oled->drawStr(0, 15, "WEATHER");
    } while (oled->nextPage());

    switch (navigation)
    {
    case action(BACK):
      last_page = page(HOME);
      break;
    }
  }
};

struct PageSystem
{

  Home *home;
  Menu *menu;
  Weather *weather;

  PageSystem(Home *h, Menu *m, Weather *w)
  {
    last_page = page(HOME);
    home = h;
    menu = m;
    weather = w;
  }

  void render(int navigation, int minute, int hour)
  {
    switch (last_page)
    {
    case HOME:
      home->set_time(minute, hour);
      home->render(navigation);
      break;
    case MENU:
      menu->render(navigation);
      break;
    case WEATHER:
      weather->render(navigation);
      break;
    }
  }
};

#endif
