#include <iostream>
#include <SDL2/SDL.h>

#define WIDTH 1088
#define HEIGHT 612

int main(int argv, char** args) {

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "[ERROR] SDL Not Initialized !" << std::endl;
        return 0;
    }

    SDL_Window *window = SDL_CreateWindow("MAP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    if(window == nullptr){
        std::cout << "[ERROR] Couldn't Create a window" << std::endl;
        return 0;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == nullptr){
        std::cout << "[ERROR] Couldn't Create a renderer" << std::endl;
        return 0;
    }

    SDL_Event event;
    while(true){

        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
            case SDL_QUIT:
                return 0;
                break;
            default:
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

    }

}