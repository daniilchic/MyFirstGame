#include "Bullet.h"
#include "Renderer/getTileUV.h"

Bullet::Bullet(const std::shared_ptr<Renderer::Texture2D>& pTexture,
               const std::shared_ptr<Renderer::ShaderProgram>& pShader,
               const glm::vec2& position,
               const glm::vec2& direction,
               const int damage,
               const float speed,
               const glm::vec2& size)
               :Renderer::Sprite(pTexture, pShader, position, size)
               ,m_direction(direction)
               ,m_speed(speed)
               ,m_damage(damage)
               ,m_lifetime(3.0f)
               ,m_destroyed(false) 
{
    glm::vec2 offset, scale;
    getTileUV(164, offset, scale);
    setUVRegion(offset, scale);
}

void Bullet::update(float deltaTime){
    if (m_destroyed) {return;}

    glm::vec2 newPosition = getPosition();
    newPosition.x += m_direction.x * deltaTime * m_speed;
    newPosition.y += m_direction.y * deltaTime * m_speed;
    setPosition(newPosition);

    m_lifetime -= deltaTime;
    if(m_lifetime <= 0.0f){m_destroyed = true;}
}

bool Bullet::isDestroyed() const{
    return m_destroyed;
}
