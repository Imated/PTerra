#include "Terra.h"

#include "misc/Random.h"
#include "renderer/ShaderLibrary.h"

namespace Terra
{
    Terra::Terra()
    {
        Random::init();
        window = std::make_unique<Window>(480, 256, "Terra");
        world = std::make_unique<World>();
    }

    void Terra::init()
    {
        Renderer::initialize();
        Registry::registerBaseItems();
        glfwSwapInterval(0);
        world->generateChunks();
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

            world->render();

            glfwSwapBuffers(window->getWindow());
            glfwPollEvents();

            deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startFrame).count();
            //std::cout << "\rFPS: " + static_cast<char>(1 / deltaTime) << std::flush;
        }
    }
}
