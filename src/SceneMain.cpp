#include "SceneMain.h"
#include "Game.h"
#include "SceneTitle.h"
#include <SDL_image.h>
#include <format>
#include <iostream>
#include <random>

SceneMain::~SceneMain() { }

using std::format;

template <typename T>
static void initTextureByTemplate(Game& game, T& object, const char* imageName, bool isSquare = false)
{
    object.texture = IMG_LoadTexture(game.getRenderer(), format("{}/assets/{}", PROJECT_DIR, imageName).c_str());
    SDL_QueryTexture(object.texture, NULL, NULL, &object.width, &object.height);
    // 将图片纹理的宽度和高度缩小为原来的 1 / 4
    if (!isSquare) {
        object.height /= 4;
    }
    object.width = isSquare ? object.height : (object.width / 4);
}

// 定义SceneMain类的init成员函数，用于初始化场景
void SceneMain::init()
{
    // 读取并播放背景音乐
    bgm = Mix_LoadMUS(format("{}/assets/music/03_Racing_Through_Asteroids_Loop.ogg", PROJECT_DIR).c_str());
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1); // -1 表示循环播放

    uiHealth = IMG_LoadTexture(game.getRenderer(), format("{}/assets/image/Health UI Black.png", PROJECT_DIR).c_str());

    scoreFont = TTF_OpenFont(format("{}/assets/font/VonwaonBitmap-12px.ttf", PROJECT_DIR).c_str(), 24);

    // 读取音效
    sounds[SoundType::PLAYER_SHOOT] = Mix_LoadWAV(format("{}/assets/sound/laser_shoot4.wav", PROJECT_DIR).c_str());
    sounds[SoundType::ENEMY_SHOOT] = Mix_LoadWAV(format("{}/assets/sound/xs_laser.wav", PROJECT_DIR).c_str());
    sounds[SoundType::PLAYER_EXPLODE] = Mix_LoadWAV(format("{}/assets/sound/explosion2.wav", PROJECT_DIR).c_str());
    sounds[SoundType::ENEMY_EXPLODE] = Mix_LoadWAV(format("{}/assets/sound/explosion3.wav", PROJECT_DIR).c_str());
    sounds[SoundType::GET_ITEM] = Mix_LoadWAV(format("{}/assets/sound/eff11.wav", PROJECT_DIR).c_str());
    sounds[SoundType::HIT] = Mix_LoadWAV(format("{}/assets/sound/eff5.wav", PROJECT_DIR).c_str());

    // 初始化随机数生成器
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    initTextureByTemplate(game, player, "image/SpaceShip.png");

    // 设置玩家飞船的初始位置
    // 横坐标为窗口宽度的一半减去飞船宽度的一半，使飞船水平居中
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    // 纵坐标为窗口高度减去飞船的高度，使飞船位于窗口底部
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化模板
    initTextureByTemplate(game, projectilePlayerTemplate, "image/laser-3.png");
    initTextureByTemplate(game, enemyTemplate, "image/insect-2.png");
    initTextureByTemplate(game, projectileEnemyTemplate, "image/laser-2.png");
    initTextureByTemplate(game, explosionTemplate, "effect/explosion.png", true);
    initTextureByTemplate(game, itemLifeTemplate, "image/bonus_life.png");
    // 设置爆炸动画的帧数 防止序列动画文件被替换后导致帧数不匹配
    explosionTemplate.totalFrames = explosionTemplate.width / explosionTemplate.height;
}

// 更新游戏主场景中的状态
void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime); // 根据键盘输入更新玩家状态
    updatePlayerProjectiles(deltaTime); // 更新玩家发射的子弹状态
    updateEnemyProjectiles(deltaTime); // 更新敌人发射的子弹状态
    spawEnemy(); // 生成新的敌人
    updateEnemies(deltaTime); // 更新敌人状态
    updatePlayer(deltaTime); // 更新玩家状态
    updateExplosions(deltaTime); // 更新爆炸效果
    updateItems(deltaTime); // 更新道具
}

