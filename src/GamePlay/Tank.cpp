#include "Tank.h"
#include "Renderer/getTileUV.h"
#include "Bullet.h"
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

Tank::Tank(const std::shared_ptr<Renderer::Texture2D>& pTexture,
           const std::shared_ptr<Renderer::ShaderProgram>& pShader,
           const glm::vec2& position,
           const bool isPlayer,
           const int tankDamage,
           const int tankHealth,
           const glm::vec2& size,
           const float rotation)
           :Sprite(pTexture, pShader, position, size, rotation),
           m_tankDamage(tankDamage),
           m_tankHealth(tankHealth),
           m_isPlayer(isPlayer),
           m_shootCooldown(0.5f),
           m_aiCooldown(0.3f),
           m_aiDirection(0)
{
    glm::vec2 offset, scale;
    getTileUV(0, offset, scale);
    setUVRegion(offset, scale);   
}
Tank::~Tank(){

}

int Tank::getTankDamage() const {
    return m_tankDamage;
}
void Tank::setTankDamage(const int newDamage){
    if(newDamage > 0 && newDamage < 1000000){
        m_tankDamage = newDamage;
    }
}

int Tank::getTankHealth() const{
    return m_tankHealth;
}
void Tank::setTankHealth(const int newHealth){
    m_tankHealth = newHealth;
}
void Tank::damage(const int dmg){
    setTankHealth(getTankHealth() - dmg);
}
bool Tank::isTankPlayer() const{
    return m_isPlayer;
}
float Tank::getShootCooldown() const{
    return m_shootCooldown;
}
void Tank::setShootCooldown(const float newCooldown) {
    m_shootCooldown = newCooldown;
}
float Tank::getAiCooldown() const{
    return m_aiCooldown;
}
void Tank::setAiCooldown(const float newCooldown){
    m_aiCooldown = newCooldown;
}
int Tank::getAiDirection() const{
    return m_aiDirection;
}
void Tank::setAiDirection(const int newDirection){
    m_aiDirection = newDirection;
}

void Tank::playerMove(InputData& input, GLFWwindow* pWindow, std::vector<std::shared_ptr<Bullet>>& bullets){
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) input.moveY = 1;
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) input.moveY = -1;
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) input.moveX = -1;
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) input.moveX = 1;
    if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS && getShootCooldown() <= 0){
        auto bullet = shoot();
        bullets.push_back(bullet);
        setShootCooldown(0.5f);
    }

}
void Tank::aiMove(InputData& input, std::mt19937& gen, std::vector<std::shared_ptr<Bullet>>& enemyBullets){
    if(getAiCooldown() <= 0.0f){
        std::uniform_int_distribution<> distr(0, 4);
        setAiDirection(distr(gen));
        setAiCooldown(0.3f);
    }
    switch(getAiDirection()){
        case 0:
            input.moveY = 1;
            break;
        case 1:
            input.moveY = -1;
            break;
        case 2:
            input.moveX = 1;
            break;
        case 3:
            input.moveX = -1;
            break;
        case 4:
            if(getShootCooldown() <= 0.0f){
                auto enemyBullet = shoot();
                enemyBullets.push_back(enemyBullet);
                setShootCooldown(0.5f);
            }
        default:
            break;
    }
}
std::shared_ptr<Bullet> Tank::shoot(){
    glm::vec2 dir;
    float rot = getRotation();
    if(rot == 0.0f){ dir = {0.0f, 1.0f}; }
    else if(rot == 90.0f){ dir = {-1.0f, 0.0f}; }
    else if(rot == 180.0f){ dir = {0.0f, -1.0f}; }
    else if(rot == 270.0f){ dir = {1.0f, 0.0f}; }
    else { dir = {0.0f, 1.0f}; }
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(getTexture(), getShader(), getPosition(), dir, getTankDamage(), getRotation());
    return bullet;
}
