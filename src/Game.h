#pragma once

#include "Object.h"
// #include "Scene.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
// #include <SDL_mixer.h>

#include <map>
#include <string>

struct Scene;

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
    SDL_Point renderTextCentered(const std::string& text, float posY, bool isTitle);
    void renderTextPos(const std::string& text, int posX, int posY, bool isLeft = true);
    void insertLeaderBoard(int score, const std::string name);
    std::multimap<int, std::string, std::greater<int>>& getLeaderBoard() { return leaderBoard; }

    static Game& getInstance()
    {
        static Game instance; // 单例模式
        return instance;
    }

    SDL_Renderer* getRenderer() { return renderer; }
    SDL_Window* getWindow() { return window; }
    int getWindowWidth() { return windowWidth; }
    int getWindowHeight() { return windowHeight; }
    int getFinalScore() { return finalScore; }
    void setFinalScore(int score) { finalScore = score; }

private:
    bool isRunning { true };
    Scene* currentScene {};
    SDL_Window* window {};
    SDL_Renderer* renderer {};
    int windowWidth { 600 };
    int windowHeight { 800 };
    int FPS { 60 };
    Uint32 frameTime;
    float deltaTime {};
    Background nearStars;
    Background farStars;
    TTF_Font* titleFont {};
    TTF_Font* textFont {};
    int finalScore {};
    // 存储排行榜数据，键为分数，值为玩家名字，按分数从高到低排序
    std::multimap<int, std::string, std::greater<int>> leaderBoard;

    Game() = default; // 私有化构造函数，防止外部创建对象
    // 禁止拷贝
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
};