// 定义SceneMain类的render函数，用于渲染场景中的主要内容
void SceneMain::render()
{
    renderPlayerProjectiles(); // 渲染玩家发射的子弹
    renderEnemyProjectiles(); // 渲染敌人

    if (!isDead) {
        // 创建一个 SDL_Rect 结构体，用于存储玩家飞船的起始位置和宽高
        // player.position.x 和 player.position.y 分别表示绘制玩家飞船的起始位置坐标（左上角）
        // player.width 和 player.height 分别表示玩家飞船的宽度和高度
        SDL_Rect playerRect {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width, player.height
        };
        // 使用SDL_RenderCopy函数将玩家的纹理贴图渲染到屏幕上
        // game.getRenderer() 获取当前的SDL渲染器
        // player.texture 表示玩家的纹理贴图
        // NULL 表示源纹理的区域为整个纹理
        // &playerRect 表示目标纹理的区域，即玩家在屏幕上的位置和大小
        SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    }

    renderEnemies(); //     渲染敌人
    renderItems(); //       渲染道具
    renderExplosions(); //  渲染爆炸动画
    renderUI(); //          渲染UI
}

// 清理容器
template <typename T>
static void cleanContainer(std::list<T*>& container)
{
    for (auto& item : container) {
        if (item != nullptr) {
            delete item;
        }
    }
    container.clear();
}

// 清理模板
template <typename T>
static void destroyTextureTemplate(T& object)
{
    if (object.texture != nullptr) {
        SDL_DestroyTexture(object.texture);
    }
}

void SceneMain::clean()
{
    for (auto& sound : sounds) {
        if (sound.second != nullptr) {
            Mix_FreeChunk(sound.second);
        }
    }
    sounds.clear();

    cleanContainer(projectilesPlayer);
    cleanContainer(enemies);
    cleanContainer(projectilesEnemy);
    cleanContainer(explosions);
    cleanContainer(items);

    // 释放UI纹理占用的内存
    if (uiHealth != nullptr) {
        SDL_DestroyTexture(uiHealth);
    }

    // 释放字体内存
    if (scoreFont != nullptr) {
        TTF_CloseFont(scoreFont);
    }

    destroyTextureTemplate(player);
    destroyTextureTemplate(projectilePlayerTemplate);
    destroyTextureTemplate(enemyTemplate);
    destroyTextureTemplate(projectileEnemyTemplate);
    destroyTextureTemplate(explosionTemplate);
    destroyTextureTemplate(itemLifeTemplate);

    // 停止播放背景音乐并释放内存
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneMain::handleEvents(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        game.changeScene(new SceneTitle());
    }
}

// SceneMain类的成员函数，用于处理键盘控制
void SceneMain::keyboardControl(float deltaTime)
{
    if (isDead) {
        return;
    }

    // 获取当前键盘状态
    auto keyboardState = SDL_GetKeyboardState(NULL);
    // 检查左键是否按下，并且玩家位置x坐标大于等于0
    if (keyboardState[SDL_SCANCODE_LEFT] && player.position.x >= 0) {
        // 玩家向左移动，位置x坐标减去deltaTime乘以速度
        player.position.x -= deltaTime * player.speed;
    }
    // 检查右键是否按下，并且玩家位置x坐标小于窗口宽度减去玩家宽度
    if (keyboardState[SDL_SCANCODE_RIGHT] && player.position.x <= game.getWindowWidth() - player.width) {
        // 玩家向右移动，位置x坐标加上deltaTime乘以速度
        player.position.x += deltaTime * player.speed;
    }
    // 检查上键是否按下，并且玩家位置y坐标大于等于0
    if (keyboardState[SDL_SCANCODE_UP] && player.position.y >= 0) {
        // 玩家向上移动，位置y坐标减去deltaTime乘以速度
        player.position.y -= deltaTime * player.speed;
    }
    // 检查下键是否按下，并且玩家位置y坐标小于窗口高度减去玩家高度
    if (keyboardState[SDL_SCANCODE_DOWN] && player.position.y <= game.getWindowHeight() - player.height) {
        // 玩家向下移动，位置y坐标加上deltaTime乘以速度
        player.position.y += deltaTime * player.speed;
    }

    // 控制子弹发射
    if (keyboardState[SDL_SCANCODE_SPACE]) {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime > player.coolDown) {
            shootPlayer();
            player.lastShootTime = currentTime;
        }
    }
}

void SceneMain::shootPlayer()
{
    // 发射子弹
    auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
    // 初始化子弹的位置
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    projectilesPlayer.push_back(projectile); // 将子弹添加到子弹列表中
    Mix_PlayChannel(0, sounds[SoundType::PLAYER_SHOOT], 0); // 播放射击音效
}

