#include <Arduino.h>
#include <TlixtyGraphics.h>
#include <U8g2lib.h>
#include <vector>

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, D8, D0);
GIcon bottle;
GIcon bottle2;

void setup()
{
    u8g2.begin();
    Serial.begin(115200);

    // bottle

    bottle = GIcon(&u8g2);
    int bottle_body[] = {30, 59, 46, 55, 53, 61, 61, 59, 67, 53, 67, 46, 73, 40, 65, 32, 59, 38, 51, 38, 30, 59};
    bottle.addGPolygon(GPolygon(&u8g2, (sizeof(bottle_body) / sizeof(int) / 2), bottle_body));
    int bottle_segment[] = {46, 54, 53, 47, 63, 57};
    bottle.addGPolygon(GPolygon(&u8g2, (sizeof(bottle_segment) / sizeof(int) / 2), bottle_segment));
    bottle.addGLine(GLine(&u8g2, GPoint(60, 37), GPoint(68, 45)));
    bottle.addGLine(GLine(&u8g2, GPoint(30, 60), GPoint(27, 60)));
    bottle.addGLine(GLine(&u8g2, GPoint(61, 59), GPoint(73, 56)));

    bottle.scale(0.5);

    bottle2 = bottle;
};

int startTime = millis();
int frame = 0;

void loop()
{
    if (millis() - startTime > 33) // ~ 30 fps
    {
        frame++;
        frame = frame % 30;
        startTime = millis();
    }

    u8g2.firstPage();
    do
    {
        bottle2 = bottle;
        bottle2.scale(1 + (1.0 / 30) * frame);
        bottle2.render();

    } while (u8g2.nextPage());
};