#ifndef __GPOLYGON_H__
#define __GPOLYGON_H__

#include <Arduino.h>
#include <U8g2lib.h>
#include "GPoint.h"
#include "GLine.h"
#include <vector>

using namespace std;

class GPolygon
{
private:
    U8G2 *display;
    int pointsNumber;
    int **pointsArray;
    vector<GLine> segments;

public:
    GPolygon(U8G2 *_display, int _pointsNumber, int _pointsArray[]);
    ~GPolygon();
    void printPoints();
    void scale(float size);
    void render();
};


#endif