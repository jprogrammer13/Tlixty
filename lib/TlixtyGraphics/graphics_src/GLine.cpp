#include "GLine.h"

GLine::GLine()
{
    display = NULL;
    p1 = GPoint();
    p2 = GPoint();
}

GLine::GLine(U8G2 *_display, GPoint _p1, GPoint _p2)
{
    display = _display;
    p1 = _p1;
    p2 = _p2;
}

void GLine::scale(float size)
{
    // int newX1 = (int(p1.getX() * cos(rad) - p1.getY() * sin(rad)));
    // int newY1 = (int(p1.getX() * sin(rad) + p1.getY() * cos(rad)));

    p1.setX(p1.getX()*size);
    p1.setY(p1.getY()*size);

    // int newX2 = (int(p2.getX() * cos(rad) - p2.getY() * sin(rad)));
    // int newY2 = (int(p2.getX() * sin(rad) + p2.getY() * cos(rad)));

    
    p2.setX(p2.getX()*size);
    p2.setY(p2.getY()*size);
}

void GLine::printPoints()
{
    Serial.println("[" + String(p1.getX()) + "," + String(p1.getY()) + "]");
    Serial.println("[" + String(p2.getX()) + "," + String(p2.getY()) + "]");
}

void GLine::render()
{
    display->drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
}