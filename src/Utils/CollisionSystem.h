#pragma once
#include <vector>
#include <memory>

class Bullet;
class Tank;
class Tile;
class Explosion;
namespace Renderer{
    class Texture2D;
    class ShaderProgram;
}
class CollisionSystem{
    public:
        static void handleBulletCollisions(
                    const std::shared_ptr<Renderer::Texture2D>& pTexture,
                    const std::shared_ptr<Renderer::ShaderProgram>& pShader,
                    std::vector<std::shared_ptr<Bullet>>& bullets,
                    Tank& playerTank,
                    std::vector<std::shared_ptr<Tank>>& enemyTanks,
                    std::vector<std::shared_ptr<Explosion>>& explosions,
                    const std::vector<std::shared_ptr<Tile>>& blockingTiles,
                    int& kills
                );
        static bool handleTankCollisions(const Tank& tank, const std::vector<std::shared_ptr<Tank>>& tanks, const std::vector<std::shared_ptr<Tile>>& blockingTiles);
};
