#include "TankController.h"
#include "Utils/Collision.h"

void TankController::update(Tank& tank, const std::vector<std::shared_ptr<Tile>>& blockingTiles, 
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
