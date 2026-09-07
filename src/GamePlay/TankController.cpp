#include "TankController.h"
#include "Utils/Collision.h"
#include "Bullet.h"

void TankController::update(Tank& tank, const std::vector<std::shared_ptr<Tile>>& blockingTiles, 
                            const InputData& input, const float deltaTime, std::vector<std::shared_ptr<Bullet>>& bullets, const float speed)
{
    float moveSpeed = deltaTime * speed;
    char moveX = input.moveX;
    char moveY = input.moveY;

    if(moveX != 0 && moveY != 0){
        moveX = 0;
    }

    glm::vec2 oldPos = tank.getPosition();
    glm::vec2 pos = oldPos;

    pos.x += moveX * moveSpeed;
    tank.setPosition(pos);
    if(checkAllCollisions(tank, blockingTiles)){
        pos.x = oldPos.x;
        tank.setPosition(pos);
    }

    pos.y += moveY * moveSpeed;
    tank.setPosition(pos);
    if(checkAllCollisions(tank, blockingTiles)){
        pos.y = oldPos.y;
        tank.setPosition(pos);
    }

    if(input.fire && tank.getShootCooldown() <= 0){
        std::shared_ptr<Bullet> bullet = tank.shoot();
        bullets.push_back(bullet);
        tank.setShootCooldown(0.5f);
    }


    if(moveY > 0){
        tank.setRotation(0.0f);
    }
    else if(moveY < 0){
        tank.setRotation(180.0f);
    }
    else if(moveX > 0){
        tank.setRotation(270.0f);
    }
    else if(moveX < 0){
        tank.setRotation(90.0f);
    }
}
