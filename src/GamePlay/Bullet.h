#pragma once
#include "Renderer/Sprite.h"
#include "glm/vec2.hpp"

class Bullet : public Renderer::Sprite {
    public:
        Bullet(const std::shared_ptr<Renderer::Texture2D>& pTexture,
               const std::shared_ptr<Renderer::ShaderProgram>& pShader,
               const glm::vec2& position,
               const glm::vec2& direction, 
               const int damage = 1,
               const float speed = 300.f,
               const glm::vec2& size = glm::vec2(8.f, 8.f));
        ~Bullet() = default;

        void update(float deltaTime);
        bool isDestroyed() const { return m_destroyed; }

    private:
        glm::vec2 m_direction;
        int m_damage;
        float m_speed;
        float m_lifetime;
        bool m_destroyed;
};
