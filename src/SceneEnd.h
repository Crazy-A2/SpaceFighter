#pragma once

#include "Scene.h"

class SceneEnd : public Scene {
public:
    SceneEnd() = default;
    ~SceneEnd() = default;

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;
};