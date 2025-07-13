#pragma once
#include <chrono>
#include <memory>

#include "renderer/Window.h"
#include "world/World.h"

namespace Terra
{
    class Terra
    {
    public:
        Terra();
        void init();
        void run();
        void gameLoop();

    private:
        std::unique_ptr<Window> window;
        std::unique_ptr<World> world;
        double deltaTime = 0.0f;
        std::chrono::time_point<std::chrono::system_clock> startFrame;
    };
}
