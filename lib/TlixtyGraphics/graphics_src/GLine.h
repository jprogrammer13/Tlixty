#ifndef __GLINE_H__
#define __GLINE_H__

#include <Arduino.h>
#include <U8g2lib.h>
#include "GPoint.h"

class GLine
{
private:
    U8G2 *display;
    GPoint p1;
    GPoint p2;

public:
    GLine();
    GLine(U8G2 *_display, GPoint _p1, GPoint _p2);
    void printPoints();
    void scale(float size);
    void render();
};


#endif