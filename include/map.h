#ifndef __MAP_H__
#define __MAP_H__

#include <vector>
#include "SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "point.h"

enum class Terrain {
    VOID,
    BOUNDARY,
    LOCATION,
};

enum class Mode {
    IDLE,
    EDIT,
    MODE_COUNT
};

// static const char* modeNames[(int)Mode::MODE_COUNT] = {"IDLE", "EDIT"};
std::ostream& operator<< (std::ostream&, Terrain);

class Map {

    int width, height;
    int numOfCols, numOfRows;
    int _w, _h;

    struct mapCell {
        Terrain type;
        SDL_FRect srcRect;
        SDL_FRect dstRect;
    };

    std::vector<mapCell> map;

    Mode mode;

    // mouse debounce
    bool mousePressed;

    public:
        Map(int width, int height, int numOfCols = 10, int numOfRows = 10);
        void draw(SDL_Renderer *renderer);
        void update();
        void UI();

    private:
        int transform(int i, int j);
        int transform(Point v);

        inline void edit();
        inline void setMode();

        void setColor(SDL_Renderer *renderer, Terrain type);

};

#endif // __MAP_H__