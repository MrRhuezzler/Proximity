#ifndef __ZORDER_H__
#define __ZORDER_H__

#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

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

        std::vector<Point> inRange(Point tl, Point br);

        std::string computeZCode(Point p);
        std::vector<Point> buildPath();
        std::vector<Point> buildPathFromRange(int min, int max);

    public:
        std::vector<Point> getPoints(std::vector<queryZCode>& zRanges);

        std::vector<queryBox> cutBox(char hyperPlaneDimension, int hyperPlaneCutIndex[], Point tl, Point br);

        std::vector<queryZCode> getRanges(int hyperPlanePosition, Point tl, Point br, int highestBit);
        
        pointBitString pointToBitString(Point p);
        std::string pointBitStringToZCode(pointBitString ps);
        std::string toBitString(int zCode);
        pointBitString zCodeToPointBitString(std::string zBitString);
        std::string decToBin(int value);
        int binToDec(std::string bitString);

        std::string simpleReplaceAndFill(int pos, std::string source, char c, char fillc);

};

#endif // __ZORDER_H__