// SceneMain类的成员函数，用于更新玩家发射的投射物
void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    // 使用迭代器遍历玩家投射物列表
    for (auto iterator = projectilesPlayer.begin(); iterator != projectilesPlayer.end();) {
        // 获取当前迭代器指向的投射物对象
        auto projectile = *iterator;
        // 更新投射物的位置，沿y轴向下移动，移动距离为速度乘以时间差
        projectile->position.y -= projectile->speed * deltaTime;
        // 检查投射物是否超出屏幕范围（y坐标小于其高度负值）
        if (projectile->position.y < -projectile->height) {
            // 如果超出范围，释放投射物对象的内存
            delete projectile;
            // 从列表中移除该投射物，并返回新的迭代器位置
            iterator = projectilesPlayer.erase(iterator);
            // SDL_Log("子弹被移除了...");
        } else {
            bool hit = false;
            // 遍历敌人列表，检查是否有敌人被击中
            for (auto enemy : enemies) {
                // 创建投射物和敌人的矩形区域
                SDL_Rect projectileRect {
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y),
                    projectile->width,
                    projectile->height
                };
                SDL_Rect enemyRect {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height
                };

                // 检查投射物和敌人是否相交
                if (SDL_HasIntersection(&projectileRect, &enemyRect)) {
                    // 如果相交，减少敌人的当前生命值
                    enemy->currentHealth -= projectile->damage;
                    // 释放投射物对象的内存
                    delete projectile;
                    // 从列表中移除该投射物，并返回新的迭代器位置
                    iterator = projectilesPlayer.erase(iterator);
                    hit = true;
                    Mix_PlayChannel(0, sounds[SoundType::HIT], 0); // 播放敌人被击中音效
                    // 跳出敌人列表的循环
                    break;
                }
            }
            // 如果投射物未击中任何敌人，继续遍历下一个投射物
            if (!hit) {
                ++iterator;
            }
        }
    }
}

