#pragma once

#include "Scene.h"

#include <string>

class SceneEnd : public Scene {
    bool isTyping { true };
    std::string name { "" };
    float blinkTimer { 1.0f };

    void renderPhase1();
    void renderPhase2();
    void removeLastUTF8Char(std::string& str);

public:
    SceneEnd() = default;
    ~SceneEnd() = default;

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;
};