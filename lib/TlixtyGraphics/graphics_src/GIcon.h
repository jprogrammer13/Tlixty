#ifndef __GICON_H__
#define __GICON_H__

#include <Arduino.h>
#include <U8g2lib.h>
#include "GPoint.h"
#include "GLine.h"
#include "GPolygon.h"
#include <vector>
#include <tuple>

using namespace std;

class GIcon
{
private:
    U8G2 *display;
    vector<GPolygon> polygons;
    vector<GLine> lines;

public:
    GIcon();
    GIcon(U8G2 *_display);
    void addGPolygon(GPolygon polygon);
    void addGLine(GLine line);
    void scale(float size);
    void render();
};

#endif