#pragma once

#include "Object.h"
#include "Scene.h"

#include <SDL_events.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <list>
#include <map>
#include <random>

// #include "Game.h"
struct Game; // 不导入头文件 防止可能的循环引用

enum class SoundType {
    PLAYER_SHOOT,
    ENEMY_SHOOT,
    PLAYER_EXPLODE,
    ENEMY_EXPLODE,
    HIT,
    GET_ITEM,
};

// 定义一个名为 SceneMain 的结构体，继承自 Scene 类，并使用 final 关键字表示不允许被继承
struct SceneMain final : Scene {
    SceneMain();
    ~SceneMain();

    void init() override; //                            初始化场景
    void render() override; //                          渲染场景
    void clean() override; //                           清理场景资源
    void handleEvents(SDL_Event* event) override; //    处理事件
    void shootPlayer(); //                              处理玩家射击
    void renderPlayerProjectiles(); //                  渲染玩家子弹
    void spawEnemy(); //                                生成敌人
    void renderEnemies(); //                            渲染敌人
    void shootEnemy(Enemy* enemy); //                   处理敌人射击
    SDL_FPoint getDirection(Enemy* enemy); //           获取敌人射击方向
    void renderEnemyProjectiles(); //                   渲染敌人子弹
    void enemyExplode(Enemy* enemy); //                 敌人死亡
    void updatePlayer(float deltaTime); //              更新玩家状态
    void updateExplosions(float deltaTime); //          更新爆炸效果
    void renderExplosions(); //                         渲染爆炸效果
    void dropItem(Enemy* enemy); //                     敌人死亡后掉落道具
    void update(float deltaTime) override; //           更新场景状态，deltaTime 表示时间间隔
    void keyboardControl(float deltaTime); //           处理键盘控制，deltaTime 表示时间间隔
    void updatePlayerProjectiles(float deltaTime); //   计算玩家子弹相关逻辑 deltaTime 表示时间间隔
    void updateEnemies(float deltaTime); //             更新敌人逻辑 deltaTime 表示时间间隔
    void updateEnemyProjectiles(float deltaTime); //    计算敌人子弹相关逻辑 deltaTime 表示时间间隔
    void updateItems(float deltaTime); //               更新道具逻辑 deltaTime 表示时间间隔
    void playerGetItem(Item* item); //                  玩家拾取道具
    void renderItems(); //                              渲染道具
    void renderUI(); //                                 渲染UI

private:
    Game& game; //                                      引用 Game 对象，用于访问游戏状态和资源
    Player player; //                                   玩家对象
    bool isDead {}; //                                  用于表示玩家是否死亡
    Mix_Music* bgm; //                                  定义一个音乐对象，用于播放背景音乐
    SDL_Texture* uiHealth; //                           定义一个纹理对象，用于显示玩家生命值
    std::mt19937 gen; // 定义一个随机数生成器对象 gen，使用 Mersenne Twister 19937 算法
    std::uniform_real_distribution<float> dis; // 定义一个均匀分布的对象 dis，用于生成浮点数
    TTF_Font* scoreFont; //                             定义一个字体对象，用于渲染文本
    int score {}; //                                    得分

    Explosion explosionTemplate; //                     定义一个爆炸效果的模板
    ProjectileEnemy projectileEnemyTemplate; //         定义敌人子弹的模板对象
    Enemy enemyTemplate; //                             定义一个敌人的模板对象
    ProjectilePlayer projectilePlayerTemplate; //       定义玩家子弹的模板对象
    Item itemLifeTemplate; //                           定义一个道具的模板对象

    std::list<Explosion*> explosions; //                存储爆炸效果的列表
    std::list<ProjectileEnemy*> projectilesEnemy; //    存储敌人子弹对象的子弹列表
    std::list<Enemy*> enemies; //                       存储敌人对象的列表
    std::list<ProjectilePlayer*> projectilesPlayer; //  存储玩家子弹对象的子弹列表
    std::list<Item*> items; //                          存储道具对象的列表
    std::map<SoundType, Mix_Chunk*> sounds; //          存储音效
};
