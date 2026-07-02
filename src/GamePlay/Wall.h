#pragma once
#include "../Renderer/Sprite.h"

class Wall : public Renderer::Sprite {
    public:
           Wall(const std::shared_ptr<Renderer::Texture2D>& pTexture,
                const std::shared_ptr<Renderer::ShaderProgram>& pShader,
                const glm::vec2& position,
                const glm::vec2& size = glm::vec2(64.f, 64.f),
                const float rotation = 0.f);
           ~Wall();
};
