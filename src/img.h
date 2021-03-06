#ifndef IMG_H
#define IMG_H

#include <Arduino.h>

#define logo_width 61
#define logo_height 60


static unsigned char logo[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x01,
   0x00, 0x00, 0xe8, 0xee, 0xee, 0xee, 0xee, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0xff, 0x7f, 0x04, 0x00, 0x00, 0xba, 0xbb, 0xbb, 0xbb, 0x3b, 0x06,
   0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x07, 0x00, 0x80, 0xee, 0xee,
   0xee, 0xee, 0x8e, 0x07, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x07,
   0x00, 0xa0, 0xbb, 0xbb, 0xbb, 0xbb, 0xe3, 0x07, 0x00, 0xf0, 0xff, 0xff,
   0xff, 0xff, 0xf1, 0x07, 0x00, 0xe8, 0xee, 0xee, 0xee, 0xee, 0xf8, 0x07,
   0x00, 0xfc, 0xff, 0xff, 0xff, 0x7f, 0xfc, 0x07, 0x00, 0xba, 0xbb, 0xbb,
   0xbb, 0x3b, 0xfe, 0x07, 0x00, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x07,
   0x80, 0xee, 0xee, 0xee, 0xee, 0x8e, 0xff, 0x03, 0xc0, 0xff, 0xff, 0xff,
   0xff, 0xc7, 0xff, 0x01, 0xa0, 0xbb, 0xbb, 0xbb, 0xbb, 0xe3, 0xff, 0x00,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x7f, 0x00, 0xe8, 0xee, 0xee, 0xee,
   0xee, 0xf8, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0x00, 0xfe, 0xff, 0xff, 0xff,
   0x7f, 0xfc, 0x0f, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x2a, 0xfc, 0x0f, 0x00,
   0xfe, 0xff, 0xff, 0xff, 0x7f, 0xfc, 0x0f, 0x00, 0xaa, 0xaa, 0xaa, 0xaa,
   0x2a, 0xfc, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0xfc, 0x0f, 0x00,
   0xaa, 0xaa, 0xaa, 0xaa, 0x2a, 0xfc, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff,
   0x7f, 0xfc, 0x0f, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x2a, 0xcc, 0x0f, 0x00,
   0xfe, 0xff, 0xff, 0xff, 0x7f, 0xc4, 0x0f, 0x00, 0xaa, 0xaa, 0xaa, 0xaa,
   0x2a, 0xc0, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0xc0, 0x0f, 0x00,
   0xaa, 0xaa, 0xaa, 0xaa, 0x2a, 0xc0, 0x0f, 0x00, 0xfe, 0xff, 0xff, 0xff,
   0x7f, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0xc8, 0x0f, 0x00,
   0x00, 0x00, 0xff, 0x01, 0x40, 0xc5, 0x0f, 0x00, 0x00, 0x00, 0xaa, 0xf8,
   0x27, 0xc2, 0x0f, 0x00, 0x00, 0x00, 0xff, 0xf9, 0x47, 0xc5, 0x0f, 0x00,
   0x00, 0x00, 0xaa, 0xf8, 0x87, 0xc8, 0x0f, 0x00, 0x00, 0x00, 0xff, 0xf9,
   0x47, 0xc5, 0x0f, 0x00, 0x00, 0x00, 0xaa, 0xf8, 0x27, 0xc2, 0x0f, 0x00,
   0x00, 0x00, 0xff, 0xf9, 0x47, 0xc5, 0x07, 0x00, 0x00, 0x00, 0xaa, 0xf8,
   0x87, 0xc8, 0x03, 0x00, 0x00, 0x00, 0xff, 0xf9, 0x47, 0xc5, 0x01, 0x00,
   0x00, 0x00, 0xaa, 0xf8, 0x27, 0xc2, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf9,
   0x47, 0x45, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xf8, 0x87, 0x08, 0x00, 0x00,
   0x00, 0x00, 0xff, 0xf9, 0x47, 0x05, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xf8,
   0x23, 0x02, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf9, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xaa, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x79,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x38, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xff, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x08,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00 };


#endif