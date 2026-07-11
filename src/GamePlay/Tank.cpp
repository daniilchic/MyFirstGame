#include "Tank.h"
#include "Renderer/getTileUV.h"
#include "Bullet.h"
#include <glm/vec2.hpp>

Tank::Tank(const std::shared_ptr<Renderer::Texture2D>& pTexture,
           const std::shared_ptr<Renderer::ShaderProgram>& pShader,
           const glm::vec2& position,
           const int tankDamage,
           const glm::vec2& size,
           const float rotation)
           :Sprite(pTexture, pShader, position, size, rotation),
           m_tankDamage(tankDamage)
{
    glm::vec2 offset, scale;
    getTileUV(0, offset, scale);
    setUVRegion(offset, scale);   
}
Tank::~Tank(){

}

int Tank::getTankDamage() const {
    return m_tankDamage;
}
void Tank::setTankDamage(const int newDamage){
    if(newDamage > 0 && newDamage < 1000000){
        m_tankDamage = newDamage;
    }
}

std::shared_ptr<Bullet> Tank::shoot(){
    glm::vec2 dir;
    float rot = getRotation();
    if(rot == 0.0f){ dir = {0.0f, 1.0f}; }
    else if(rot == 90.0f){ dir = {-1.0f, 0.0f}; }
    else if(rot == 180.0f){ dir = {0.0f, -1.0f}; }
    else if(rot == 270.0f){ dir = {1.0f, 0.0f}; }
    else { dir = {0.0f, 1.0f}; }
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(getTexture(), getShader(), getPosition(), dir, m_tankDamage, getRotation());
    return bullet;
}
