#pragma once // 确保该头文件只被包含一次，防止重复定义

#include "SDL.h" // 包含SDL库的头文件，用于图形和多媒体处理

// 定义玩家结构体
struct Player {
    SDL_Texture* texture { nullptr }; // 玩家的纹理指针
    SDL_FPoint position { 0, 0 }; // 玩家的位置
    int width { 0 }; // 玩家的宽度
    int height { 0 }; // 玩家的高度
    int speed { 400 }; // 玩家的速度
    Uint32 coolDown { 200 }; // 玩家射击的冷却时间，单位为毫秒
    Uint32 lastShootTime { 0 }; // 玩家上次射击的时间，单位为毫秒
    int currentHealth { 3 }; // 玩家的当前生命值
};

// 定义玩家射出的子弹结构体
struct ProjectilePlayer {
    SDL_Texture* texture { nullptr }; // 子弹的纹理指针
    SDL_FPoint position { 0, 0 }; // 子弹的位置
    int width { 0 }; // 子弹的宽度
    int height { 0 }; // 子弹的高度
    int speed { 500 }; // 子弹的速度
    int damage { 1 }; // 子弹的伤害值
};

// 定义游戏中的敌人结构体
struct Enemy {
    SDL_Texture* texture { nullptr }; // 敌人的纹理指针
    SDL_FPoint position { 0, 0 }; // 敌人的位置
    int width { 0 }; // 敌人的宽度
    int height { 0 }; // 敌人的高度
    int speed { 300 }; // 敌人的移动速度
    Uint32 coolDown { 1000 }; // 敌人射击的冷却时间，单位为毫秒
    Uint32 lastShootTime { 0 }; // 敌人上次射击的时间，单位为毫秒
    int currentHealth { 2 }; // 敌人的当前生命值
};

// 定义敌人射出的子弹结构体
struct ProjectileEnemy {
    SDL_Texture* texture { nullptr }; // 子弹的纹理指针
    SDL_FPoint position { 0, 0 }; // 子弹的位置
    SDL_FPoint direction { 0, 0 }; // 子弹的方向
    int width { 0 }; // 子弹的宽度
    int height { 0 }; // 子弹的高度
    int speed { 500 }; // 子弹的速度
    int damage { 1 }; // 子弹的伤害值
};
