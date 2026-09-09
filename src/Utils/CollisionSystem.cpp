#include "CollisionSystem.h"
#include "GamePlay/Bullet.h"
#include "GamePlay/Tank.h"
#include "GamePlay/Tile.h"
#include "GamePlay/Explosion.h"
#include "Utils/Collision.h"

void CollisionSystem::handleBulletCollisions(
        const std::shared_ptr<Renderer::Texture2D>& pTexture,
        const std::shared_ptr<Renderer::ShaderProgram>& pShader,
        std::vector<std::shared_ptr<Bullet>>& bullets,
        std::shared_ptr<Tank>& playerTank,
        std::vector<std::shared_ptr<Tank>>& enemyTanks,
        std::vector<std::shared_ptr<Explosion>>& explosions,
        const std::vector<std::shared_ptr<Tile>>& blockingTiles)
{
    for(auto& bullet : bullets){
        if(bullet->isDestroyed()) { continue; }

        //check tiles collisions
        bool hitWall = false;
        for(const auto& tile : blockingTiles){
            if(checkCollision(bullet->getRect(), tile->getRect()) && tile->getBulletBlock()){
                tile->damage(bullet->getDamage());
                bullet->destroy();
                explosions.push_back(std::make_shared<Explosion>(pTexture, pShader, bullet->getPosition()));
                hitWall = true;
                break;
            }
        }
        if(hitWall){ continue; }

        //check Tanks collisions

        if(!bullet->isBulletFromPlayer()){
            if(playerTank && !playerTank->isTankDestroyed() && checkCollision(bullet->getRect(), playerTank->getRect())){
                playerTank->damage(bullet->getDamage());
                bullet->destroy();
                explosions.push_back(std::make_shared<Explosion>(pTexture, pShader, bullet->getPosition()));
            }
        }
        else{
            for(auto& enemy : enemyTanks){
                if(!enemy->isTankDestroyed() && checkCollision(enemy->getRect(), bullet->getRect())){
                    enemy->damage(bullet->getDamage());
                    bullet->destroy();
                    explosions.push_back(std::make_shared<Explosion>(pTexture, pShader, bullet->getPosition()));
                    break;
                }
            }
        }
    }
}
