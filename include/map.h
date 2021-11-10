#ifndef __MAP_H__
#define __MAP_H__

#include <vector>
#include <SDL2/SDL.h>

#include <point.h>

enum class Terrain {
    VOID, 
    BOUNDARY,
    LOCATION,
};

std::ostream& operator<< (std::ostream&, Terrain);

class Map {

    int width, height;
    int numOfCols, numOfRows;

    struct mapCell {
        Terrain type;
        SDL_FRect srcRect;
        SDL_FRect dstRect;
    };

    std::vector<mapCell> map;

    public:
        Map(int width, int height, int numOfCols = 10, int numOfRows = 10);

};

#endif // __MAP_H__