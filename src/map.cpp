#include <map.h>

std::ostream& operator<< (std::ostream& stm, Terrain t)
{
    switch(t)
    {
        case Terrain::LOCATION: 
            return stm << "LOCATION";
        case Terrain::BOUNDARY:
            return stm << "BOUNDARY";
        case Terrain::VOID:
            return stm << "VOID" ;
        default:
            return stm << "Terrain{" << int(t) << '}' ;
    }
}

Map::Map(int width, int height, int numOfCols, int numOfRows) 
:width(width), height(height), numOfCols(numOfCols), numOfRows(numOfRows)
{
    map = std::vector<mapCell>(numOfCols * numOfRows);
}