#pragma once

#include "Scene.h"
#include <SDL.h>

struct Game {
    Game(/* args */);

    ~Game();

    void run();

    void init();

    void clean();

    void changeScene(Scene* scene);

private:
    bool isRunning = true;
    Scene* currentScene = nullptr;
};
