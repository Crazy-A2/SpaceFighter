#include "SceneTitle.h"
#include "sceneMain.h"

#include <format>
#include <iostream>

void SceneTitle::init()
{
    bgm = Mix_LoadMUS(std::format("{}/assets/music/06_Battle_in_Space_Intro.ogg", PROJECT_DIR).c_str());
    if (bgm == NULL) {
        printf("Failed to load 06_Battle_in_Space_Intro.ogg! SDL_mixer Error: %s\n", Mix_GetError());
    } else {
        Mix_PlayMusic(bgm, -1);
    }
}

void SceneTitle::update(float deltaTime)
{
    timer += deltaTime;
    if (timer > 1.0f) {
        timer -= 1.0f;
    }
    printf("timer: %f\n", timer);
}

void SceneTitle::render()
{
    std::string title = "太空战机";
    game.renderTextCentered(title, 0.4, true);

    if (timer < .5f) {
        title = "点击 空格键 开始游戏";
        game.renderTextCentered(title, 0.7, false);
    }
}

void SceneTitle::clean() { }

void SceneTitle::handleEvents(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
        game.changeScene(new SceneMain());
    }
}