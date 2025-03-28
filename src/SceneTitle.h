#pragma once

#include "Game.h"
#include "Scene.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

class SceneTitle final : public Scene {
    Mix_Music* bgm {}; //  背景音乐
    float timer {}; //     计时器

public:
    SceneTitle() = default;
    ~SceneTitle() = default;

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;
};