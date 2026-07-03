#include "Wall.h"
#include "../Renderer/getTileUV.h"

Wall::Wall(const std::shared_ptr<Renderer::Texture2D>& pTexture,
           const std::shared_ptr<Renderer::ShaderProgram>& pShader,
           const glm::vec2& position,
           const bool isMetal,
           const glm::vec2& size, 
           const float rotation)
           : Tile(pTexture, pShader, position, size, rotation)
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
