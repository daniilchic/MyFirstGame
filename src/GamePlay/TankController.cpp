#include "TankController.h"
#include "Utils/Collision.h"
#include "Bullet.h"
#include "Utils/CollisionSystem.h"

void TankController::update(Tank& tank, const std::vector<std::shared_ptr<Tank>>& tanks ,const std::vector<std::shared_ptr<Tile>>& blockingTiles, 
                            const InputData& input, const float deltaTime, const float speed)
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
    pos.y += moveY * moveSpeed;
    tank.setPosition(pos);
    if(CollisionSystem::handleTankCollisions(tank, tanks, blockingTiles)){
        pos = oldPos;
        tank.setPosition(pos);
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
