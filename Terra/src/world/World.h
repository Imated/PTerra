#pragma once
#include <memory>
#include <unordered_map>

#include "Chunk.h"
#include "misc/Constants.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

namespace Terra {

class World {
public:
    World();
    ~World();

    void init();

    void updateChunks();

    void render(glm::mat4 vp);

    static Tile* getGlobalTileAt(glm::ivec2 worldPos);
    static Chunk generateChunk(glm::ivec2 chunkPos);

    struct chunkData { //struct providing storage and methods for loading and unloading chunks
        static inline std::array<std::array<Chunk*,MAX_CHUNKS_X>, MAX_CHUNKS_Y> chunks; //chunks actively updated and rendered
        static inline std::map<std::vector<int>, glm::ivec2> globalPositions;
        static void loadChunk(glm::ivec2 worldPos, glm::ivec2 centerChunk);
        static void unloadChunk(glm::ivec2 worldPos);
    };
    static chunkData loadedChunks;

    struct worldData { //bruh
        static inline std::array<Chunk, 64>* quadrant1; //portion of world loaded into ram. swap space for world. TODO: implement save files, then infinite world
        static inline std::array<Chunk, 64>* quadrant2;
        static inline std::array<Chunk, 64>* quadrant3;
        static inline std::array<Chunk, 64>* quadrant4;
        Chunk* operator[](glm::ivec2 worldPos);
    };
    static inline worldData world;

private:
    Texture tileAtlas;
    int32_t seed = 0;
    Shader* tileShader;
};
}

