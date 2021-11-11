#include "map.h"
#include <list>

#define TURN_TO_STRING(x) #x

static const char* modeNames[(int)Mode::MODE_COUNT] = {"IDLE", "EDIT", "QUERY", "UPDATE"};
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
            return stm << "VOID";
        default:
            return stm << "Terrain{" << int(t) << '}' ;
    }
}

Map::Map(int width, int height, int numOfCols, int numOfRows) 
:width(width), height(height), numOfCols(numOfCols), numOfRows(numOfRows)
{

    mode = Mode::IDLE;
    edit_terrain = Terrain::VOID;
    query_terrain = Terrain::HOTEL;
    mousePressed = false;
    qt = nullptr;
    locations = std::set<int>();
    searchProximity = Point(100, 100);
    searchProximityVisible = false;
    highlightBorder = 5;
    center = Point(width / 2.0, height / 2.0);

    _w = width / numOfCols;
    _h = height / numOfRows;
    map = std::vector<mapCell>(_w * _h);

    for(int i = 0; i < _w; i++){
        for(int j = 0; j < _h; j++){

            map[transform(i, j)].type = Terrain::VOID;
            map[transform(i, j)].is_glowing = false;

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
        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        break;
    }
}

void Map::Draw(SDL_Renderer *renderer){

    for(int i = 0; i < _w * _h; i++){
        //map[i].type = terrain;                                              //NEW-Agi
        if(map[i].type != Terrain::VOID){

            if(map[i].is_glowing){

                map[i].dstRect.x = map[i].srcRect.x - (highlightBorder / 2);
                map[i].dstRect.y = map[i].srcRect.y - (highlightBorder / 2);
                map[i].dstRect.w = map[i].srcRect.w + (highlightBorder);
                map[i].dstRect.h = map[i].srcRect.h + (highlightBorder);
                setColor(renderer, Terrain::VOID);
                SDL_RenderFillRectF(renderer, &map[i].dstRect);

            }

            setColor(renderer, map[i].type);
            map[i].dstRect = map[i].srcRect;
            SDL_RenderFillRectF(renderer, &map[i].dstRect);

        }
    }

    if(searchProximityVisible && mode == Mode::QUERY){

        int x, y;
        SDL_PumpEvents();
        SDL_GetMouseState(&x, &y);
        SDL_FRect mouseRect = SDL_FRect{
            x - (searchProximity.x),
            y - (searchProximity.y),
            searchProximity.x * 2,
            searchProximity.y * 2,
        };

        SDL_RenderDrawRectF(renderer, &mouseRect);
        
    }


}

void Map::edit(){

    int x, y;
    
    SDL_PumpEvents();
    const Uint32 mState = SDL_GetMouseState(&x, &y);
    const Uint8 *kState = SDL_GetKeyboardState(NULL);

    if(x > width || x < 0 || y < 0 || y > height){
        return;
    }

    Point mouse(x / numOfCols, y / numOfRows);

    if(!mousePressed && (mState & SDL_BUTTON_LMASK)){
        mousePressed = true;
        if(map[transform(mouse)].type != Terrain::BOUNDARY)
            if(map[transform(mouse)].type != Terrain::VOID){

                map[transform(mouse)].type = Terrain::VOID;
                auto it = locations.find(transform(mouse));
                if(it != locations.end()){
                    locations.erase(it);
                }

            }else{
                map[transform(mouse)].type = edit_terrain;
                if(edit_terrain != Terrain::VOID || edit_terrain != Terrain::BOUNDARY){
                    locations.insert(transform(mouse));
                }
            }
    }

    if(mousePressed && (!(mState & SDL_BUTTON_LMASK))){
        mousePressed = false;
    }

}

void Map::update(){

    if(qt){
        delete qt;
    }

    qt = new Quad(Point(0, 0), Point(width, height), 4);
    for(auto it = locations.begin(); it != locations.end(); it++){
        Point p = (Point(map[*it].srcRect.x, map[*it].srcRect.y));
        Node* n = new Node(p.x, p.y, map[*it].type);
        qt->insert(n);
    }

    mode = Mode::IDLE;

}

void Map::query(){

    if(!qt){
        return;
    }

    for(auto i : locations){
        map[i].is_glowing = false;
    }

    int x, y;
    SDL_PumpEvents();
    const Uint32 mState = SDL_GetMouseState(&x, &y);

    if(x > width || x < 0 || y < 0 || y > height){
        return;
    }

    Point mouse(x, y);
    Point tl = (mouse - searchProximity);
    Point rb = (mouse + searchProximity);

    auto desired_locations = qt->inRange(tl, rb, query_terrain);

    for(auto d : desired_locations){
        map[transform(d->x / numOfCols, d->y / numOfRows)].is_glowing = true;
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
        
        case Mode::UPDATE:
            update();
            break;
        
        case Mode::QUERY:
            query();
            break;

        default:
            break;
    }
}


void Map::UI(){

    ImGui::Begin("Map Editor");

    ImGui::Text("Mode Selector");
    const char* currentModeName = (mode >= Mode::IDLE && mode < Mode::MODE_COUNT) ? modeNames[(int)mode] : "Unknown";
    ImGui::SliderInt("Mode", (int*)(&mode), 0, ((int)Mode::MODE_COUNT) - 1, currentModeName);

    if(mode == Mode::EDIT){
        ImGui::Text("Terrain Selector");
        const char* currentTerrainName = (edit_terrain >= Terrain::VOID && edit_terrain < Terrain::TERRAIN_COUNT) ? terrainNames[(int)edit_terrain] : "Unknown";
        ImGui::SliderInt("Terrain", (int*)(&edit_terrain), 0, ((int)Terrain::TERRAIN_COUNT) - 1, currentTerrainName);
    }

    if(mode == Mode::QUERY){

        ImGui::Text("Query Settings");
        ImGui::Checkbox("Show", &searchProximityVisible);
        ImGui::DragFloat("x", &searchProximity.x, 0.1f, 10.0f, 200.0f);
        ImGui::DragFloat("y", &searchProximity.y, 0.1f, 10.0f, 200.0f);

        int query_terrain_int = (int)query_terrain;
        const char* currentTerrainName = (query_terrain >= Terrain::VOID && query_terrain < Terrain::TERRAIN_COUNT) ? terrainNames[(int)query_terrain] : "Unknown";
        ImGui::SliderInt("Terrain", (int*)(&query_terrain), 0, ((int)Terrain::TERRAIN_COUNT) - 1, currentTerrainName);

    }

    ImGui::End();

}

Point Map::toFourCord(Point p){
    return Point(p.x - center.x, -(p.y - center.y));
}

Point Map::toComCord(Point p){
    return Point(p.x + center.x, center.y - p.y);
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


Map::~Map(){
    delete qt;
}