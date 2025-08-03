#include "Terra.h"

#include "audio/Audio.h"
#include "audio/AudioBuffer.h"
#include "audio/AudioSource.h"
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
        cursor = std::make_unique<Cursor>();
    }

    void Terra::init()
    {
        ShaderLibrary::load("tile",
                            {
                                {Vertex, "resources/default.vert"},
                                {Fragment, "resources/tile.frag"}
                            });
        ShaderLibrary::load("entity",
                            {
                                {Vertex, "resources/default.vert"},
                                {Fragment, "resources/entity.frag"}
                            });
        ShaderLibrary::load("cursor",
                    {
                        {Vertex, "resources/default.vert"},
                        {Fragment, "resources/cursor.frag"}
                    });
        Renderer::initialize(window.get());
        Registry::registerBaseItems();
        Audio::init();
        glfwSwapInterval(1);
        world->init();
        player->init();
        cursor->init();
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

            currentChunkPos = Renderer::getCamera()->getChunk();
            if (lastChunkPos != currentChunkPos) {
                world->updateChunks();
                INFO("UPDATING CHUNKS:  %f %f; %f %f", currentChunkPos.x, currentChunkPos.y, lastChunkPos.x, lastChunkPos.y);
                lastChunkPos = currentChunkPos;
            }

            glm::mat4 vp = Renderer::getCamera()->getProjectionMatrix() * Renderer::getCamera()->getViewMatrix();
            world->render(vp);
            cursor->render(window.get(), vp, deltaTime);
            player->render(vp);

            Audio::update();

            glfwSwapBuffers(window->getWindow());
            glfwPollEvents();

            deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startFrame).count();
        }
    }
}
