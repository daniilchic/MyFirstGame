#pragma once
#include "Tile.h"

class Bush : public Tile {
    public:
        Bush(const std::shared_ptr<Renderer::Texture2D>& pTexture,
             const std::shared_ptr<Renderer::ShaderProgram>& pShader,
             const glm::vec2& position,
             const glm::vec2& size = glm::vec2(64.f, 64.f),
             const float rotation = 0,
             const bool bulletBlock = false);
        ~Bush() = default;
        bool isPassable() const override;
};
