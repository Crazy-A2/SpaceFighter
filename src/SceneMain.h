#pragma once

#include "Object.h"
#include "Scene.h"
#include <SDL_events.h>

// #include "Game.h"
struct Game; // 不导入头文件 防止可能的循环引用

struct SceneMain final : Scene {
    SceneMain();
    ~SceneMain();

    void init() override;
    void update() override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;

private:
    Game& game;
    Player player;
};
