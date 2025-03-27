#include "SceneEnd.h"
#include "Game.h"

#include <format>
#include <string>

#include <iostream>

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

void SceneEnd::update(float deltaTime) { };

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
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                name.pop_back();
            }
        }
    } else {
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
        game.renderTextCentered(name, 0.8f, false);
    }
}

void SceneEnd::renderPhase2()
{
}
