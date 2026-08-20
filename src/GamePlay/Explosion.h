#pragma once
#include "Renderer/Sprite.h"

class Explosion : public Renderer::Sprite{
    public:
        Explosion(const std::shared_ptr<Renderer::Texture2D>& pTexture, const std::shared_ptr<Renderer::ShaderProgram>& pShader, const glm::vec2& position);

        void update(float deltaTime);
        bool isFinished() const;

    private:
        int m_currentFrame;
        float m_frameDuration;
        float m_animTimer;
        bool m_finished;
};
