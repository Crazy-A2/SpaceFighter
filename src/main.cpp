#define SDL_MAIN_HANDLED // 解决 main 函数被 SDL_main 函数替换的问题
#include "Game.h"
#include <SDL.h>
// #include <SDL_image.h>
// #include <SDL_mixer.h>
// #include <SDL_ttf.h>
// #include <format>
#include <iostream>

int main(int, char**)
{
    Game* game = new Game();
    game->init();
    game->run();
}

// int main(int, char**)
// {
//     // 初始化 SDL
//     if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//         std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     // 创建窗口
//     SDL_Window* window = SDL_CreateWindow("Hello, SDL2!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);

//     // 创建渲染器
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

//     // SDL_Image 初始化
//     if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
//         std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
//         return 1;
//     }

//     // 创建纹理
//     SDL_Texture* texture = IMG_LoadTexture(renderer, std::format("{}/assets/image/bg.png", PROJECT_DIR).c_str());

//     // SDL_Mixer 初始化
//     if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
//         std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
//         return 1;
//     }

//     // 读取音频文件 路径以生成二进制后路径为准
//     Mix_Music* music = Mix_LoadMUS(std::format("{}/assets/music/03_Racing_Through_Asteroids_Loop.ogg", PROJECT_DIR).c_str());
//     // 播放音乐
//     Mix_PlayMusic(music, -1);

//     // SDL_ttf 初始化
//     if (TTF_Init() != 0) {
//         std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
//         return -1;
//     }

//     // 加载字体
//     TTF_Font* font = TTF_OpenFont(std::format("{}/assets/font/VonwaonBitmap-16px.ttf", PROJECT_DIR).c_str(), 24);
//     SDL_Color color { 255, 255, 0, 255 };
//     SDL_Surface* surface = TTF_RenderUTF8_Solid(font, "这是加载后的字体", color);
//     // 用字体创建纹理
//     SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, surface);

//     // 渲染循环
//     while (true) {
//         SDL_Event event;

//         if (SDL_PollEvent(&event) && (event.type == SDL_QUIT)) {
//             break;
//         }

//         // 清空渲染器
//         SDL_RenderClear(renderer);
//         // 画个长方形
//         SDL_Rect rect = { 100, 100, 200, 200 };
//         SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 渲染器设为红色
//         SDL_RenderFillRect(renderer, &rect);
//         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 渲染器改回黑色

//         // 绘制纹理
//         SDL_Rect dstrect { 200, 200, 300, 400 };
//         SDL_RenderCopy(renderer, texture, NULL, &dstrect);

//         // 绘制字体纹理
//         SDL_Rect textDstrect { 100, 100, surface->w, surface->h }; // 使用字体纹理的宽高
//         SDL_RenderCopy(renderer, textTexture, NULL, &textDstrect);

//         // 更新屏幕
//         SDL_RenderPresent(renderer);
//     }

//     // 清理并释放字体资源
//     SDL_DestroyTexture(textTexture);
//     TTF_CloseFont(font);
//     TTF_Quit();

//     // 清理并释放图片相关资源
//     SDL_DestroyTexture(texture);
//     IMG_Quit();

//     // 清理并释放音乐相关资源
//     Mix_FreeMusic(music);
//     Mix_CloseAudio();
//     Mix_Quit();

//     // 清理并退出 SDL
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
// }
