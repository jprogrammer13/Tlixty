#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

void dithering(int sx, int sy, int w, int h, int percent, int size, U8G2 *oled)
{
    switch (percent)
    {
    // 75% (DARK GREY)
    case (75):

        for (int y = sy; y < sy + h; y++)
        {
            for (int x = sx; x < sx + w; x++)
            {
                //display.fillRect(x, y, size, size, (y % 2 == 0) ? ((x % 2 == 0) ? GxEPD_WHITE : GxEPD_BLACK) : GxEPD_BLACK);
                if (y % 2 == 0)
                {
                    if (x % 2 != 0)
                    {
                        oled->drawBox(x, y, size, size);
                    }
                }
                else
                {
                    oled->drawBox(x, y, size, size);
                }
            }
        }
        break;

    // 50% (MIDDLE GREY)
    case (50):

        for (int y = sy; y < sy + h; y++)
        {
            for (int x = sx; x < sx + w; x++)
            {
                //display.fillRect(x, y, size, size, (y % 2 == 0) ? ((x % 2 == 0) ? GxEPD_WHITE : GxEPD_BLACK) : ((x % 2 == 0) ? GxEPD_BLACK : GxEPD_WHITE));
                if (y % 2 == 0)
                {
                    if (x % 2 != 0)
                    {
                        oled->drawBox(x, y, size, size);
                    }
                }
                else if (x % 2 == 0)
                {
                    oled->drawBox(x, y, size, size);
                }
            }
        }
        break;

        // 25% (LIGHT GREY)
    case (25):

        for (int y = sy; y < sy + h; y++)
        {
            for (int x = sx; x < sx + w; x++)
            {
                //display.fillRect(x, y, size, size, (y % 2 == 0) ? GxEPD_WHITE : ((x % 2 == 0) ? GxEPD_BLACK : GxEPD_WHITE));

                if (y % 2 != 0)
                {
                    if (x % 2 == 0)
                    {
                        oled->drawBox(x, y, size, size);
                    }
                }
            }
        }
        break;
    }
}