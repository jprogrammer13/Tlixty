#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Dictionary.h>

const int n_events = 30;

String events_name[n_events];
String events_time[n_events];
String events_description[n_events];
int events_icon[n_events];

void clean_events()
{
    for (int i = 0; i < n_events; i++)
    {
        events_name[i] = "";
        events_time[i] = "";
        events_description[i] = "";
        events_icon[i] = 0x0000;
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
        Serial.print(events_name[i] + "->" + events_time[i] + events_description[i] + "->" + events_icon[i] + "; ");
    }
    Serial.println("");
}

void shift_events(int index)
{
    int index_last = last_events_index();
    for (int i = index_last; i >= index; i--)
    {
        events_time[i + 1] = events_time[i];
        events_name[i + 1] = events_name[i];
        events_description[i + 1] = events_description[i];
        events_icon[i + 1] = events_icon[i];
    }
}

void insert_event(int i, String name, int h, int m, String descriptions, int icon)
{
    events_name[i] = name;
    events_time[i] = ((h < 10) ? "0" + String(h) : String(h)) + ":" + ((m < 10) ? "0" + String(m) : String(m));
    events_description[i] = descriptions;
    events_icon[i] = icon;
}

void new_event(String name, int h, int m, String descriptions, int icon)
{
    int index_last = last_events_index();

    if (index_last != -1)
    {
        for (int i = 0; i <= index_last; i++)
        {

            if (events_time[i] == "")
            {
                insert_event(i, name, h, m, descriptions, icon);
                i = index_last + 1;
            }
            else
            {

                int h_current_event = events_time[i].substring(0, 2).toInt();
                if (h < h_current_event)
                {
                    shift_events(i);
                    insert_event(i, name, h, m, descriptions, icon);
                    i = index_last + 1;
                }
                else if (h == h_current_event)
                {
                    if (m < events_time[i].substring(3, 5).toInt())
                    {
                        shift_events(i);
                        insert_event(i, name, h, m, descriptions, icon);
                        i = index_last + 1;
                    }
                }
            }
        }
    }
}

#endif