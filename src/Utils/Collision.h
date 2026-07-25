#pragma once
#include <vector>
#include <memory>
#include "Renderer/Sprite.h"
#include "GamePlay/Tile.h"

inline bool checkCollision(const Renderer::Sprite::Rect& a, const Renderer::Sprite::Rect& b){
    bool overlapX = (a.x < b.x + b.width) && (a.x + a.width > b.x);
    bool overlapY = (a.y < b.y + b.height) && (a.y + a.height > b.y);
    return overlapX && overlapY;
}
inline bool checkAllCollisions(const Renderer::Sprite& a, const std::vector<std::shared_ptr<Tile>>& tiles){
    for(auto& tile : tiles) {
        if(checkCollision(a.getRect(), tile->getRect())){
            return true;
        }
    }
    return false;
}

