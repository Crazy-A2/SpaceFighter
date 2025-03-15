#define SDL_MAIN_HANDLED // 解决 main 函数被 SDL_main 函数替换的问题
#include <SDL.h>
#include <iostream>

// int main(int argc, char** argv)
int main(int, char**)
{
    std::cout << "Hello, from sdl2-game!\n";

    // 初始化 SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("Hello, SDL2!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 渲染循环
    while (true) {
        SDL_Event event;

        if (SDL_PollEvent(&event) && (event.type == SDL_QUIT)) {
            break;
        }

        // 清空渲染器
        SDL_RenderClear(renderer);
        // 画个长方形
        SDL_Rect rect = { 100, 100, 200, 200 };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 渲染器设为红色
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 渲染器改回黑色

        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 销毁渲染器
    SDL_DestroyRenderer(renderer);
    // 销毁窗口
    SDL_DestroyWindow(window);
    // 退出 SDL
    SDL_Quit();
}
