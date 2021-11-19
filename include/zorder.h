#ifndef __ZORDER_H__
#define __ZORDER_H__

#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "point.h"

class Z {

    std::string zMapFunction;
    int resolution;
    int colScale, rowScale;

    std::vector<Point> space;

    public:
        struct pointBitString {
            std::string xBitString;
            std::string yBitString;
        };

        struct queryZCode
        {
            int low;
            int high;
        };

        struct queryBox
        {
            Point tl;
            Point br;
        };

    public:
        Z();
        Z(int colScale, int rowScale, int resolution);
        std::vector<Point>& getSpace();

        std::string computeZCode(Point p);
        Point computePoint(int zCode);

        std::vector<Point> buildPath();
        std::vector<Point> buildPathFromRange(int min, int max);

    public:
        pointBitString pointToBitString(Point p);
        std::string pointBitStringToZCode(pointBitString ps);
        std::string toBitString(int zCode);
        pointBitString zCodeToPointBitString(std::string zBitString);
        std::string decToBin(int value);
        int binToDec(std::string bitString);

};

#endif // __ZORDER_H__