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
        Renderer::initialize(window.get());
        Registry::registerBaseItems();
        glfwSwapInterval(0);
        world->generateChunks();
    }

    void Terra::run()
    {
        init();
        gameLoop();
    }

    double totalTime = 0.0;
    int frameCount = 0;

    void Terra::gameLoop()
    {
        while (!glfwWindowShouldClose(window->getWindow()))
        {
            startFrame = std::chrono::high_resolution_clock::now();
            glClear(GL_COLOR_BUFFER_BIT);

            glm::vec2 direction = glm::vec2(0);

            if (glfwGetKey(window->getWindow(), GLFW_KEY_D))
                direction = glm::vec2(1.0f, direction.y);
            if (glfwGetKey(window->getWindow(), GLFW_KEY_A))
                direction = glm::vec2(-1.0f, direction.y);
            if (glfwGetKey(window->getWindow(), GLFW_KEY_W))
                direction = glm::vec2(direction.x, 1.0f);
            if (glfwGetKey(window->getWindow(), GLFW_KEY_S))
                direction = glm::vec2(direction.x, -1.0f);
            if (direction != glm::vec2(0.0f))
                direction = glm::normalize(direction);
            Renderer::getCamera()->setPosition(Renderer::getCamera()->getPosition() + direction * glm::vec2(deltaTime) * glm::vec2(8));

            world->render();

            glfwSwapBuffers(window->getWindow());
            glfwPollEvents();

            deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startFrame).count();

            totalTime += deltaTime;
            frameCount++;

            double avgFPS = frameCount / totalTime;

            std::cout << "\rFPS: " << avgFPS << "          ";
        }
    }
}
