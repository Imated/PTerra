#include "Terra.h"

#include "misc/Random.h"
#include "renderer/ShaderLibrary.h"

namespace Terra
{
    Terra::Terra()
    {
        window = std::make_unique<Window>(480, 256, "Terra");
        ShaderLibrary::load("default",
                    {
                        {Vertex, RESOURCES_PATH "default.vert"},
                        {Fragment, RESOURCES_PATH "default.frag"}
                    });
        world = std::make_unique<World>();
    }

    void Terra::init()
    {
        Registry::registerBaseItems();
        Random::init();
        Renderer::initialize();
        glfwSwapInterval(1);
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
            DEBUGN("\rFPS: %f ", 1/deltaTime);
        }
    }
}
