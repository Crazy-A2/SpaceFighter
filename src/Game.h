#pragma once

#include "Object.h"
#include "Scene.h"
#include <SDL.h>

struct Game {
    ~Game();
    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);
    void handleEvent(SDL_Event* event);
    void update(float deltaTime);
    void render();
    void backgroundUpdate(float deltaTime);
    void renderBackground();

    static Game& getInstance()
    {
        static Game instance; // 单例模式
        return instance;
    }

    SDL_Renderer* getRenderer() { return renderer; }
    SDL_Window* getWindow() { return window; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }

private:
    bool isRunning { true };
    Scene* currentScene {};
    SDL_Window* window {};
    SDL_Renderer* renderer {};
    int windowWidth { 600 };
    int windowHeight { 800 };
    int FPS { 60 };
    Uint32 frameTime;
    float deltaTime;
    Background nearStars;
    Background farStars;

    Game() = default; // 私有化构造函数，防止外部创建对象
    // 禁止拷贝
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
};
