#include "Terra.h"

#include "misc/Constants.h"
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
        glfwSwapInterval(1);
        world->init();
        world->generateChunks();
    }

    void Terra::run()
    {
        init();
        gameLoop();
    }

    double totalTime = 0.0;
    int frameCount = 0;
    glm::vec2 lastChunkPos = glm::vec2(0);
    glm::vec2 currentChunkPos = glm::vec2(0);

    void Terra::gameLoop()
    {
        while (!glfwWindowShouldClose(window->getWindow()))
        {
            startFrame = std::chrono::high_resolution_clock::now();
            glClear(GL_COLOR_BUFFER_BIT);

            auto direction = glm::vec2(0);
            direction.x += glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS;
            direction.x -= glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS;
            direction.y += glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS;
            direction.y -= glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS;

            if (direction != glm::vec2(0.f))
                direction = glm::normalize(direction);

            auto camSpeed = 8.f;
            Renderer::getCamera()->setPosition(Renderer::getCamera()->getPosition() + direction * glm::vec2(deltaTime) * camSpeed);

            lastChunkPos = currentChunkPos;
            auto cam   = Renderer::getCamera();
            glm::vec2 worldTilePos = cam->getPosition();
            currentChunkPos = { std::floor(worldTilePos.x / CHUNK_WIDTH), std::floor(worldTilePos.y / CHUNK_WIDTH) };;


            std::cout << "camPos: " << worldTilePos.x << ", " << worldTilePos.y
          << " | chunk: " << currentChunkPos.x << ", " << currentChunkPos.y << std::endl;

            if (lastChunkPos != currentChunkPos)
                world->updateChunks();

            world->render();

            glfwSwapBuffers(window->getWindow());
            glfwPollEvents();

            deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startFrame).count();

            totalTime += deltaTime;
            frameCount++;

            double avgFPS = frameCount / totalTime;

            //std::cout << "\rFPS: " << avgFPS << "          ";
        }
    }
}
