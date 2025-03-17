#pragma once

#include <SDL_events.h>

// 场景接口，所有场景必须实现该接口
struct Scene {
    Scene() = default;
    virtual ~Scene() = default; // 基类此处必须为虚析构函数 否则派生类析构无法释放正确的资源

    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvents(SDL_Event* event) = 0;
};