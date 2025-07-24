#pragma once

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 16;
//constexpr int TILE_WIDTH = 16;
//constexpr int TILE_HEIGHT = 16;
constexpr int MAX_CHUNKS_X = 5;
constexpr int MAX_CHUNKS_Y = 3;
constexpr float HORIZONTAL_TILES = 48.f;
constexpr float VERTICAL_TILES = 27.f; // TODO: HORIZONTAL_TILES / aspect
constexpr float LAYER_BACKGROUND = 0.f;
constexpr float LAYER_BEHIND_PLAYER = 0.1f;
constexpr float LAYER_ENTITY = 0.2f;
constexpr float LAYER_OVER_PLAYER = 0.3f;
constexpr float LAYER_FOREGROUND = 0.4f;
