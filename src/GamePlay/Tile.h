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
	     const int health = 1, 
             const glm::vec2& size = glm::vec2(64.f, 64.f),
             const float rotation = 0.0f,
             const bool bulletBlock = true)
             : Sprite(pTexture, pShader, position, size, rotation)
             ,m_bulletBlock(bulletBlock)
	     ,m_health(health)
             ,m_isDestroyed(false)
        {

        }
        virtual ~Tile() = default;
        virtual bool isPassable() const = 0;

        bool getBulletBlock() const {
            return m_bulletBlock;
        }
        void setBulletBlock(bool newBlock){
            m_bulletBlock = newBlock;
        }
	int getHealth() const{
            return m_health;
	}
	void setHealth(const int newHealth){
            m_health = newHealth;
	}
	virtual void damage(const int dmg){
            setHealth(getHealth() - dmg);
	    if(m_health <= 0){
                m_isDestroyed = true;
	    }
	}
	bool isDestroyed() const{
            return m_isDestroyed;
	}
	void destroy(){
            m_isDestroyed = true;
	}
    private:
        bool m_bulletBlock;
	int m_health;
	bool m_isDestroyed;
};
