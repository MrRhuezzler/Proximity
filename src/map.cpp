#include "map.h"
#include <list>

#define TURN_TO_STRING(x) #x

static const char* modeNames[(int)Mode::MODE_COUNT] = {"IDLE", "EDIT", "QUERY", "UPDATE"};
static const char* terrainNames[(int)Terrain::TERRAIN_COUNT] = {"VOID", "BOUNDARY", "LOCATION", "HOTEL", "MARKET", "HOSPITAL", "BOUTIQUE"};

// Overloading 'cout' for printing the terrain type
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

// Constructor of the class Map
Map::Map(int width, int height, int numOfCols, int numOfRows) 
:width(width), height(height), numOfCols(numOfCols), numOfRows(numOfRows)
{

    mode = Mode::IDLE;                                              // The initial mode is set to IDLE
    edit_terrain = Terrain::VOID;                                   // The initial edit terrain is set to VOID
    query_terrain = Terrain::LOCATION;                              // The initial query terrain is set to LOCATION
    mousePressed = false;
    qt = nullptr;
    locations = std::set<int>();
    searchProximity = Point(100, 100);                              // The initial search range is set to a 100X100 square
    searchProximityVisible = false;
    highlightBorder = 5;
    center = Point(width / 2.0, height / 2.0);

    _w = width / numOfCols;                                         // Scaled width
    _h = height / numOfRows;                                        // Scaled height
    map = std::vector<mapCell>(_w * _h);

    // Initializing the blocks of the map
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

// Assigning colours for each terrain type
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

    for(int i = 0; i < _w * _h; i++){                       // Traversing all the blocks of the map row by row
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

    //Drawing the search range boundary
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

    if(x > width || x < 0 || y < 0 || y > height){                  // Checking if the mouse pointer is within the map boundaries
        return;
    }

    Point mouse(x / numOfCols, y / numOfRows);                      // Converting the position of the mouse to its equivalent block position in the map

    if(!mousePressed && (mState & SDL_BUTTON_LMASK)){
        mousePressed = true;                                        // mousePressed bool helps us to avoid holding down the cursor to draw multiple blocks
        if(map[transform(mouse)].type != Terrain::BOUNDARY)
            if(map[transform(mouse)].type != Terrain::VOID){        
                // Clicking on a non-VOID block turns it to VOID
                map[transform(mouse)].type = Terrain::VOID;
                auto it = locations.find(transform(mouse));
                if(it != locations.end()){
                    // Deleting the location from the set locations
                    locations.erase(it);
                }

            }else{
                map[transform(mouse)].type = edit_terrain;          // Setting the terrain of the block to be drawn
                if(edit_terrain != Terrain::VOID || edit_terrain != Terrain::BOUNDARY){
                    locations.insert(transform(mouse));             // Inserting the newly drawn location into the set locations
                }
            }
    }

    if(mousePressed && (!(mState & SDL_BUTTON_LMASK))){
        mousePressed = false;
    }

}

// Updating the map once it has been edited
void Map::update(){

    if(qt){                 // If a quad tree already exists, we delete it to create a new one for the newly added locations
        delete qt;
    }

    qt = new Quad(Point(0, 0), Point(width, height), 4);
    // Inserting the locations into the quad tree
    for(auto it = locations.begin(); it != locations.end(); it++){
        Point p = (Point(map[*it].srcRect.x, map[*it].srcRect.y));
        Node* n = new Node(p.x, p.y, map[*it].type);
        qt->insert(n);
    }

    // After updating, the mode is set to IDLE
    mode = Mode::IDLE;

}

void Map::query(){

    if(!qt){                            // If there is no quad tree, then there is nothing to query
        return;
    }

    for(auto i : locations){            
        map[i].is_glowing = false;      // Resetting the is_glowing bool if it was previously set
    }

    int x, y;
    SDL_PumpEvents();
    const Uint32 mState = SDL_GetMouseState(&x, &y);

    if(x > width || x < 0 || y < 0 || y > height){          // Checking if the mouse pointer is within the boundaries of the map
        return;
    }

    Point mouse(x, y);
    Point tl = (mouse - searchProximity);                   // tl is the top left point of the query range boundary
    Point rb = (mouse + searchProximity);                   // rb is the bottom right point of the query range boundary

    auto desired_locations = qt->inRange(tl, rb, query_terrain);        // This is the main query call using quad tree to find the locations in the query range

    for(auto d : desired_locations){
        map[transform(d->x / numOfCols, d->y / numOfRows)].is_glowing = true;       //Setting the is_glowing bool to true for the locations in range
    }

}

// If E is pressed, the mode is set to EDIT
void Map::setMode(){

    SDL_PumpEvents();
    const Uint8 *kState = SDL_GetKeyboardState(NULL);

    if(kState[SDL_SCANCODE_E]){
        mode = Mode::EDIT;
    }

}

// Main update function that handles the modes and their respective functions
void Map::Update(){

    SDL_PumpEvents();
    const Uint8 *kState = SDL_GetKeyboardState(NULL);
    
    // If 'R' and 'Left CTRL' are pressed together, the mode is set to IDLE
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

// UI to be rendered
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
int Map::transform(int i, int j)                            //Row-major ordering
{
    return (_w) * j + i;        //In 1D, (x,y) becomes y * width + x
}

int Map::transform(Point v)
{
    return transform(v.x, v.y);
}


Map::~Map(){
    delete qt;
}