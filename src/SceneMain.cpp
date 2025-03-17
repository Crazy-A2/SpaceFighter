#include "SceneMain.h"
#include "Game.h"
#include <SDL_image.h>
#include <format>

SceneMain::SceneMain()
    : game(Game::getInstance())
{
}

SceneMain::~SceneMain() { }

// 定义SceneMain类的init成员函数，用于初始化场景
void SceneMain::init()
{
    // 加载玩家飞船的纹理图片，路径为PROJECT_DIR目录下的assets/image/SpaceShip.png
    // game.getRenderer()获取当前的SDL渲染器，std::format用于格式化字符串
    player.texture = IMG_LoadTexture(game.getRenderer(), std::format("{}/assets/image/SpaceShip.png", PROJECT_DIR).c_str());
    if (player.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load player texture: %s", IMG_GetError());
        return;
    }

    // 查询纹理的宽度和高度，并将结果存储在player.width和player.height中
    // SDL_QueryTexture函数的第一个参数是纹理，第二个和第三个参数是用于接收纹理格式和访问信息的指针（这里不需要，所以传入NULL）
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    // 将玩家飞船的宽度和高度缩小为原来的 1 / 4
    player.width *= .25;
    player.height *= .25;

    // 设置玩家飞船的初始位置
    // 横坐标为窗口宽度的一半减去飞船宽度的一半，使飞船水平居中
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    // 纵坐标为窗口高度减去飞船的高度，使飞船位于窗口底部
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化模板
    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), std::format("{}/assets/image/laser-3.png", PROJECT_DIR).c_str());
    SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    // 将子弹的宽度和高度缩小为原来的 1 / 4
    projectilePlayerTemplate.width *= .25;
    projectilePlayerTemplate.height *= .25;
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime);
}

// 定义SceneMain类的render函数，用于渲染场景中的主要内容
void SceneMain::render()
{
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

void SceneMain::clean()
{
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
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

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    for (auto projectile : projectilesPlayer) { // 遍历玩家发射的所有子弹
        projectile->position.y -= projectile->speed * deltaTime; // 更新子弹的位置，根据子弹的速度和经过的时间计算新的Y坐标
        if (projectile->position.y < 0) {
            delete projectile;
            projectilesPlayer.remove(projectile);
            // 检查子弹是否超出屏幕顶部，如果是，则删除子弹并从列表中移除
            
        }
    }
}
