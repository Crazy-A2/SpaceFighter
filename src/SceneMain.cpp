#include "SceneMain.h"
#include "Game.h"
#include <SDL_image.h>
#include <format>
#include <random>

SceneMain::SceneMain()
    : game(Game::getInstance())
{
}

SceneMain::~SceneMain() { }

template <typename T>
static void initTextureByTemplate(Game& game, T& object, const char* imageName)
{
    object.texture = IMG_LoadTexture(game.getRenderer(), std::format("{}/assets/image/{}", PROJECT_DIR, imageName).c_str());
    // if (object.texture == nullptr) {
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load %s texture: %s", IMG_GetError());
    //     return;
    // }
    SDL_QueryTexture(object.texture, NULL, NULL, &object.width, &object.height);
    // 将图片纹理的宽度和高度缩小为原来的 1 / 4
    object.width /= 4;
    object.height /= 4;
}

// 定义SceneMain类的init成员函数，用于初始化场景
void SceneMain::init()
{
    // 初始化随机数生成器
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    // 加载玩家飞船的纹理图片，路径为PROJECT_DIR目录下的assets/image/SpaceShip.png
    // game.getRenderer()获取当前的SDL渲染器，std::format用于格式化字符串
    // player.texture = IMG_LoadTexture(game.getRenderer(), std::format("{}/assets/image/SpaceShip.png", PROJECT_DIR).c_str());
    // if (player.texture == nullptr) {
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load player texture: %s", IMG_GetError());
    //     return;
    // }

    // // 查询纹理的宽度和高度，并将结果存储在player.width和player.height中
    // // SDL_QueryTexture函数的第一个参数是纹理，第二个和第三个参数是用于接收纹理格式和访问信息的指针（这里不需要，所以传入NULL）
    // SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    // // 将玩家飞船的宽度和高度缩小为原来的 1 / 4
    // player.width /= 4;
    // player.height /= 4;
    initTextureByTemplate(game, player, "SpaceShip.png");

    // 设置玩家飞船的初始位置
    // 横坐标为窗口宽度的一半减去飞船宽度的一半，使飞船水平居中
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    // 纵坐标为窗口高度减去飞船的高度，使飞船位于窗口底部
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化模板
    // projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), std::format("{}/assets/image/laser-3.png", PROJECT_DIR).c_str());
    // SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    // projectilePlayerTemplate.width /= 4;
    // projectilePlayerTemplate.height /= 4;
    initTextureByTemplate(game, projectilePlayerTemplate, "laser-3.png");

    // enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), std::format("{}/assets/image/insect-2.png", PROJECT_DIR).c_str());
    // SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
    // enemyTemplate.width /= 4;
    // enemyTemplate.height /= 4;
    initTextureByTemplate(game, enemyTemplate, "insect-2.png");

    // projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), std::format("{}/assets/image/laser-2.png", PROJECT_DIR).c_str());
    // SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    // projectileEnemyTemplate.width /= 4;
    // projectileEnemyTemplate.height /= 4;
    initTextureByTemplate(game, projectileEnemyTemplate, "laser-2.png");
}

// 更新游戏主场景中的状态
void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime); // 根据键盘输入更新玩家状态
    updatePlayerProjectiles(deltaTime); // 更新玩家发射的子弹状态
    updateEnemyProjectiles(deltaTime); // 更新敌人发射的子弹状态
    spawEnemy(); // 生成新的敌人
    updateEnemies(deltaTime); // 更新敌人状态
}

// 定义SceneMain类的render函数，用于渲染场景中的主要内容
void SceneMain::render()
{
    renderPlayerProjectiles(); // 渲染玩家发射的子弹
    renderEnemyProjectiles(); // 渲染敌人
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

    renderEnemies();
}

void SceneMain::clean()
{
    // 清理容器
    for (auto& projectile : projectilesPlayer) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesPlayer.clear();
    for (auto& enemy : enemies) {
        if (enemy != nullptr) {
            delete enemy;
        }
    }
    enemies.clear();
    for (auto& projectile : projectilesEnemy) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilesEnemy.clear();

    // 清理模板
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
    if (projectilePlayerTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }
    if (enemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    if (projectileEnemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }
}

void SceneMain::handleEvents(SDL_Event* event) { }

// SceneMain类的成员函数，用于处理键盘控制
void SceneMain::keyboardControl(float deltaTime)
{
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
            SDL_Log("子弹被移除了...");
        } else {
            // 如果未超出范围，继续遍历下一个投射物
            ++iterator;
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

void SceneMain::updateEnemies(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for (auto iterator = enemies.begin(); iterator != enemies.end();) {
        auto enemy = *iterator;
        enemy->position.y += enemy->speed * deltaTime;
        if (enemy->position.y > game.getWindowHeight()) {
            delete enemy;
            iterator = enemies.erase(iterator);
        } else {
            if (currentTime - enemy->lastShootTime > enemy->coolDown) {
                shootEnemy(enemy);
                enemy->lastShootTime = currentTime;
            }
            ++iterator;
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

// 更新玩家发射的导弹位置
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
            // 如果导弹未超出边界，继续遍历下一个导弹对象
            ++iterator;
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