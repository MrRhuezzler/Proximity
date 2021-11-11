#ifndef ZORDERING_H
#define ZORDERING_H

#include <string>
#include <algorithm>
#include <cmath>

#include "point.h"

class Z {

    private:
        static int N;

    private:

        // Binary to Decimal
        static int binToDec(std::string bin);

        // Decimal to Binary
        static std::string decToBin(int x);

        // Calculates X Coord from index
        static int X(int i);

        // Calculates Y Coord from index
        static int Y(int i);

    public:
        static Point toVec(int i);
        static int toIndex(Point v);

};

#endif
