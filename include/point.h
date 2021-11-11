#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>

class Point {

    public:
        float x;
        float y;

        Point();
        Point(float x, float y);

        Point operator+(Point v);
        Point operator-(Point v);
        Point operator+=(Point v);
        Point operator-=(Point v);

        friend std::ostream& operator<<(std::ostream& out, const Point& v);

};

#endif // __VECTOR_H__