// 定义SceneMain类的成员函数renderPlayerProjectiles，用于渲染玩家发射的投射物
void SceneMain::renderPlayerProjectiles()
{
    // 遍历projectilesPlayer容器中的每一个投射物
    for (auto projectile : projectilesPlayer) {
        // 创建一个SDL_Rect结构体，用于定义投射物的矩形区域
        SDL_Rect projectileRect {
            // 将投射物的x坐标转换为整数并赋值给矩形区域的x坐标
            static_cast<int>(projectile->position.x),
            // 将投射物的y坐标转换为整数并赋值给矩形区域的y坐标
            static_cast<int>(projectile->position.y),
            // 将投射物的宽度赋值给矩形区域的宽度
            projectile->width,
            // 将投射物的高度赋值给矩形区域的高度
            projectile->height
        };
        // 使用SDL_RenderCopy函数将投射物的纹理绘制到渲染器上
        // 参数1：渲染器，通过game对象的getRenderer方法获取
        // 参数2：投射物的纹理
        // 参数3：源矩形，这里传入NULL表示使用整个纹理
        // 参数4：目标矩形，即projectileRect，定义了纹理在屏幕上的位置和大小
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}

void SceneMain::spawEnemy()
{
    if (dis(gen) > 1 / 60.0f) {
        return;
    }
    Enemy* enemy = new Enemy(enemyTemplate);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}

// SceneMain类的成员函数，用于更新敌人的状态
void SceneMain::updateEnemies(float deltaTime)
{
    // 获取当前时间，单位为毫秒
    auto currentTime = SDL_GetTicks();
    // 遍历敌人列表
    for (auto iterator = enemies.begin(); iterator != enemies.end();) {
        // 获取当前迭代器指向的敌人对象
        auto enemy = *iterator;
        // 更新敌人的位置，根据敌人的速度和时间的增量计算新的y坐标
        enemy->position.y += enemy->speed * deltaTime;
        // 检查敌人是否超出窗口的底部边界
        if (enemy->position.y > game.getWindowHeight()) {
            // 如果超出边界，删除该敌人对象
            delete enemy;
            // 从敌人列表中移除该敌人，并更新迭代器指向下一个有效元素
            iterator = enemies.erase(iterator);
        } else {
            // 如果未超出边界，检查敌人的冷却时间是否已过
            if (currentTime - enemy->lastShootTime > enemy->coolDown && !isDead) {
                // 如果冷却时间已过，调用shootEnemy函数让敌人射击
                shootEnemy(enemy);
                // 更新敌人的最后射击时间为当前时间
                enemy->lastShootTime = currentTime;
            }

            SDL_Rect enemyRect {
                static_cast<int>(enemy->position.x),
                static_cast<int>(enemy->position.y),
                enemy->width, enemy->height
            };
            SDL_Rect playerRect {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width, player.height
            };
            // 检查敌人是否与玩家碰撞
            if (SDL_HasIntersection(&enemyRect, &playerRect)) {
                // 如果碰撞，调用playerHit函数处理玩家被击中
                player.currentHealth -= 1;
                enemy->currentHealth = 0;
            }

            // 检查敌人的当前生命值是否小于等于0
            if (enemy->currentHealth <= 0) {
                // 如果生命值小于等于0，调用enemyExplode函数处理敌人爆炸
                enemyExplode(enemy);
                // 从敌人列表中移除该敌人，并更新迭代器指向下一个有效元素
                iterator = enemies.erase(iterator);
                Mix_PlayChannel(-1, sounds[SoundType::ENEMY_EXPLODE], 0);
            } else {
                // 如果生命值大于0，迭代器指向下一个敌人
                ++iterator;
            }
        }
    }
}

void SceneMain::renderEnemies()
{
    // 渲染敌人
    for (auto& enemy : enemies) {
        SDL_Rect enemyRect {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width, enemy->height
        };
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}

void SceneMain::shootEnemy(Enemy* enemy)
{
    auto projectile = new ProjectileEnemy(projectileEnemyTemplate);
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;
    projectile->direction = getDirection(enemy);
    projectilesEnemy.push_back(projectile);
    Mix_PlayChannel(-1, sounds[SoundType::ENEMY_SHOOT], 0);
}

// 定义SceneMain类中的getDirection方法，用于计算从敌人到玩家的方向向量
SDL_FPoint SceneMain::getDirection(Enemy* enemy)
{
    auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    auto length = sqrt(x * x + y * y); // 计算向量长度
    x /= length; // 将x分量除以向量长度，得到单位向量的x分量
    y /= length; // 将y分量除以向量长度，得到单位向量的y分量
    return SDL_FPoint { x, y };
}

// 更新敌人发射的导弹状态
void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    // 定义一个边界值margin，用于判断导弹是否超出窗口边界
    auto margin = 32;

    // 使用迭代器遍历projectilesEnemy容器中的所有导弹
    for (auto iterator = projectilesEnemy.begin(); iterator != projectilesEnemy.end();) {
        // 获取当前迭代器指向的导弹对象
        auto projectile = *iterator;
        // 更新导弹的x坐标位置，根据导弹的速度、方向和经过的时间计算
        projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
        // 更新导弹的y坐标位置，根据导弹的速度、方向和经过的时间计算
        projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;
        // 判断导弹是否超出窗口边界
        if (projectile->position.y > game.getWindowHeight() + margin
            || projectile->position.y < -margin
            || projectile->position.x < -margin
            || projectile->position.x > game.getWindowWidth() + margin) {
            // 如果导弹超出边界，释放导弹对象的内存
            delete projectile;
            // 从projectilesEnemy容器中移除该导弹对象，并返回新的迭代器
            iterator = projectilesEnemy.erase(iterator);
        } else {
            SDL_Rect projectileRect {
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width, projectile->height
            };
            SDL_Rect playerRect {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width, player.height
            };
            // 判断导弹是否与玩家碰撞
            if (SDL_HasIntersection(&projectileRect, &playerRect) && !isDead) {
                player.currentHealth -= projectile->damage;
                delete projectile;
                iterator = projectilesEnemy.erase(iterator);
                Mix_PlayChannel(-1, sounds[SoundType::HIT], 0);
            } else {
                ++iterator;
            }
        }
    }
}

// 渲染敌人的子弹
void SceneMain::renderEnemyProjectiles()
{
    // 遍历所有敌人的子弹
    for (auto& projectile : projectilesEnemy) {
        // 创建一个SDL_Rect结构体，用于定义子弹的矩形区域
        SDL_Rect projectileRect {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width, projectile->height
        };
        // 计算子弹的旋转角度
        // 以正下方为 0 度，顺时针旋转
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180 / M_PI - 90;
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
    }
}

template <typename ReferencePoint>
// 定义一个静态函数addExplosion，用于在爆炸列表中添加一个新的爆炸效果
// 参数explosionTemplate是一个常量引用，表示爆炸的模板
// 参数object是一个指向ReferencePoint对象的指针，表示爆炸发生的位置
// 参数explosions是一个指向std::list<Explosion*>的引用，表示存储爆炸对象的列表
static void addExplosion(const Explosion& explosionTemplate, ReferencePoint object, std::list<Explosion*>& explosions)
{
    // 获取当前时间，单位为毫秒
    auto currentTime = SDL_GetTicks();
    // 根据模板创建一个新的爆炸对象
    auto explosion = new Explosion(explosionTemplate);
    // 设置爆炸对象的x坐标，使其位于object的中心位置
    explosion->position.x = object->position.x + object->width / 2 - explosion->width / 2;
    // 设置爆炸对象的y坐标，使其位于object的中心位置
    explosion->position.y = object->position.y + object->height / 2 - explosion->height / 2;
    // 设置爆炸对象的开始时间
    explosion->startTime = currentTime;
    // 将新的爆炸对象添加到爆炸列表中
    explosions.push_back(explosion);
}

void SceneMain::enemyExplode(Enemy* enemy)
{
    addExplosion(explosionTemplate, enemy, explosions);
    // 生成一个随机数，决定是否掉落道具
    if (dis(gen) < 0.5f) {
        dropItem(enemy);
    }
    score += 10;
    delete enemy;
}

// 更新玩家状态
void SceneMain::updatePlayer(float)
{
    if (isDead) {
        return;
    }
    if (player.currentHealth <= 0) {
        isDead = true;
        addExplosion(explosionTemplate, &player, explosions);
        Mix_PlayChannel(-1, sounds[SoundType::PLAYER_EXPLODE], 0);
    }
}

void SceneMain::updateExplosions(float)
{
    // 获取当前时间（以毫秒为单位）
    auto currentTime = SDL_GetTicks();
    // 遍历所有爆炸效果
    for (auto iterator = explosions.begin(); iterator != explosions.end();) {
        // 获取当前迭代器指向的爆炸效果对象
        auto explosion = *iterator;
        // 计算当前帧数，根据当前时间减去爆炸开始时间，再乘以每秒帧数（FPS），最后除以1000将毫秒转换为秒
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000;
        // 如果当前帧数大于或等于总帧数，表示爆炸效果播放完毕
        if (explosion->currentFrame > explosion->totalFrames) {
            // 删除爆炸效果对象，释放内存
            delete explosion;
            // 从爆炸效果列表中移除当前迭代器指向的元素，并返回新的迭代器
            iterator = explosions.erase(iterator);
        } else {
            // 如果爆炸效果还未播放完毕，继续遍历下一个元素
            ++iterator;
        }
    }
}

// 渲染爆炸效果
void SceneMain::renderExplosions()
{
    // 遍历explosions容器中的每一个爆炸对象
    for (auto explosion : explosions) {
        // 定义源矩形src，用于指定纹理中爆炸动画当前帧的位置和大小
        SDL_Rect src {
            explosion->currentFrame * explosion->width, // 计算当前帧在纹理中的x坐标
            0, // y坐标固定为0，假设所有帧都在同一行
            explosion->width, explosion->height
        };
        SDL_Rect dst {
            static_cast<int>(explosion->position.x),
            static_cast<int>(explosion->position.y),
            explosion->width, explosion->height
        };
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &src, &dst);

        // TODO debug 完删除 爆炸动画仅有2帧
        using std::cout, std::format;
        // cout << format("源矩形{} {} {} {}\n", src.x, src.y, src.w, src.h);
        // cout << format("目标矩形{} {} {} {}\n", dst.x, dst.y, dst.w, dst.h);
        cout << format("当前帧：{}\n\n", explosion->currentFrame);
    }
}

