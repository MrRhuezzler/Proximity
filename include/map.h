#ifndef __MAP_H__
#define __MAP_H__

#include <vector>
#include <set>
#include "SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "point.h"
#include "quadTree.h"

enum class Terrain {
    VOID,
    BOUNDARY,
    LOCATION,
    HOTEL,
    MARKET,
    HOSPITAL,
    BOUTIQUE,
    TERRAIN_COUNT
};
std::ostream& operator<< (std::ostream&, Terrain);

enum class Mode {
    IDLE,
    EDIT,
    QUERY,
    UPDATE,
    MODE_COUNT
};

// static const char* modeNames[(int)Mode::MODE_COUNT] = {"IDLE", "EDIT"};

class Quad;
class Map {

    int width, height;
    int numOfCols, numOfRows;
    int _w, _h;

    Point center;

    Point searchProximity;
    bool searchProximityVisible;
    struct mapCell {
        Terrain type;
        bool is_glowing;
        SDL_FRect srcRect;
        SDL_FRect dstRect;
    };

    std::vector<mapCell> map;
    int highlightBorder;

    // Locations
    std::set<int> locations;

    Mode mode;
    Terrain edit_terrain;        //The terrain to be drawn
    Terrain query_terrain;

    // mouse debounce
    bool mousePressed;

    // Quadtree
    Quad *qt;

    public:
        Map(int width, int height, int numOfCols = 10, int numOfRows = 10);
        ~Map();

        void Draw(SDL_Renderer *renderer);
        void Update();
        void UI();

    private:
        Point toFourCord(Point p);
        Point toComCord(Point p);

        int transform(int i, int j);
        int transform(Point v);

        inline void edit();
        inline void update();
        inline void query();

        inline void setMode();

        void setColor(SDL_Renderer *renderer, Terrain type);

};

#endif // __MAP_H__