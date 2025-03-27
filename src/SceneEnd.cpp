#include "SceneEnd.h"

#include "Game.h"
#include "SceneMain.h"

#include <format>
#include <string>

#include <iostream>

#define FLASHING(x)          \
    if (blinkTimer > 0.5f) { \
        x;                   \
    }

// SceneEnd();
// ~SceneEnd();

using std::format, std::string;

void SceneEnd::init()
{
    if (!SDL_IsTextInputActive()) {
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to start text input");
    }
};

void SceneEnd::update(float deltaTime)
{
    blinkTimer -= deltaTime;
    if (blinkTimer < 0.0f) {
        blinkTimer = 1.0f;
    }
};

void SceneEnd::render()
{
    if (isTyping) {
        renderPhase1();
    } else {
        renderPhase2();
    }
};

void SceneEnd::clean() { };

void SceneEnd::handleEvents(SDL_Event* event)
{
    if (isTyping) {
        if (event->type == SDL_TEXTINPUT) {
            name = format("{}{}", name, event->text.text);
        }
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                printf("enter\n");
                isTyping = false;
                SDL_StopTextInput();
                if (name == "") {
                    // name = "(❁´◡`❁)=￣ω￣=";
                    // name = "ahh2333";
                    name = "无名";
                }
                game.insertLeaderBoard(game.getFinalScore(), name);
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                if (name != "") {
                    removeLastUTF8Char(name);
                }
            }
        }
    } else {
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                game.changeScene(new SceneMain());
            }
        }
    }
};

void SceneEnd::renderPhase1()
{
    auto score { game.getFinalScore() };
    string scoreText { format("你的得分是：{}", score) };
    string gameOver { "Game Over" };
    string instrutionText { "请输入你的名字，按回车键确认：" };
    game.renderTextCentered(gameOver, 0.2f, true);
    game.renderTextCentered(scoreText, 0.4f, false);
    game.renderTextCentered(instrutionText, 0.6f, false);

    if (name != "") {
        auto p = game.renderTextCentered(name, 0.8f, false);
        FLASHING(game.renderTextPos("_", p.x, p.y))
    } else {
        FLASHING(game.renderTextCentered("_", 0.8f, false))
    }
}

void SceneEnd::renderPhase2()
{
    game.renderTextCentered("得分榜", 0.1f, true);
    auto i { 1 };
    auto posY { 0.2f * game.getWindowHeight() };
    for (auto& item : game.getLeaderBoard()) {
        string name { format("{}. {}", i, item.second) };
        string score { format("{}", item.first) };

        game.renderTextPos(name, 100, static_cast<int>(posY), true);
        game.renderTextPos(score, 100, static_cast<int>(posY), false);
        posY += 50;
        ++i;
    }
    FLASHING(game.renderTextCentered("按 ESC 返回游戏", 0.8f, false))
}

// 定义一个函数，用于从给定的UTF-8字符串中移除最后一个字符
void SceneEnd::removeLastUTF8Char(std::string& str)
{
    // 获取字符串的最后一个字符
    auto lastChar { str.back() };
    // 检查最后一个字符是否是UTF-8多字节字符的第一个字节
    // UTF-8多字节字符的第一个字节的高位总是以2个或更多的1开头
    if ((lastChar & 0b1000'0000) == 0b1000'0000) {
        // 如果是，移除最后一个字符
        str.pop_back();
        // 继续移除该多字节字符的后续字节
        // UTF-8多字节字符的后续字节总是以10开头
        while ((str.back() & 0b1100'0000) != 0b1100'0000) {
            str.pop_back();
        }
    }
    // 移除最后一个字符（无论是单字节字符还是多字节字符的最后一个字节）
    str.pop_back();
}
