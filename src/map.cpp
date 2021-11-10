#include <map.h>

Map::Map(int width, int height, int numOfCols, int numOfRows) 
:width(width), height(height), numOfCols(numOfCols), numOfRows(numOfRows)
{
    map = std::vector<mapCell>(numOfCols * numOfRows);
}