// 在敌人死亡时生成并掉落物品
void SceneMain::dropItem(Enemy* enemy)
{
    // 创建一个新的物品对象，使用itemLifeTemplate作为模板
    auto item = new Item(itemLifeTemplate);
    // 设置物品的初始位置为敌人的中心位置
    // 物品的x坐标为敌人的x坐标加上敌人宽度的一半再减去物品宽度的一半，使物品出现在敌人中心
    item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;
    // 物品的y坐标为敌人的y坐标加上敌人高度的一半再减去物品高度的一半，使物品出现在敌人中心
    item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2;
    // 生成一个随机角度，用于确定物品掉落的方向
    // dis是一个分布对象，gen是一个随机数生成器
    float angle = dis(gen) * 2 * M_PI;
    // 根据生成的角度计算物品的x方向上的速度分量
    item->direction.x = cos(angle);
    // 根据生成的角度计算物品的y方向上的速度分量
    item->direction.y = sin(angle);
    // 将生成的物品添加到场景中的物品列表中
    items.push_back(item);
}

// 更新场景中的物品状态
void SceneMain::updateItems(float deltaTime)
{
    for (auto iterator = items.begin(); iterator != items.end();) {
        auto item = *iterator;
        // 更新物品的位置
        item->position.x += item->direction.x * item->speed * deltaTime;
        item->position.y += item->direction.y * item->speed * deltaTime;

        // 物品到屏幕边缘时反弹
        if (item->bounceCount >= 0 && item->position.x < 0 || item->position.x + item->width > game.getWindowWidth()) {
            item->direction.x = -item->direction.x;
            --item->bounceCount;
        }
        if (item->bounceCount >= 0 && item->position.y < 0 || item->position.y + item->height > game.getWindowHeight()) {
            item->direction.y = -item->direction.y;
            --item->bounceCount;
        }

        // 如果超出屏幕范围则删除
        if (item->position.x + item->width < 0
            || item->position.x > game.getWindowWidth()
            || item->position.y + item->height < 0
            || item->position.y > game.getWindowHeight()) {
            delete item;
            iterator = items.erase(iterator);
            continue;
        }

        SDL_Rect itemRect {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width, item->height
        };
        SDL_Rect playerRect {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width, player.height
        };
        // 检查玩家是否与物品发生碰撞
        if (SDL_HasIntersection(&playerRect, &itemRect) && isDead == false) {
            playerGetItem(item);
            delete item;
            iterator = items.erase(iterator);
            continue;
        }
        ++iterator;
    }
}

