#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Dictionary.h>

const int n_events = 30;

String events_name[n_events];
String events_time[n_events];

void clean_events()
{
    for (int i = 0; i < n_events; i++)
    {
        events_name[i] = "";
        events_time[i] = "";
    }
}

int last_events_index()
{
    int index = -1; //full

    for (int i = 0; i < n_events; i++)
    {
        if (events_time[i] == "")
        {
            index = i;
            i = n_events;
        }
    }

    return index;
}

bool new_event(String name, int h, int m)
{
    int index_last = last_events_index();
    if (index_last != -1)
    {
        if (index_last == 0)
        {
            events_name[0] = name;
            events_time[0] = ((h < 10) ? "0" + String(h) : String(h)) + ":" + ((m < 10) ? "0" + String(m) : String(m));
        }
        else
        {
            for (int i = 0; i <= index_last; i++)
            {
                if (h < events_time->substring(0, 2).toInt())
                {
                    // chiama lo shifter
                    //inserisco
                }
                else if (h == events_time->substring(0, 2).toInt())
                {
                    if (m < events_time->substring(3, 5).toInt())
                    {
                        //chiamo lo shifter
                        // inserisco
                    }
                    else
                    {
                        //i++
                        //chiamo lo shifter
                        //inserisco
                    }
                }
            }
        }
    }
}

#endif