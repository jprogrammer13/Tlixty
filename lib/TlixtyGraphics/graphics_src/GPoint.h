#ifndef __GPOINT_H__
#define __GPOINT_H__

#include <Arduino.h>
#include <U8g2lib.h>


class GPoint
{
private:
    int x;
    int y;

public:
    GPoint();
    GPoint(int _x, int _y);
    int setX(int _x);
    int setY(int _y);
    int getX();
    int getY();
};

#endif