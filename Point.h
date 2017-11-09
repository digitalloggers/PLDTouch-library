#pragma once
#include "Arduino.h"

struct Point
{
    unsigned long x, y;
    Point();
    Point(unsigned long x0, unsigned long y0);
    Point(const Point &pt);
    bool isValid() const;
};
