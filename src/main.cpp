#include <vector>
#include <iostream> //std libraries
#include <algorithm>                    
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> //visual programming libraries

#include "Renderer/Sprite.h"
#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/getTileUV.h"
#include "GamePlay/Tank.h"
#include "GamePlay/Wall.h"
#include "GamePlay/GameMap.h"
#include "Utils/WorldPosition.h"
#include "GamePlay/TankController.h"
#include "GamePlay/Explosion.h"
#include "GamePlay/InputData.h" // my files
#include "Utils/InputManager.h"
#include "Utils/CollisionSystem.h"
#include "GamePlay/findRandomFreeCell.h"

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height);
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode);
glm::ivec2 g_windowSize(640, 640);

int main(int argc, char** argv){
    const int mapCols = 10;
    const int mapRows = 10;

    std::vector<std::vector<int>> mapData = //Game map, 0 - empty, 1 - wall, 2 - metal, 3 - bush, 4 - water 
    {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 0, 0, 1, 0, 0, 1, 0, 0, 2},
        {2, 0, 0, 1, 2, 2, 1, 0, 0, 2},
        {2, 0, 0, 0, 3, 3, 0, 0, 0, 2},
        {2, 1, 1, 1, 1, 1, 3, 4, 3, 2},
        {2, 3, 4, 3, 1, 1, 1, 1, 1, 2},
        {2, 0, 0, 0, 3, 3, 0, 0, 0, 2},
        {2, 0, 0, 1, 2, 2, 1, 0, 0, 2},
        {2, 0, 0, 1, 0, 0, 1, 0, 0, 2}, // player is on 5 col
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };
    if(!glfwInit()){
	std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Game", nullptr, nullptr); 
    
    if(!pWindow){ 
    	std::cout << "glfwCreateWindow failed" << std::endl;
        glfwTerminate();
    	return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(1);
    if(!gladLoadGL()){
        std::cout << "Can't load Glad" << std::endl;
    	return -1;
    }
    
    Renderer::Sprite::initRenderData(); // initialization of Sprites Size, color, texture

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    glClearColor(0, 0, 0, 1);
    {
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> distr(0, 4);

        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex_shader.txt", "res/shaders/fragment_shader.txt"); //shaders
        if(!pDefaultShaderProgram){
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }
        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");
        auto pShader = resourceManager.getShaderProgram("DefaultShader");
        GameMap gameMap(mapData, tex, pShader);
        auto tank = std::make_shared<Tank>(tex, pShader, worldPosition(5, 2), true, 1, 3); //player tank
        int kills = 0;
        int savedKills = 0;
        int waves = 1;
        std::vector<std::shared_ptr<Tank>> enemyTanks;
        auto& blockingTiles = gameMap.getBlockingTiles();
        auto& backgroundTiles = gameMap.getBackgroundTiles();
        auto& foregroundTiles = gameMap.getForegroundTiles();
        std::vector<std::shared_ptr<Bullet>> bullets;
        std::vector<std::shared_ptr<Explosion>> explosions;
        float speed = 100.0f;
        float respawnCooldown = 3.0f;
        float enemySpawnCooldown = 0.0f;
        float lastFrameTime = (float)glfwGetTime();
        while(!glfwWindowShouldClose(pWindow)){ //game loop
            glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f); //initialization projection matrix
            float currentFrameTime = (float)glfwGetTime();
            float deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
		    glClear(GL_COLOR_BUFFER_BIT);
            enemySpawnCooldown -= deltaTime;
            if(enemySpawnCooldown <= 0.0f){
                bool spawned = false;
                for(int i = 0; i < waves/10 + 1; i++){
                    glm::ivec2 freeCell = findRandomFreeCell(blockingTiles, tank, gen);
                    if(freeCell.x > 0 && freeCell.y > 0){
                        spawned = true;
                        auto enemyTank = std::make_shared<Tank>(tex, pShader, worldPosition(freeCell.x, freeCell.y), false, 1 + waves * (1 + waves/40) / 6, 3 + waves * (1 + waves/20) / 2);
                        enemyTanks.push_back(enemyTank);
                        std::cout << "Enemy tank was spawned, wave - " << waves << std::endl;
                        enemySpawnCooldown = 5.0f;
                    }
                    else{
                        enemySpawnCooldown = 0.5f;
                    }
                }
                if(spawned) { waves++; }
            }
            if(tank->isTankDestroyed()){
                kills = kills/2;
                savedKills = savedKills/2;
                respawnCooldown-=deltaTime;
                if(respawnCooldown <= 0){

                    tank->respawn(worldPosition(5,2), 3+kills*(1 + waves/20), 1+kills/3*(1 + waves/30));
                    respawnCooldown = 5.0f;
                }
            }

            InputData input = readInput(pWindow);
            tank->update(deltaTime, input, bullets);
            for(auto& enemyTank : enemyTanks){
                InputData enemyInput;
                enemyTank->aiMove(enemyInput, gen);
                enemyTank->update(deltaTime, enemyInput, bullets);
                TankController::update(*enemyTank, {tank}, blockingTiles, enemyInput, deltaTime);
            }

            TankController::update(*tank, enemyTanks, blockingTiles, input, deltaTime);
            for(auto& bullet : bullets){
                bullet->update(deltaTime);
            }
            CollisionSystem::handleBulletCollisions(tex, pShader, bullets, *tank, enemyTanks, explosions, blockingTiles, kills);
            if(!tank->isTankDestroyed() && kills > savedKills){
                int killDif = kills - savedKills;
                tank->setTankHealth(tank->getTankHealth() + killDif * (1 + waves / 20));
                tank->setTankDamage(1 + (kills / 3)*(1 + waves / 40));
                savedKills = kills;
            }
            for(auto& explosion : explosions){
                explosion->update(deltaTime);
            }

            for(auto& bullet : bullets){
                if(bullet->isDestroyed()){
                    auto explosion = std::make_shared<Explosion>(tex, pShader, bullet->getPosition());
                    explosions.push_back(explosion);
                }
            }

            blockingTiles.erase(std::remove_if(blockingTiles.begin(), blockingTiles.end(),
                        [](const auto& b){return b->isDestroyed();}), blockingTiles.end());
            backgroundTiles.erase(std::remove_if(backgroundTiles.begin(), backgroundTiles.end(),
                        [](const auto& b){return b->isDestroyed();}), backgroundTiles.end());
	        foregroundTiles.erase(std::remove_if(foregroundTiles.begin(), foregroundTiles.end(),
		        [](const auto& b){return b->isDestroyed();}), foregroundTiles.end());
            explosions.erase(std::remove_if(explosions.begin(), explosions.end(),
                        [](const auto& b){return b->isFinished();}), explosions.end());
            bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                        [](const auto& b){return b->isDestroyed();}), bullets.end());
            enemyTanks.erase(std::remove_if(enemyTanks.begin(), enemyTanks.end(),
                        [](const auto& b){return b->isTankDestroyed();}), enemyTanks.end());

            for(auto& explosion : explosions){
                explosion->draw(projectionMatrix);
            }
            if(!tank->isTankDestroyed()){
                tank->draw(projectionMatrix);
            }
            for(auto& enemyTank : enemyTanks){
                enemyTank->draw(projectionMatrix);
            }
            for(auto& backgroundTile : backgroundTiles){
                backgroundTile->draw(projectionMatrix);
            }

            for(auto& bullet : bullets){
                bullet->draw(projectionMatrix);
            }

            for(auto& foregroundTile : foregroundTiles){
                foregroundTile->draw(projectionMatrix);
            }

            std::cout << "kills - " << kills << std::endl;
            std::cout << "HP - " << tank->getTankHealth() << std::endl;
            std::cout << "damage - " << tank->getTankDamage() << std::endl;
		    glfwSwapBuffers(pWindow);

		    glfwPollEvents();
        }
    }

   	glfwDestroyWindow(pWindow);
    glfwTerminate();

  	return 0;
    
}

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height){
   	g_windowSize.x = width;
   	g_windowSize.y = height;
   	glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode){
   	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
       	glfwSetWindowShouldClose(pWindow, GL_TRUE);
   	}
}

