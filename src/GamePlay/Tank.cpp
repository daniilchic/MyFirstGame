#include "Tank.h"
#include "../Renderer/getTileUV.h"

Tank::Tank(const std::shared_ptr<Renderer::Texture2D>& pTexture,
           const std::shared_ptr<Renderer::ShaderProgram>& pShader,
           const glm::vec2& position,
           const glm::vec2& size,
           const float rotation)
           :Sprite(pTexture, pShader, position, size, rotation)
{
    glm::vec2 offset, scale;
    getTileUV(0, offset, scale);
    setUVRegion(offset, scale);   
}
Tank::~Tank(){

}
