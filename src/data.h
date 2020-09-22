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

void print_events()
{
    for (int i = 0; i < last_events_index(); i++)
    {
        Serial.print(events_name[i] + "->" + events_time[i] + "; ");
    }
    Serial.println("");
}

void shift_events(int index)
{
    int index_last = last_events_index();
    for (int i = index; i <= index_last; i++)
    {
        events_time[i + 1] = events_time[i];
        events_name[i + 1] = events_name[i];
    }
}

void insert_event(int i, String name, int h, int m)
{
    events_name[i] = name;
    events_time[i] = ((h < 10) ? "0" + String(h) : String(h)) + ":" + ((m < 10) ? "0" + String(m) : String(m));
}

bool new_event(String name, int h, int m)
{
    int index_last = last_events_index();
    Serial.println(String(h)+String(m));

    if (index_last != -1)
    {

        if (index_last == 0)
        {
            insert_event(0,name,h,m);
        }
        else
        {
            for (int i = 0; i <= index_last; i++)
            {
                if (h < events_time->substring(0, 2).toInt())
                {
                    shift_events(i);
                    insert_event(i,name,h,m);
                }
                else if (h == events_time->substring(0, 2).toInt())
                {
                    if (m < events_time->substring(3, 5).toInt())
                    {
                        shift_events(i);
                        insert_event(i,name,h,m);
                    }
                    else
                    {
                        i++;
                        shift_events(i);
                        insert_event(i,name,h,m);
                    }
                }
            }
        }
    }
}


#endif