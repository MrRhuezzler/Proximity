#include "map.h"

static const char* modeNames[(int)Mode::MODE_COUNT] = {"IDLE", "EDIT"};
static const char* terrainNames[(int)Terrain::TERRAIN_COUNT] = {"VOID", "BOUNDARY", "LOCATION", "HOTEL", "MARKET", "HOSPITAL", "BOUTIQUE"};

std::ostream& operator<< (std::ostream& stm, Terrain t)
{
    switch(t)
    {
        case Terrain::LOCATION: 
            return stm << "LOCATION";
        case Terrain::BOUNDARY:
            return stm << "BOUNDARY";
        case Terrain::BOUTIQUE:
            return stm << "BOUTIQUE";
        case Terrain::HOSPITAL:
            return stm << "HOSPITAL";
        case Terrain::HOTEL:
            return stm << "HOTEL";
        case Terrain::MARKET:
            return stm << "MARKET";
        case Terrain::VOID:
            return stm << "VOID" ;
        default:
            return stm << "Terrain{" << int(t) << '}' ;
    }
}

Map::Map(int width, int height, int numOfCols, int numOfRows) 
:width(width), height(height), numOfCols(numOfCols), numOfRows(numOfRows)
{

    mode = Mode::IDLE;
    terrain = Terrain::VOID;
    mousePressed = false;

    _w = width / numOfCols;
    _h = height / numOfRows;
    map = std::vector<mapCell>(_w * _h);

    for(int i = 0; i < _w; i++){
        for(int j = 0; j < _h; j++){
            
            map[transform(i, j)].type = Terrain::VOID;

            map[transform(i, j)].srcRect.x = i * numOfCols;
            map[transform(i, j)].srcRect.y = j * numOfRows;
            map[transform(i, j)].srcRect.w = numOfCols;
            map[transform(i, j)].srcRect.h = numOfRows;

            map[transform(i, j)].dstRect.x = i * numOfCols;
            map[transform(i, j)].dstRect.y = j * numOfRows;
            map[transform(i, j)].dstRect.w = numOfCols;
            map[transform(i, j)].dstRect.h = numOfRows;

        }
    }

    // Setting up boundaries of the world
    for(int j = 0; j < _h; j++){
        map[transform(0, j)].type = Terrain::BOUNDARY;
        map[transform((_w - 1), j)].type = Terrain::BOUNDARY;
    }

    for(int i = 0; i < _w; i++){
        map[transform(i, 0)].type = Terrain::BOUNDARY;
        map[transform(i, (_h - 1))].type = Terrain::BOUNDARY;
    }

}

void Map::setColor(SDL_Renderer *renderer, Terrain type){
    switch (type)
    {
    case Terrain::BOUNDARY:
        SDL_SetRenderDrawColor(renderer, 102, 102, 102, 255);
        break;

    case Terrain::LOCATION:
        SDL_SetRenderDrawColor(renderer, 255, 0, 150, 255);
        break;

    case Terrain::HOTEL:
        SDL_SetRenderDrawColor(renderer, 56, 242, 87, 255);
        break;

    case Terrain::HOSPITAL:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;

    case Terrain::MARKET:
        SDL_SetRenderDrawColor(renderer, 33, 23, 227, 255);
        break;

    case Terrain::BOUTIQUE:
        SDL_SetRenderDrawColor(renderer, 23, 227, 186, 255);
        break;

    default:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        break;
    }
}

void Map::Draw(SDL_Renderer *renderer){

    for(int i = 0; i < _w * _h; i++){
        //map[i].type = terrain;                                              //NEW-Agi
        if(map[i].type != Terrain::VOID){
            setColor(renderer, map[i].type);
            SDL_RenderFillRectF(renderer, &map[i].dstRect);
        }
    }
}

void Map::edit(){

    int x, y;
    
    SDL_PumpEvents();
    const Uint32 mState = SDL_GetMouseState(&x, &y);
    const Uint8 *kState = SDL_GetKeyboardState(NULL);

    Point mouse(x / numOfCols, y / numOfRows);

    if(!mousePressed && (mState & SDL_BUTTON_LMASK)){
        mousePressed = true;
        if(map[transform(mouse)].type != Terrain::BOUNDARY)
            if(map[transform(mouse)].type != Terrain::VOID)
                map[transform(mouse)].type = Terrain::VOID;
            else
                map[transform(mouse)].type = terrain;
    }

    if(mousePressed && (!(mState & SDL_BUTTON_LMASK))){
        mousePressed = false;
    }

}

void Map::setMode(){
    
    SDL_PumpEvents();
    const Uint8 *kState = SDL_GetKeyboardState(NULL);

    if(kState[SDL_SCANCODE_E]){
        mode = Mode::EDIT;
    }

}

void Map::Update(){

    SDL_PumpEvents();
    const Uint8 *kState = SDL_GetKeyboardState(NULL);
    
    if(kState[SDL_SCANCODE_R] && kState[SDL_SCANCODE_LCTRL]){
        mode = Mode::IDLE;
    }

    switch (mode)
    {
        case Mode::IDLE:
            setMode();
            break;

        case Mode::EDIT:
            edit();
            break;

        default:
            // ImGui::Begin("Debug");
            // ImGui::Text("Mode Count State");
            // ImGui::End();
            break;
    }
}



void Map::UI(){

    ImGui::Begin("Mode Selector");
    const char* currentModeName = (mode >= Mode::IDLE && mode < Mode::MODE_COUNT) ? modeNames[(int)mode] : "Unknown";
    ImGui::SliderInt("Mode", (int*)(&mode), 0, ((int)Mode::MODE_COUNT) - 1, currentModeName);
    ImGui::End();

    ImGui::Begin("Terrain Selector");
    const char* currentTerrainName = (terrain >= Terrain::VOID && terrain < Terrain::TERRAIN_COUNT) ? terrainNames[(int)terrain] : "Unknown";
    ImGui::SliderInt("Terrain", (int*)(&terrain), 0, ((int)Terrain::TERRAIN_COUNT) - 1, currentTerrainName);
    ImGui::End();
}

// i - cols
// j - rows
int Map::transform(int i, int j) 
{
    return (_w) * j + i;
}

int Map::transform(Point v)
{
    return transform(v.x, v.y);
}
