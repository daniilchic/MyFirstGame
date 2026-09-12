#pragma once

#include <vector>
#include <memory>
#include <random>
#include <glm/vec2.hpp>

#include "Tank.h"
#include "Tile.h"
#include "Renderer/Sprite.h"
#include "Utils/Collision.h"
#include "Utils/WorldPosition.h"

inline glm::ivec2 findRandomFreeCell(const std::vector<std::shared_ptr<Tile>>& blockingTiles,
                                     const std::shared_ptr<Tank>& playerTank,
                                     std::mt19937& gen)
{
    std::vector<glm::ivec2> freeCells;
    freeCells.reserve(100);

    for(int row = 1; row <= 10; row++){
        for(int col = 1; col <= 10; col++){
            glm::vec2 cellPos = worldPosition(col, row);
            Renderer::Sprite::Rect cellRect{cellPos.x - 32.f, cellPos.y - 32.f, 64.f, 64.f};
            bool isBlocked = false;
            for(auto& tile : blockingTiles){
                if(checkCollision(tile->getRect(), cellRect)){
                    isBlocked = true;
                    break;
                }
            }
            if(isBlocked){ continue; }

            if(playerTank && !playerTank->isTankDestroyed() && checkCollision(playerTank->getRect(), cellRect)){
                continue;
            }
            freeCells.push_back({col, row});
        }
    }
    if(freeCells.empty()) { return {-1, -1}; }

    std::uniform_int_distribution<> dist(0, static_cast<int>(freeCells.size()) - 1);
    return freeCells[dist(gen)];
}
