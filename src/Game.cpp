#include "Game.h"
#include "SceneMain.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <format>
#include <iostream>

Game::~Game() { clean(); }

void Game::init()
{
    frameTime = 1000 / FPS; // 每帧的时间间隔，单位为毫秒

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

    // SDL_Mixer 初始化
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_Init Error: %s\n", Mix_GetError());
        isRunning = false;
    }
    // 打开音频设备
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_OpenAudio Error: %s\n", Mix_GetError());
        isRunning = false;
    }
    Mix_AllocateChannels(32); // 分配32个音频通道

    // 初始化背景卷轴
    nearStars.texture = IMG_LoadTexture(renderer, std::format("{}/assets/image/Stars-A.png", PROJECT_DIR).c_str());
    SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
    nearStars.width /= 2;
    nearStars.height /= 2;
    farStars.texture = IMG_LoadTexture(renderer, std::format("{}/assets/image/Stars-B.png", PROJECT_DIR).c_str());
    SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
    farStars.width /= 2;
    farStars.height /= 2;
    farStars.speed = 20;

    // 初始化当前场景
    changeScene(new SceneMain());
}

void Game::run()
{
    while (isRunning) {
        auto frameStart = SDL_GetTicks();
        SDL_Event event;
        handleEvent(&event);
        update(deltaTime);
        render();
        auto frameEnd = SDL_GetTicks();
        auto diff = frameEnd - frameStart;
        if (diff < frameTime) {
            SDL_Delay(frameTime - diff);
            deltaTime = frameTime / 1000.0f;
        } else {
            deltaTime = diff / 1000.0f;
        }
    }
}

void Game::clean()
{
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }
    if (nearStars.texture != nullptr) {
        SDL_DestroyTexture(nearStars.texture);
    }
    if (farStars.texture != nullptr) {
        SDL_DestroyTexture(farStars.texture);
    }

    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();

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

    // 将传入的新场景指针赋值给currentScene，更换当前场景
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

void Game::update(float deltaTime)
{
    backgroundUpdate(deltaTime);
    currentScene->update(deltaTime);
}

void Game::render()
{
    SDL_RenderClear(renderer); //   清空
    renderBackground(); //          渲染背景
    currentScene->render(); //      渲染
    SDL_RenderPresent(renderer); // 显示更新
}

void Game::backgroundUpdate(float deltaTime)
{
    nearStars.offset += nearStars.speed * deltaTime;
    if (nearStars.offset >= 0) {
        nearStars.offset -= nearStars.height;
    }

    farStars.offset += farStars.speed * deltaTime;
    if (farStars.offset >= 0) {
        farStars.offset -= farStars.height;
    }
}

void Game::renderBackground()
{
    // 渲染远景
    for (int posY { static_cast<int>(farStars.offset) }; posY < getWindowHeight(); posY += farStars.height) {
        for (int posX = 0; posX < getWindowWidth(); posX += farStars.width) {
            SDL_Rect dstRect = { posX, posY, farStars.width, farStars.height };
            SDL_RenderCopy(renderer, farStars.texture, NULL, &dstRect);
        }
    }

    // 渲染近景
    for (int posY { static_cast<int>(nearStars.offset) }; posY < getWindowHeight(); posY += nearStars.height) {
        for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width) {
            SDL_Rect dstRect = { posX, posY, nearStars.width, nearStars.height };
            SDL_RenderCopy(renderer, nearStars.texture, NULL, &dstRect);
        }
    }
}