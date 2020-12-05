#include "GPoint.h"

GPoint::GPoint()
{
    x = 0;
    y = 0;
}

GPoint::GPoint(int _x, int _y)
{
    x = _x;
    y = _y;
}
int GPoint::setX(int _x)
{
    x = _x;
}

int GPoint::setY(int _y)
{
    y = _y;
}

int GPoint::getX()
{
    return x;
}

int GPoint::getY()
{
    return y;
}