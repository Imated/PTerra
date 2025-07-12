#include "Terra.h"

Terra::Terra()
{
    window = std::make_unique<Window>(480, 256, "Terra");
}

void Terra::init()
{
    glClearColor(0.3f, 0.65f, 1.f, 1.f);
}

void Terra::run()
{
    init();
    gameLoop();
}

void Terra::gameLoop()
{
    while (!glfwWindowShouldClose(window->getWindow()))
    {
        window->clear();
        
        glfwSwapBuffers(window->getWindow());
        glfwPollEvents();
    }
}
