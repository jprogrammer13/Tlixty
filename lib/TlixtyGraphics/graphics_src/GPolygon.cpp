#include "GPolygon.h"

GPolygon::GPolygon(U8G2 *_display, int _pointsNumber, int _pointsArray[])
{
    display = _display;
    pointsNumber = _pointsNumber;

    // Make a matrix [[x0,y0],[x1,y1],...,[xN,yN]]

    pointsArray = new int *[pointsNumber];
    for (int i = 0; i < pointsNumber; i++)
    {
        pointsArray[i] = new int[2];
    }

    for (int i = 0, j = 0; i < pointsNumber; i++, j += 2)
    {
        pointsArray[i][0] = _pointsArray[j];
        pointsArray[i][1] = _pointsArray[j + 1];
    }

    // for (int i = 0; i < pointsNumber; i++)
    // {

    //     Serial.println("[" + String(pointsArray[i][0]) + "," + String(pointsArray[i][1]) + "]");
    // }

    for (int i = 0; i < pointsNumber - 1; i++)
    {
        // Serial.println(String(pointsArray[i][0]) + "," + String(pointsArray[i][1]) + "-->" + String(pointsArray[i + 1][0]) + "," + String(pointsArray[i + 1][1]));
        segments.push_back(GLine(display, GPoint(pointsArray[i][0], pointsArray[i][1]), GPoint(pointsArray[i + 1][0], pointsArray[i + 1][1])));
    }
}

GPolygon::~GPolygon()
{
}

void GPolygon::printPoints()
{
    for (int i = 0; i < pointsNumber; i++)
    {

        Serial.println("[" + String(pointsArray[i][0]) + "," + String(pointsArray[i][1]) + "]");
    }
}

void GPolygon::scale(float size)
{
    for (auto i = segments.begin(); i != segments.end(); i++)
    {
        (*i).scale(size);
    }
}

void GPolygon::render()
{
    for (auto i = segments.begin(); i != segments.end(); i++)
    {
        (*i).render();
    }
}