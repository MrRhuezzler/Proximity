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
        Z(int numOfCols, int numOfRows, int resolution);
        std::vector<queryZCode> inRange(int hyperPlanePosition, Point tl, Point br, int highestBit);
        std::string computeZCode(Point p);
        std::vector<Point> buildPath();
        std::vector<Point> buildPathFromRange(int min, int max);

    private:
        std::vector<queryBox> cutBox(char hyperPlaneDimension, int hyperPlaneCutIndex[], Point tl, Point br);

        pointBitString pointToBitString(Point p);
        std::string pointBitStringToZCode(pointBitString ps);
        std::string toBitString(int zCode);
        pointBitString zCodeToPointBitString(std::string zBitString);
        std::string decToBin(int value);
        int binToDec(std::string bitString);

        std::string simpleReplaceAndFill(int pos, std::string source, char c, char fillc);
        
};

#endif // __ZORDER_H__