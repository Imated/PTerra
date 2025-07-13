#include "Terra.h"

Terra::Terra()
{
    window = std::make_unique<Window>(480, 256, "Terra");
}

void Terra::init()
{
    glClearColor(0.3f, 0.65f, 1.f, 1.f);
    glfwSwapInterval(0);
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
        startFrame = std::chrono::high_resolution_clock::now();
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window->getWindow());
        glfwPollEvents();

        deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startFrame).count();
        DEBUG("FPS: %f", 1/deltaTime);
    }
}
