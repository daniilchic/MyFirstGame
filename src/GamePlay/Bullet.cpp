#include "Bullet.h"
#include "Renderer/getTileUV.h"

Bullet::Bullet(const std::shared_ptr<Renderer::Texture2D>& pTexture,
               const std::shared_ptr<Renderer::ShaderProgram>& pShader,
               const glm::vec2& position,
               const glm::vec2& direction,
               const int damage,
               const float rotation,
               const float speed,
               const glm::vec2& size)
               :Renderer::Sprite(pTexture, pShader, position, size, rotation)
               ,m_direction(direction)
               ,m_speed(speed)
               ,m_damage(damage)
               ,m_lifetime(3.0f)
               ,m_destroyed(false) 
{
    glm::vec2 offset, scale;
    getTileUV(164, offset, scale);

    float x_left = 3.0f/384.0f;
    float x_right = 10.0f/384.0f;
    float y_top = 7.0f/256.0f;
    float y_bot = 6.0f/256.0f;

    offset.x += x_left;
    offset.y += y_top;
    
    scale.x -= (x_left + x_right);
    scale.y -= (y_top + y_bot);

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

