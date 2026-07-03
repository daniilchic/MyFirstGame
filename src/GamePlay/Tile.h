#pragma once
#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include "Renderer/Sprite.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"

class Tile : public Renderer::Sprite {
    public:
        Tile(const std::shared_ptr<Renderer::Texture2D>& pTexture,
             const std::shared_ptr<Renderer::ShaderProgram>& pShader,
             const glm::vec2& position,
             const glm::vec2& size = glm::vec2(64.f, 64.f),
             const float rotation = 0.f)
             : Sprite(pTexture, pShader, position, size, rotation) {}
        virtual ~Tile() = default;
        virtual bool isPassable() const = 0;
};
