#pragma once // 确保该头文件只被包含一次，防止重复定义

#include "SDL.h" // 包含SDL库的头文件，用于图形和多媒体处理

// 定义玩家结构体
struct Player {
    SDL_Texture* texture { nullptr }; // 玩家的纹理指针，初始化为nullptr
    SDL_FPoint position { 0, 0 }; // 玩家的位置，使用浮点数表示，初始化为(0, 0)
    int width { 0 }; // 玩家的宽度，初始化为0
    int height { 0 }; // 玩家的高度，初始化为0
    int speed { 400 }; // 玩家的速度，单位为像素/秒，初始化为400
    Uint32 coolDown { 0 }; // 玩家射击的冷却时间，单位为毫秒，初始化为0
    Uint32 lastShootTime { 0 }; // 玩家上次射击的时间，单位为毫秒，初始化为0
};

// 定义玩家射出的子弹结构体
struct ProjectilePlayer {
    SDL_Texture* texture { nullptr }; // 子弹的纹理指针，初始化为nullptr
    SDL_FPoint position { 0, 0 }; // 子弹的位置，使用浮点数表示，初始化为(0, 0)
    int width { 0 }; // 子弹的宽度，初始化为0
    int height { 0 }; // 子弹的高度，初始化为0
    int speed { 500 }; // 子弹的速度，单位为像素/秒，初始化为600
};
