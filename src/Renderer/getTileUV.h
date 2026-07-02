#pragma once
#include <glm/vec2.hpp>


inline void getTileUV(unsigned int tileID, glm::vec2& outOffset, glm::vec2& outScale) {
    
    const float TEX_W = 384.0f;
    const float TEX_H = 256.0f;
    const float TILE_SIZE = 16.0f;
    const int TILES_PER_ROW = static_cast<int>(TEX_W / TILE_SIZE);
     
    outScale.x = TILE_SIZE / TEX_W;
    outScale.y = TILE_SIZE / TEX_H;
    
    int col = tileID % TILES_PER_ROW;
    int row = tileID / TILES_PER_ROW;
 
    outOffset.x = (col * TILE_SIZE) / TEX_W;
    outOffset.y = 1.0f - ((row + 1) * TILE_SIZE) / TEX_H;
}
