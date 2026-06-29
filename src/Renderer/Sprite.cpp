#include <glm/gtc/matrix_transform.hpp>
#include "Texture2D.h"
#include "ShaderProgram.h"
#include "Sprite.h"

namespace Renderer{
    
    GLuint Sprite::s_vao = 0;
    GLuint Sprite::s_vbo_positions = 0;
    GLuint Sprite::s_vbo_colors = 0;
    GLuint Sprite::s_vbo_texCoords = 0;

    Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
                   const std::shared_ptr<ShaderProgram> pShaderProgram,
                   const glm::vec2& position,
                   const glm::vec2& size,
                   const float rotation)
                   : m_pTexture(pTexture)
                   , m_pShaderProgram(pShaderProgram)
                   , m_position(position)
                   , m_size(size)
                   , m_rotation(rotation) 
                   , m_uvOffset(0.0f, 0.0f)
                   , m_uvScale(1.0f, 1.0f)
    {
        
    }
    
    Sprite::~Sprite() = default;

    glm::vec2 Sprite::getPosition() const{
        return m_position;
    }
    void Sprite::setPosition(const glm::vec2& newPosition){
        m_position = newPosition;
    }
    glm::vec2 Sprite::getSize() const{
        return m_size;
    }
    void Sprite::setSize(const glm::vec2& newSize){
        m_size = newSize;
    }
    float Sprite::getRotation() const{
        return m_rotation;
    }
    void Sprite::setRotation(const float newRotation){
        m_rotation = newRotation;
    }

    void Sprite::initRenderData(){
        GLfloat points[] = {
            -0.5f,  0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,

             0.5f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f
        };
        GLfloat colors[] = {
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f
        };
        GLfloat texCoords[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f
        };
        glGenVertexArrays(1, &s_vao);

        glGenBuffers(1, &s_vbo_positions);
        glGenBuffers(1, &s_vbo_colors);
        glGenBuffers(1, &s_vbo_texCoords);

        glBindVertexArray(s_vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, s_vbo_positions);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, s_vbo_colors);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, s_vbo_texCoords);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(0);

    } 
    
    void Sprite::draw(const glm::mat4& projectionMatrix){
        m_pShaderProgram->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(m_position, 0.0f));
        model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(m_size, 1.0f));

        m_pShaderProgram->setMatrix4("projectionMat", projectionMatrix);
        m_pShaderProgram->setMatrix4("modelMat", model);
        
        m_pTexture->bind(0);
        glBindVertexArray(s_vao);
        m_pShaderProgram->setVec2("u_uvOffset", m_uvOffset);
        m_pShaderProgram->setVec2("u_uvScale", m_uvScale);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Sprite::setUVRegion(const glm::vec2& offset, const glm::vec2& scale){
        m_uvOffset = offset;
        m_uvScale = scale;
    }

    Sprite::Rect Sprite::getRect() const{
        float margin = 2.0f;
        float halfW = m_size.x * 0.5f;
        float halfH = m_size.y * 0.5f;
        return { m_position.x - halfW + margin, m_position.y - halfH + margin, (halfW-margin)*2, (halfH-margin)*2 };
    }
}
