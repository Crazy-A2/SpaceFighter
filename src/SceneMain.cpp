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
}

void SceneMain::update()
{
    keyboardControl();
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

void SceneMain::keyboardControl()
{
    auto keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_LEFT] && player.position.x >= 0) {
        player.position.x -= .4;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT] && player.position.x <= game.getWindowWidth() - player.width) {
        player.position.x += .4;
    }
    if (keyboardState[SDL_SCANCODE_UP] && player.position.y >= 0) {
        player.position.y -= .4;
    }
    if (keyboardState[SDL_SCANCODE_DOWN] && player.position.y <= game.getWindowHeight() - player.height) {
        player.position.y += .4;
    }
}