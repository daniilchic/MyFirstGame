#pragma once
#include "Renderer/Sprite.h"
#include "Bullet.h"

class Tank : public Renderer::Sprite {
    public:
        Tank(const std::shared_ptr<Renderer::Texture2D>& pTexture,
             const std::shared_ptr<Renderer::ShaderProgram>& pShader,
             const glm::vec2& position,
             const bool isPlayer = true,
             const int tankDamage = 1,
             const int tankHealth = 3,
             const glm::vec2& size = glm::vec2(64.f, 64.f),
             const float rotation = 0.f);
        ~Tank();

        int getTankDamage() const;
        void setTankDamage(const int newDamage);
        int getTankHealth() const;
        void setTankHealth(const int newHealth);
        void damage(const int dmg);
        bool IsTankPlayer() const;
        std::shared_ptr<Bullet> shoot();

    private:
        int m_tankDamage;
        int m_tankHealth;
        bool m_isPlayer;
};
