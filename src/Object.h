#pragma once // 确保该头文件只被包含一次，防止重复定义

#include "SDL.h" // 包含SDL库的头文件，用于图形和多媒体处理

// 定义玩家结构体
struct Player {
    SDL_Texture* texture {}; //         玩家的纹理指针
    SDL_FPoint position { 0, 0 }; //    玩家的位置
    int width {}; //                    玩家的宽度
    int height {}; //                   玩家的高度
    int speed { 400 }; //               玩家的速度
    Uint32 coolDown { 200 }; //         玩家射击的冷却时间，单位为毫秒
    Uint32 lastShootTime {}; //         玩家上次射击的时间，单位为毫秒
    int currentHealth { 3 }; //         玩家的当前生命值
    int maxHealth { 3 }; //             玩家的最大生命值
};

// 定义玩家射出的子弹结构体
struct ProjectilePlayer {
    SDL_Texture* texture {}; //         子弹的纹理指针
    SDL_FPoint position { 0, 0 }; //    子弹的位置
    int width {}; //                    子弹的宽度
    int height {}; //                   子弹的高度
    int speed { 500 }; //               子弹的速度
    int damage { 1 }; //                子弹的伤害值
};

// 定义游戏中的敌人结构体
struct Enemy {
    SDL_Texture* texture {}; //         敌人的纹理指针
    SDL_FPoint position { 0, 0 }; //    敌人的位置
    int width {}; //                    敌人的宽度
    int height {}; //                   敌人的高度
    int speed { 300 }; //               敌人的移动速度
    Uint32 coolDown { 2000 }; //        敌人射击的冷却时间，单位为毫秒
    Uint32 lastShootTime {}; //         敌人上次射击的时间，单位为毫秒
    int currentHealth { 2 }; //         敌人的当前生命值
};

// 定义敌人射出的子弹结构体
struct ProjectileEnemy {
    SDL_Texture* texture {}; //         子弹的纹理指针
    SDL_FPoint position { 0, 0 }; //    子弹的位置
    SDL_FPoint direction { 0, 0 }; //   子弹的方向
    int width {}; //                    子弹的宽度
    int height {}; //                   子弹的高度
    int speed { 500 }; //               子弹的速度
    int damage { 1 }; //                子弹的伤害值
};

// 定义爆炸效果结构体
struct Explosion {
    SDL_Texture* texture {}; //         爆炸的纹理指针
    SDL_FPoint position { 0, 0 }; //    爆炸的位置
    int width {}; //                    爆炸的宽度
    int height {}; //                   爆炸的高度
    int currentFrame {}; //             爆炸的当前帧
    int totalFrames { 8 }; //           爆炸的总帧数
    Uint32 startTime {}; //             爆炸开始的时间，单位为毫秒
    Uint32 FPS { 10 }; //               爆炸的帧率，单位为帧/秒
};

// 定义游戏中的道具类型
enum class ItemType {
    Life,
    Shield,
    Time,
};

// 定义游戏中的道具结构体
struct Item {
    SDL_Texture* texture {}; //             道具的纹理指针
    SDL_FPoint position { 0, 0 }; //        道具的位置
    SDL_FPoint direction { 0, 0 }; //       道具的移动方向
    int width {}; //                        道具的宽度
    int height {}; //                       道具的高度
    ItemType type { ItemType::Life }; //    道具的具体类型
    int speed { 200 }; //                   道具的移动速度
    int bounceCount { 3 }; //               道具的反弹次数
};

struct Background {
    SDL_Texture* texture {}; //             背景的纹理指针
    SDL_FPoint position { 0, 0 }; //        背景的位置
    int width {}; //                        背景的宽度
    int height {}; //                       背景的高度
    int speed { 30 }; //                    背景的移动速度
    float offset {}; //                     背景的偏移量
};