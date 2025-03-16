#include "Game.h"
#include "SceneMain.h"
#include <SDL_image.h>
#include <iostream>

Game::~Game() { clean(); }

void Game::init()
{
    // SDL 初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s\n", SDL_GetError());
        isRunning = false;
    };

    // 创建窗口
    window = SDL_CreateWindow(
        "太空战机",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    // SDL_Image 初始化
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s\n", IMG_GetError());
        isRunning = false;
    }

    // 初始化当前场景
    // currentScene = new SceneMain();
    changeScene(new SceneMain());
}

void Game::run()
{
    while (isRunning) {
        SDL_Event event;
        handleEvent(&event);
        update();
        render();
    }
}

void Game::clean()
{
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }

    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// 定义Game类中的changeScene成员函数，用于切换游戏场景
void Game::changeScene(Scene* scene)
{
    // 检查当前场景是否不为空
    if (currentScene != nullptr) {
        // 调用当前场景的clean函数，进行清理工作，例如释放资源
        currentScene->clean();
        // 删除当前场景对象，释放内存
        delete currentScene;
    }

    // 将传入的新场景指针赋值给currentScene，更新当前场景
    currentScene = scene;
    // 调用新场景的init函数，进行初始化工作，例如加载资源
    currentScene->init();
}

void Game::handleEvent(SDL_Event* event)
{
    // 检查是否有事件发生
    // 如果有，则处理退出之外的事件
    while (SDL_PollEvent(event)) {
        // 如果事件类型是退出事件（SDL_QUIT），则设置游戏运行状态为false
        if (event->type == SDL_QUIT) {
            isRunning = false;
        }
        // 调用当前场景的handleEvents方法处理其他事件
        currentScene->handleEvents(event);
    }
}

void Game::update()
{
    currentScene->update();
}

void Game::render()
{
    // 清空
    SDL_RenderClear(renderer);
    // 渲染
    currentScene->render();
    // 显示更新
    SDL_RenderPresent(renderer);
}
