#include "Explosion.h"
#include "Renderer/getTileUV.h"

Explosion::Explosion(const std::shared_ptr<Renderer::Texture2D>& pTexture, const std::shared_ptr<Renderer::ShaderProgram>& pShader, const glm::vec2& position):
    Renderer::Sprite(pTexture, pShader, position),
    m_currentFrame(0),
    m_frameDuration(0.1f),
    m_animTimer(0.0f),
    m_finished(false)
{
    glm::vec2 offset, scale;
    getTileUV(160, offset, scale);
    setUVRegion(offset, scale);
}
void Explosion::update(const float deltaTime){
    if(m_finished) { return; }

    m_animTimer += deltaTime;

    if(m_animTimer >= m_frameDuration){
        m_currentFrame++;
        m_animTimer -= m_frameDuration;

        if(m_currentFrame >= 4){
            m_finished = true;
            return;
        }

        glm::vec2 offset, scale;
        getTileUV(160 + m_currentFrame, offset, scale);
        setUVRegion(offset, scale);
    }
}
bool Explosion::isFinished() const{
    return m_finished;
}
