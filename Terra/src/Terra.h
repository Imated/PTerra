#pragma once
#include <memory>

#include "renderer/Window.h"

class Terra
{
public:
    Terra();
    void init();
    void run();
    void gameLoop();

private:
    std::unique_ptr<Window> window;
};
