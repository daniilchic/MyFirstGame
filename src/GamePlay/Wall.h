#pragma once
#include "Tile.h"

class Wall : public Tile {
    public:
           Wall(const std::shared_ptr<Renderer::Texture2D>& pTexture,
                const std::shared_ptr<Renderer::ShaderProgram>& pShader,
                const glm::vec2& position,
                const bool isMetal = false,
                const glm::vec2& size = glm::vec2(64.f, 64.f),
                const float rotation = 0.f,
                const bool bulletBlock = true);
           ~Wall();
           bool isPassable() const override;
};
