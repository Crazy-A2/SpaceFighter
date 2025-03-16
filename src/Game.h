#pragma once

#include "Scene.h"
#include <SDL.h>

struct Game {
    Game(/* args */) = default;

    ~Game();

    void run();

    void init();

    void clean();

    void changeScene(Scene* scene);

    void handleEvent(SDL_Event* event);

    void update();

    void render();

private:
    bool isRunning { true };
    Scene* currentScene { nullptr };
    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };
    int windowWidth { 600 };
    int windowHeight { 800 };
};
