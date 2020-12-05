#include "GIcon.h"

GIcon::GIcon()
{
    display = NULL;
}

GIcon::GIcon(U8G2 *_display)
{
    display = _display;
}

void GIcon::addGPolygon(GPolygon polygon)
{
    polygons.push_back(polygon);
}

void GIcon::addGLine(GLine line)
{
    lines.push_back(line);
}

void GIcon::scale(float size)
{
    for (auto i = polygons.begin(); i != polygons.end(); i++)
    {
        (*i).scale(size);
    }

    for (auto i = lines.begin(); i != lines.end(); i++)
    {
        (*i).scale(size);
    }
}

void GIcon::render()
{
    for (auto i = polygons.begin(); i != polygons.end(); i++)
    {
        (*i).render();
    }

    for (auto i = lines.begin(); i != lines.end(); i++)
    {
        (*i).render();
    }
}