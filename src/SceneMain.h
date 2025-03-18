#pragma once

#include "Object.h"
#include "Scene.h"
#include <SDL_events.h>
#include <list>
#include <random>

// #include "Game.h"
struct Game; // 不导入头文件 防止可能的循环引用

// 定义一个名为 SceneMain 的结构体，继承自 Scene 类，并使用 final 关键字表示不允许被继承
struct SceneMain final : Scene {
    SceneMain();
    ~SceneMain();

    void init() override; // 初始化场景
    void update(float deltaTime) override; // 更新场景状态，deltaTime 表示时间间隔
    void render() override; // 渲染场景
    void clean() override; // 清理场景资源
    void handleEvents(SDL_Event* event) override; // 处理事件
    void keyboardControl(float deltaTime); // 处理键盘控制，deltaTime 表示时间间隔
    void shootPlayer(); // 处理玩家射击
    void updatePlayerProjectiles(float deltaTime); // 计算玩家子弹相关逻辑 deltaTime 表示时间间隔
    void renderPlayerProjectiles(); // 渲染玩家子弹
    void spawEnemy(); // 生成敌人
    void updateEnemies(float deltaTime); // 更新敌人逻辑 deltaTime 表示时间间隔
    void renderEnemies(); // 渲染敌人
    void shootEnemy(Enemy* enemy); // 处理敌人射击
    SDL_FPoint getDirection(Enemy* enemy); // 获取敌人射击方向
    void updateEnemyProjectiles(float deltaTime); // 计算敌人子弹相关逻辑 deltaTime 表示时间间隔
    void renderEnemyProjectiles(); // 渲染敌人子弹
    void enemyExplode(Enemy* enemy); // 敌人死亡

private:
    Game& game; // 引用 Game 对象，用于访问游戏状态和资源
    Player player; // 玩家对象
    ProjectilePlayer projectilePlayerTemplate; // 定义玩家子弹的模板对象
    std::list<ProjectilePlayer*> projectilesPlayer; // 存储玩家子弹对象的子弹列表
    Enemy enemyTemplate; // 定义一个敌人的模板对象
    std::list<Enemy*> enemies; // 存储敌人对象的列表
    ProjectileEnemy projectileEnemyTemplate; // 定义敌人子弹的模板对象
    std::list<ProjectileEnemy*> projectilesEnemy; // 存储敌人子弹对象的子弹列表
    std::mt19937 gen; // 定义一个随机数生成器对象 gen，使用 Mersenne Twister 19937 算法
    std::uniform_real_distribution<float> dis; // 定义一个均匀分布的对象 dis，用于生成浮点数
};