// 玩家拾取物品
void SceneMain::playerGetItem(Item* item)
{
    score += 5;
    if (item->type == ItemType::Life) {
        player.currentHealth += 1;
        Mix_PlayChannel(-1, sounds[SoundType::GET_ITEM], 0);
        if (player.currentHealth > player.maxHealth) {
            player.currentHealth = player.maxHealth;
        }
    }
}

// 渲染场景中的物品
void SceneMain::renderItems()
{
    for (auto& item : items) {
        SDL_Rect itemRect {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width, item->height
        };
        SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &itemRect);
    }
}

void SceneMain::renderUI()
{
    // 渲染血条
    int x { 10 };
    int y { 10 };
    int size { 32 };
    int offset { 30 };
    SDL_SetTextureColorMod(uiHealth, 100, 100, 100);
    for (int i { 0 }; i < player.maxHealth; ++i) {
        SDL_Rect dstRect { x + i * offset, y, size, size };
        SDL_RenderCopy(game.getRenderer(), uiHealth, NULL, &dstRect);
    }
    SDL_SetTextureColorMod(uiHealth, 255, 255, 255);
    for (int i { 0 }; i < player.currentHealth; ++i) {
        SDL_Rect dstRect { x + i * offset, y, size, size };
        SDL_RenderCopy(game.getRenderer(), uiHealth, NULL, &dstRect);
    }

    // 渲染得分
    auto text { format("得分: {}", score) };
    SDL_Color color { 255, 255, 255, 255 };
    SDL_Surface* surface { TTF_RenderUTF8_Solid(scoreFont, text.c_str(), color) };
    SDL_Texture* texture { SDL_CreateTextureFromSurface(game.getRenderer(), surface) };
    SDL_Rect dstRect { game.getWindowWidth() - surface->w - 10, 10, surface->w, surface->h };
    SDL_RenderCopy(game.getRenderer(), texture, NULL, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}