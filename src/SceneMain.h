#pragma once

#include "Scene.h"
#include <SDL_events.h>

struct SceneMain : Scene {
    /* data */
    SceneMain();
    ~SceneMain();

    void init() override;
    void update() override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;
};
