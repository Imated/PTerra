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
        player = std::make_unique<Player>();
    }

    void Terra::init()
    {
        ShaderLibrary::load("default",
                            {
                                {Vertex, "resources/default.vert"},
                                {Fragment, "resources/default.frag"}
                            });
        ShaderLibrary::load("player",
                            {
                                {Vertex, "resources/default.vert"},
                                {Fragment, "resources/entity.frag"}
                            });
        Renderer::initialize(window.get());
        Registry::registerBaseItems();
        glfwSwapInterval(1);
        world->init();
        world->generateChunks();
        player->init();
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            player->update(window.get(), deltaTime);

            lastChunkPos = currentChunkPos;
            auto cam   = Renderer::getCamera();
            glm::vec2 worldTilePos = cam->getPosition();
            currentChunkPos = { std::floor(worldTilePos.x / CHUNK_WIDTH), std::floor(worldTilePos.y / CHUNK_WIDTH) };

            if (lastChunkPos != currentChunkPos)
                world->updateChunks();

            glm::mat4 vp = Renderer::getCamera()->getProjectionMatrix() * Renderer::getCamera()->getViewMatrix();
            world->render(vp);
            player->render(vp);

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
