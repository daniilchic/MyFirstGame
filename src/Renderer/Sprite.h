#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace Renderer{
    class Texture2D;
    class ShaderProgram;

    class Sprite{
        public:  
            Sprite(const std::shared_ptr<Texture2D> pTexture,
                   const std::shared_ptr<ShaderProgram> pShaderProgram,
                   const glm::vec2& position = glm::vec2(0.f),
                   const glm::vec2& size = glm::vec2(1.f),
                   const float rotation = 0.f);
            ~Sprite();
            Sprite() = delete;
            Sprite(const Sprite&) = delete;
            Sprite& operator=(const Sprite&) = delete;

            glm::vec2 getPosition() const;
            void setPosition(const glm::vec2& newPosition);
            glm::vec2 getSize() const;
            void setSize(const glm::vec2& newSize);
            float getRotation() const;
            void setRotation(const float newRotation);

            void draw(const glm::mat4& projectionMatrix);
            static void initRenderData();
        private:
            static GLuint s_vao;
            static GLuint s_vbo_positions;
            static GLuint s_vbo_colors;
            static GLuint s_vbo_texCoords;
            std::shared_ptr<Texture2D> m_pTexture;
            std::shared_ptr<ShaderProgram> m_pShaderProgram;
            glm::vec2 m_position;
            glm::vec2 m_size;
            float m_rotation;
    };
}
