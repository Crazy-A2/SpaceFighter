#pragma once

#include "Object.h"
#include "Scene.h"
#include <SDL_events.h>
#include <list>

// #include "Game.h"
struct Game; // 不导入头文件 防止可能的循环引用

// 定义一个名为 SceneMain 的结构体，继承自 Scene 类，并使用 final 关键字表示不允许被继承
struct SceneMain final : Scene {
    // 构造函数，用于初始化 SceneMain 对象
    SceneMain();
    // 析构函数，用于清理 SceneMain 对象
    ~SceneMain();

    // 重写 Scene 类的 init 方法，用于初始化场景
    void init() override;
    // 重写 Scene 类的 update 方法，用于更新场景状态，deltaTime 表示时间间隔
    void update(float deltaTime) override;
    // 重写 Scene 类的 render 方法，用于渲染场景
    void render() override;
    // 重写 Scene 类的 clean 方法，用于清理场景资源
    void clean() override;
    // 重写 Scene 类的 handleEvents 方法，用于处理事件，event 指向 SDL_Event 结构体
    void handleEvents(SDL_Event* event) override;
    // 处理键盘控制，deltaTime 表示时间间隔
    void keyboardControl(float deltaTime);
    // 处理玩家射击
    void shootPlayer();
    // 更新玩家投射物
    // 参数：deltaTime - 时间增量，通常用于表示自上次更新以来的时间间隔
    void updatePlayerProjectiles(float deltaTime);
    // 渲染玩家投射物
    void renderPlayerProjectiles();

private:
    // 引用 Game 对象，用于访问游戏状态和资源
    Game& game;
    // 玩家对象
    Player player;
    // 玩家投射物模板对象
    ProjectilePlayer projectilePlayerTemplate;
    // 存储玩家投射物对象的列表
    std::list<ProjectilePlayer*> projectilesPlayer;
};
