#include "Point.h"

Point::Point()
{
    x=-1, y=-1;
}

Point::Point(unsigned long x0, unsigned long y0)
    : x(x0), y(y0)
{
}

Point::Point(const Point &pt)
{
    x=pt.x; y=pt.y;
}

bool Point::isValid() const
{
    return !(x==-1 && y==-1);
}
