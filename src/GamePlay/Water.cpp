#include "Water.h"
#include "Renderer/getTileUV.h"
#include <glm/vec2.hpp>

Water::Water(const std::shared_ptr<Renderer::Texture2D>& pTexture,
             const std::shared_ptr<Renderer::ShaderProgram>& pShader,
             const glm::vec2& position,
             const glm::vec2& size,
             const float rotation,
             const bool bulletBlock) 
             : Tile(pTexture, pShader, position, 1, size, rotation, bulletBlock)
{
    glm::vec2 offset, scale;
    getTileUV(88, offset, scale);
    setUVRegion(offset, scale);
}
bool Water::isPassable() const {
    return false;
}
