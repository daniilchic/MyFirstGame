#pragma once
#include <memory>
#include <vector>
#include "Tank.h"
#include "Tile.h"
#include "InputData.h"
#include "Utils/Collision.h"

class TankController {
    public:
        TankController() = delete;
        static void update(Tank& tank, const std::vector<std::shared_ptr<Tile>>& blockingTiles, 
                           const InputData& input, const float deltaTime, std::vector<std::shared_ptr<Bullet>>& bullets, const float speed = 100.0f);
};
