#pragma once
#include "Renderer/Sprite.h"
#include <GLFW/glfw3.h>
#include "Bullet.h"
#include "InputData.h"
#include <random>

class Tank : public Renderer::Sprite {
    public:
        Tank(const std::shared_ptr<Renderer::Texture2D>& pTexture,
             const std::shared_ptr<Renderer::ShaderProgram>& pShader,
             const glm::vec2& position,
             const bool isPlayer,
             const int tankDamage = 1,
             const int tankHealth = 3,
             const glm::vec2& size = glm::vec2(64.f, 64.f),
             const float rotation = 0.f);
        ~Tank();

        void respawn(const glm::vec2& pos, const int health);
        int getTankDamage() const;
        void setTankDamage(const int newDamage);
        int getTankHealth() const;
        void setTankHealth(const int newHealth);
        void damage(const int dmg);
        bool isTankPlayer() const;
        float getShootCooldown() const;
        void setShootCooldown(const float newCooldown);
        float getAiCooldown() const;
        void setAiCooldown(const float newCooldown);
        int getAiDirection() const;
        void setAiDirection(const int newDirection);
        bool isTankDestroyed() const;

        void update(const float deltaTime, const InputData& input, std::vector<std::shared_ptr<Bullet>>& bullets);
        void aiMove(InputData& input, std::mt19937& gen);
        std::shared_ptr<Bullet> shoot();

    private:
        int m_tankDamage;
        int m_tankHealth;
        bool m_isPlayer;
        float m_shootCooldown;
        float m_aiCooldown;
        int m_aiDirection;
        bool m_isDestroyed;
};
