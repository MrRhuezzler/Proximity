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
            index = bitsX - ps.xBitString.size();
            bitsX--;
        }else{
            isXVal = false;
            index = bitsY - ps.yBitString.size();
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
    std::string lastBits = zCodeBitString.substr(0, zCodeBitString.size() - zMapFunction.size());
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

std::string Z::computeZCode(Point p) 
{
    return pointBitStringToZCode(pointToBitString(p));
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
        Point p = Point(binToDec(coord.xBitString), binToDec(coord.yBitString));
        path.push_back(p);
    }

    return path;
}

std::string Z::simpleReplaceAndFill(int pos, std::string source, char c, char fillc){

    std::string prefix = source.substr(0, pos) + c;
    std::string suffix = "";
    for(int i = pos + 1; i < source.size(); i++){
        suffix += fillc;
    }
    return prefix + suffix;

}

std::vector<Z::queryBox> Z::cutBox(char hyperPlaneDimension, int hyperPlaneCutIndex[], Point tl, Point br) 
{

    Z::pointBitString highStrings = pointToBitString(tl);
    Z::pointBitString lowStrings = pointToBitString(br);

    std::string x1Low = lowStrings.xBitString;
    std::string y1Low = lowStrings.yBitString;
    std::string x1High = highStrings.xBitString;
    std::string y1High = highStrings.yBitString;

    if(hyperPlaneDimension == '0'){
        x1High = simpleReplaceAndFill(hyperPlaneCutIndex[0], x1High, '0', '1');
    }else{
        y1High = simpleReplaceAndFill(hyperPlaneCutIndex[1], y1High, '0', '1');
    }

    std::string x2Low = lowStrings.xBitString;
    std::string y2Low = lowStrings.yBitString;
    std::string x2High = highStrings.xBitString;
    std::string y2High = highStrings.yBitString;

    if(hyperPlaneDimension == '1'){
        x2Low = simpleReplaceAndFill(hyperPlaneCutIndex[0], x2Low, '1', '0');
    }else{
        y2Low = simpleReplaceAndFill(hyperPlaneCutIndex[1], y2Low, '1', '0');
    }

    std::vector<Z::queryBox> box(2);

    Z::queryBox box1 = {
        Point(binToDec(x1High), binToDec(y1High)),
        Point(binToDec(x1Low), binToDec(y1Low))
    };
    box.emplace_back(box1);

    Z::queryBox box2 = {
        Point(binToDec(x2High), binToDec(y2High)),
        Point(binToDec(x2Low), binToDec(y2Low))
    };

    box.emplace_back(box2);

    return box;

}

std::vector<Z::queryZCode> Z::inRange(int hyperPlanePosition, Point tl, Point br, int highestBit) 
{

    std::string zCodeLow = computeZCode(br);
    std::string zCodeHigh = computeZCode(tl);

    int index;
    char hyperPlaneDimension = '0';
    int hyperPlaneCutIndex[2] = {-1, -1};

    for(int i = 0; i < highestBit; i++){

        hyperPlaneDimension = zMapFunction.at(i);
        if(hyperPlaneDimension == '0'){
            hyperPlaneCutIndex[0]++;
        }else{
            hyperPlaneCutIndex[1]++;
        }

        index = i;
        if(zCodeHigh.at(index)  != zCodeLow.at(i)){
            break;
        }

    }

    if(index > highestBit){
    
        std::vector<Z::queryZCode> returned = std::vector<Z::queryZCode>();
        Z::queryZCode z1 = {binToDec(zCodeLow), binToDec(zCodeHigh)};
        returned.push_back(z1);
        return returned;

    }

    bool allOnes = true;
    bool allZeros = true;

    for(int i = index; i < highestBit; i++){
        if(zCodeHigh.at(i) != '1'){
            allOnes = false;
            break;
        }

        if(zCodeLow.at(i) != '0'){
            allZeros = false;
            break;
        }
    }

    if(allOnes && allZeros){

        std::vector<Z::queryZCode> returned = std::vector<Z::queryZCode>();
        Z::queryZCode z1 = {binToDec(zCodeLow), binToDec(zCodeHigh)};
        returned.push_back(z1);
        return returned;

    }

    std::vector<Z::queryBox> boxes = cutBox(hyperPlaneDimension, hyperPlaneCutIndex, tl, br);
    
    std::vector<Z::queryZCode> leftRanges = inRange(hyperPlanePosition, boxes[0].tl, boxes[0].br, highestBit);
    std::vector<Z::queryZCode> rightRanges = inRange(hyperPlanePosition, boxes[1].tl, boxes[1].br, highestBit);

    int lastLeft = leftRanges[leftRanges.size() - 1].high;
    int firstRight = rightRanges[0].low;

    if(firstRight - lastLeft == 1){

        Z::queryZCode merge = {
            leftRanges[leftRanges.size() - 1].low,
            rightRanges[0].high
        };

        leftRanges.pop_back();
        leftRanges.push_back(merge);
        for(int i = 1;i < rightRanges.size(); i++){
            leftRanges.push_back(rightRanges.at(i));
        }

    }else{

        for(int i = 0;i < rightRanges.size(); i++){
            leftRanges.push_back(rightRanges.at(i));
        }

    }

    return leftRanges;

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

}
