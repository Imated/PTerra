#include "Terra.h"

#include "misc/Constants.h"
#include "misc/Random.h"
#include "renderer/ShaderLibrary.h"

namespace Terra
{
    Terra::Terra()
    {
        Random::init();
        window = std::make_unique<Window>(960, 540, "Terra");
        world = std::make_unique<World>();
        player = std::make_unique<Player>();
    }

    void Terra::init()
    {
        ShaderLibrary::load("tile",
                            {
                                {Vertex, "resources/default.vert"},
                                {Fragment, "resources/tile.frag"}
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
        player->init();
    }

    void Terra::run()
    {
        init();
        gameLoop();
    }

    //double totalTime = 0.0;
    //int frameCount = 0;
    glm::vec2 lastChunkPos = glm::vec2(9999);
    glm::vec2 currentChunkPos = glm::vec2(0);

    void Terra::gameLoop()
    {
        while (!glfwWindowShouldClose(window->getWindow()))
        {
            startFrame = std::chrono::high_resolution_clock::now();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            player->update(window.get(), deltaTime);

            currentChunkPos = Renderer::getCamera()->getChunkCentered();
            if (lastChunkPos != currentChunkPos) {
                world->updateChunks();
                INFO("UPDATING CHUNKS:  %i %i; %i %i", currentChunkPos.x, currentChunkPos.y, lastChunkPos.x, lastChunkPos.y);
                lastChunkPos = currentChunkPos;
            }

            glm::mat4 vp = Renderer::getCamera()->getProjectionMatrix() * Renderer::getCamera()->getViewMatrix();
            world->render(vp);
            player->render(vp);

            glfwSwapBuffers(window->getWindow());
            glfwPollEvents();

            deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startFrame).count();

            //totalTime += deltaTime;
            //frameCount++;

            //double avgFPS = frameCount / totalTime;

            //std::cout << "\rFPS: " << avgFPS << "          ";
        }
    }
}
