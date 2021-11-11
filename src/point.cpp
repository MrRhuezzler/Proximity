#include "point.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Point& v){
    out << "(" << v.x << ", "<< v.y << ")";
    return out;
}

Point::Point(float x, float y)
:x(x), y(y)
{}

Point Point::operator+(Point& v)
{
    return Point(x + v.x, y + v.y);
}

Point Point::operator-(Point& v) 
{
    return Point(x - v.x, y - v.y);
}

Point Point::operator+=(Point& v) 
{
    return Point(x + v.x, y + v.y);
}

Point Point::operator-=(Point& v) 
{
    return Point(x - v.x, y - v.y);
}
