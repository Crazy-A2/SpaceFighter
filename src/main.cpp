#define SDL_MAIN_HANDLED // 解决 main 函数被 SDL_main 函数替换的问题

#include "Game.h"
#include <SDL.h>

int main(int, char**)
{
    Game::getInstance().init();
    Game::getInstance().run();
}
