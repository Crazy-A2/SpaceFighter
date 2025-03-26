#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

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

    if (!TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_Init Error: %s\n", TTF_GetError());
        isRunning = false;
    }

    // 初始化背景卷轴
    nearStars.texture = IMG_LoadTexture(renderer, std::format("{}/assets/image/Stars-A.png", PROJECT_DIR).c_str());
    if (nearStars.texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_LoadTexture Error: %s\n", IMG_GetError());
        isRunning = false;
    }
    SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
    nearStars.width /= 2;
    nearStars.height /= 2;
    farStars.texture = IMG_LoadTexture(renderer, std::format("{}/assets/image/Stars-B.png", PROJECT_DIR).c_str());
    if (farStars.texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_LoadTexture Error: %s\n", IMG_GetError());
        isRunning = false;
    }
    SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
    farStars.width /= 2;
    farStars.height /= 2;
    farStars.speed = 20;

    titleFont = TTF_OpenFont(std::format("{}/assets/font/VonwaonBitmap-16px.ttf", PROJECT_DIR).c_str(), 64);
    textFont = TTF_OpenFont(std::format("{}/assets/font/VonwaonBitmap-16px.ttf", PROJECT_DIR).c_str(), 32);
    if (titleFont == NULL || textFont == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont Error: %s\n", TTF_GetError());
        isRunning = false;
    }

    // 初始化当前场景
    changeScene(new SceneTitle());
}

void Game::run()
{
    // 主循环，当游戏正在运行时持续执行
    while (isRunning) {
        // 获取当前帧开始的时间（以毫秒为单位）
        auto frameStart = SDL_GetTicks();
        // 定义一个SDL事件结构体，用于处理事件
        SDL_Event event;
        // 处理事件，例如键盘输入、鼠标移动等
        handleEvent(&event);
        // 更新游戏状态，传入时间差deltaTime
        update(deltaTime);
        // 渲染游戏画面
        render();
        // 获取当前帧结束的时间（以毫秒为单位）
        auto frameEnd = SDL_GetTicks();
        // 计算当前帧的执行时间（以毫秒为单位）
        auto diff = frameEnd - frameStart;
        // 如果当前帧的执行时间小于目标帧时间（frameTime）
        if (diff < frameTime) {
            // 通过SDL_Delay函数暂停剩余的时间，以保证帧率稳定
            SDL_Delay(frameTime - diff);
            // 计算deltaTime为帧时间（以秒为单位）
            deltaTime = frameTime / 1000.0f;
        } else {
            // 如果当前帧的执行时间超过目标帧时间，则直接使用实际执行时间作为deltaTime
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
    TTF_Quit();

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

// 用于渲染水平居中文本
void Game::renderTextCentered(const std::string& text, float posY, bool isTitle)
{
    SDL_Color color = { 255, 255, 255, 255 }; // 白色
    SDL_Surface* surface {};
    if (isTitle) {
        surface = TTF_RenderUTF8_Solid(titleFont, text.c_str(), color);
    } else {
        surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), color);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int y { static_cast<int>((getWindowHeight() - surface->h) * posY) };
    SDL_Rect dstRect = { (getWindowWidth() - surface->w) / 2, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}