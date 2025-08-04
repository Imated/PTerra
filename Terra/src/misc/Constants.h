#pragma once

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 16;
//constexpr int TILE_WIDTH = 16;
//constexpr int TILE_HEIGHT = 16;
constexpr int MAX_CHUNKS_X = 6;
constexpr int MAX_CHUNKS_Y = 6;
inline float aspect = 16.0 / 9.0;
constexpr float HORIZONTAL_TILES = 40.f;
inline float VERTICAL_TILES = HORIZONTAL_TILES / aspect;
constexpr float LAYER_TILES_GROUND = 0.f;
constexpr float LAYER_BEHIND_PLAYER = 0.1f;
constexpr float LAYER_ENTITY = 0.2f;
constexpr float LAYER_OVER_PLAYER = 0.3f;
constexpr float LAYER_FOREGROUND = 0.4f;
constexpr int REGION_X = 16;
constexpr int REGION_Y = 16;
constexpr int TILE_TYPE_TILE = 0;
constexpr int TILE_TYPE_AUTO_TILE = 1;
constexpr int NUM_AUDIO_BUFFERS = 8;
constexpr size_t BUFFER_SAMPLES = 8192;
