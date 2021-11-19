#include <iostream>

#include "SDL.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "map.h"

#define WIDTH 1100
#define HEIGHT 600

#define V_WIDTH WIDTH - 250
#define V_HEIGHT HEIGHT

int main(int argv, char** args) {

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "[ERROR] SDL Not Initialized !" << std::endl;
        return 0;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("MAP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, window_flags);
    if(window == nullptr){
        std::cout << "[ERROR] Couldn't Create a window" << std::endl;
        return 0;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == nullptr){
        std::cout << "[ERROR] Couldn't Create a renderer" << std::endl;
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, V_WIDTH, V_HEIGHT);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer);

    Map map(WIDTH - 250, HEIGHT, 10, 10);

    SDL_Event event;
    bool is_running = true;
    while(is_running){

        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                is_running = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                is_running = false;
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // Create a Docking Space
        // {
            ImGui::SetNextWindowPos(ImVec2(0, 0));                                                // always at the window origin
            ImGui::SetNextWindowSize(ImVec2(float(WIDTH), float(HEIGHT)));    // always at the window size

            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBringToFrontOnFocus |                 // we just want to use this window as a host for the menubar and docking
                ImGuiWindowFlags_NoNavFocus |                                                      // so turn off everything that would make it act like a window
                ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoBackground;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            bool show = (ImGui::Begin("Main", NULL, windowFlags));
            ImGui::PopStyleVar();
            ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f),  ImGuiDockNodeFlags_PassthruCentralNode);
            ImGui::End();
        // }

        // Displaying the Texture
        // {
            // ImGui::SetNextWindowSize(ImVec2(float(V_WIDTH), float(V_HEIGHT)));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            // ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse
            ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse);
            ImGui::PopStyleVar();
            ImVec2 viewportSize = ImGui::GetWindowSize();
            ImGui::Image((void*)texture, viewportSize);
            ImGui::End();
        // }

        // Renderer to Texture
        SDL_SetRenderTarget(renderer, texture);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        map.UI();
        map.Draw(renderer);
        map.Update();

        SDL_SetRenderTarget(renderer, NULL);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);

    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}
