#include "zorder.h"


Z::pointBitString Z::pointToBitString(Point p) 
{

    int xResolution = 0, yResolution = 0;
    for(char dim : zMapFunction){
        if(dim == '0'){
            xResolution++;
        }else{
            yResolution++;
        }
    }

    std::string xBitString = decToBin(p.x);
    std::string yBitString = decToBin(p.y);

    if(resolution > xBitString.size()){
        std::string nullbits = std::string(xResolution - xBitString.size(), '0');
        xBitString = nullbits + xBitString;
    }

    if(resolution > yBitString.size()){
        std::string nullbits = std::string(yResolution - yBitString.size(), '0');
        yBitString = nullbits + yBitString;
    }

    return {xBitString, yBitString};

}

std::string Z::pointBitStringToZCode(Z::pointBitString ps) 
{

    int bitsX = ps.xBitString.size();
    int bitsY = ps.yBitString.size();

    std::string key = "";
    for(char dim : zMapFunction){

        int index;
        bool isXVal = false;
        if(dim == '0'){
            isXVal = true;
            index =  ps.xBitString.size() - bitsX;
            bitsX--;
        }else{
            isXVal = false;
            index = ps.yBitString.size() - bitsY;
            bitsY--;
        }

        if(isXVal && index < ps.xBitString.size()){
            key += ps.xBitString.at(index);
        }

        if(!isXVal && index < ps.yBitString.size()){
            key += ps.yBitString.at(index);
        }

    }

    return key;

}

std::string Z::toBitString(int zCode) 
{
    std::string zCodeBitString = decToBin(zCode);
    if(zMapFunction.size() > zCodeBitString.size()){
        std::string nullbits = std::string(zMapFunction.size() - zCodeBitString.size(), '0');
        return nullbits + zCodeBitString;        
    }

    std::reverse(zCodeBitString.begin(), zCodeBitString.end());
    std::string lastBits = zCodeBitString.substr(0, zMapFunction.size());
    std::reverse(lastBits.begin(), lastBits.end());
    return lastBits;

}

Z::pointBitString Z::zCodeToPointBitString(std::string zBitString) 
{
    std::string xBitString = "";
    std::string yBitString = "";
    for(int i = 0; i < zMapFunction.size(); i++){

        char dim = zMapFunction.at(i);
        if(dim == '0'){
            xBitString += zBitString.at(i);
        }else{
            yBitString += zBitString.at(i);
        }

    }

    return {xBitString, yBitString};
}

std::string Z::decToBin(int value) 
{
    std::string bin = "";
    while(value > 0){
        bin += ((value % 2) == 1) ? "1" : "0";
        value /= 2;
    }

    if(value == 0){
        bin += '0';
    }

    std::reverse(bin.begin(), bin.end());
    return bin;
}

int Z::binToDec(std::string bitString){
    
    int x = 0;
    std::reverse(bitString.begin(), bitString.end());

    int i = 0;
    for(char b: bitString){
        x += ((b == '1') ? 1 : 0) * (int)pow(2.0, i);
        i++;
    }

    return x;
}

int Z::computeZCode(Point p) 
{
    return binToDec(pointBitStringToZCode(pointToBitString(p)));
}

Point Z::computePoint(int zCode) 
{
    Z::pointBitString point = zCodeToPointBitString(toBitString(zCode));
    return Point(binToDec(point.xBitString) * colScale, binToDec(point.yBitString) * rowScale);
}

std::vector<Point> Z::buildPath()
{
    int maxPoints = (1 << resolution) * (1 << resolution) - 1;
    return buildPathFromRange(0, maxPoints);
}

std::vector<Point> Z::buildPathFromRange(int min, int max) 
{
    std::vector<Point> path;
    for(int x = min; x <= max; x++){
        std::string zString = toBitString(x);
        Z::pointBitString coord = zCodeToPointBitString(zString);
        Point p = Point(binToDec(coord.xBitString) * colScale, binToDec(coord.yBitString) * rowScale);
        path.push_back(p);
    }

    return path;
}

Z::Z(){

    colScale = 10;
    rowScale = 10;
    resolution = 4;

    zMapFunction = "";
    for(int i = 0; i < resolution; i++){
        zMapFunction += "01";
    }

    space = buildPath();

}

Z::Z(int colScale, int rowScale, int resolution)
{
    this->resolution = resolution;
    zMapFunction = "";
    for(int i = 0; i < resolution; i++){
        zMapFunction += "01";
    }
    this->colScale = colScale;
    this->rowScale = rowScale;
    space = buildPath();

}

std::vector<Point>& Z::getSpace() 
{
    return space;
}

std::vector<Point> Z::getRange(Point tl, Point m, Point rb){

    int zHigh = computeZCode(rb);
    int zM = computeZCode(m);
    int zLow = computeZCode(tl);

    std::vector<Point> points;
    for(int i = zLow; i < zHigh; i++){
        Point p = computePoint(i);
        if(p.x < tl.x || p.y < tl.y || p.x > rb.x || p.y > rb.y) continue;
        points.push_back(computePoint(i));
    }

    return points;
}
