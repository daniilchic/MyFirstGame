#include "Wall.h"
#include "../Renderer/getTileUV.h"
#include <iostream>
Wall::Wall(const std::shared_ptr<Renderer::Texture2D>& pTexture,
           const std::shared_ptr<Renderer::ShaderProgram>& pShader,
           const glm::vec2& position,
           const bool isMetal,
	   const int health,
           const glm::vec2& size, 
           const float rotation,
           const bool bulletBlock)
           : Tile(pTexture, pShader, position, health, size, rotation, bulletBlock),
	   m_isMetal(isMetal)
{
    
    glm::vec2 offset, scale;
    
    if(isMetal){
        getTileUV(40, offset, scale);
    }
    else{
        getTileUV(16, offset, scale);
    }
    setUVRegion(offset, scale);
}
Wall::~Wall(){
 
}
bool Wall::isPassable() const{
    return false;
}
void Wall::damage(const int dmg){
    if(m_isMetal){
        return;
    }
    std::cout << "WALL WAS HIT" << std::endl;
    std::cout << "HP BEFORE " << getHealth() << std::endl;
    setHealth(getHealth() - dmg);
    std::cout << "HP AFTER " << getHealth() << std::endl;
    if(getHealth() <= 0){
        destroy();
	    std::cout << "WALL WAS DESTROYED" << std::endl;
    }
}
