#pragma once
#include "Renderer/Sprite.h"
#include "glm/vec2.hpp"
#include <vector>
#include <memory>
#include "Tile.h"

class Bullet : public Renderer::Sprite {
    public:
        Bullet(const std::shared_ptr<Renderer::Texture2D>& pTexture,
               const std::shared_ptr<Renderer::ShaderProgram>& pShader,
               const glm::vec2& position,
               const glm::vec2& direction, 
               const bool isBulletPlayer,
               const int damage = 1,
               const float rotation = 0.0f,
               const float speed = 300.f,
               const glm::vec2& size = glm::vec2(12.f, 12.f));
        ~Bullet() = default;

        void update(const float deltaTime);
        bool isDestroyed() const { return m_destroyed; }
        bool isBulletFromPlayer() const { return m_isBulletPlayer; }
        void destroy() { m_destroyed = true; }
        int getDamage() const { return m_damage; }

    private:
        glm::vec2 m_direction;
        int m_damage;
        float m_speed;
        float m_lifetime;
        bool m_destroyed;
        bool m_isBulletPlayer;
};
