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

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height);
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode);
glm::ivec2 g_windowSize(640, 640);

int main(int argc, char** argv){
    const int mapCols = 10;
    const int mapRows = 10;

    std::vector<std::vector<int>> mapData = //Game map, 0 - empty, 1 - wall, 2 - metal, 3 - bush, 4 - water 
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 3, 4, 1, 1, 1, 1},
        {1, 0, 1, 0, 3, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 2, 2, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 2, 2, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1}, // player is on 5 col
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
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
        auto tank = std::make_shared<Tank>(tex, pShader, worldPosition(5, 2), true, 1); //player tank
        std::vector<std::shared_ptr<Tank>> enemyTanks;
        auto& blockingTiles = gameMap.getBlockingTiles();
        auto& backgroundTiles = gameMap.getBackgroundTiles();
        auto& foregroundTiles = gameMap.getForegroundTiles();
        std::vector<std::shared_ptr<Bullet>> bullets;
        std::vector<std::shared_ptr<Bullet>> enemyBullets;
        std::vector<std::shared_ptr<Explosion>> explosions;
        float speed = 100.0f;
        float shootCooldown = 0.0f;
        float enemyShootCooldown = 0.0f;
        float respawnCooldown = 3.0f;
        float enemySpawnCooldown = 0.0f;
        float randomCooldown = 0.0f;
        float lastFrameTime = (float)glfwGetTime();
        int randomDir;
        while(!glfwWindowShouldClose(pWindow)){ //game loop
            glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f); //initialization projection matrix
            float currentFrameTime = (float)glfwGetTime();
            float deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
		    glClear(GL_COLOR_BUFFER_BIT);
	        shootCooldown -= deltaTime;
            enemySpawnCooldown -= deltaTime;
            enemyShootCooldown -= deltaTime;
            randomCooldown -= deltaTime;
            if(enemySpawnCooldown <= 0.0f){
               auto enemyTank = std::make_shared<Tank>(tex, pShader, worldPosition(5, 8), false, 1);
               enemyTanks.push_back(enemyTank);
               enemySpawnCooldown = 10.0f;
            }
            InputData input;
            if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) input.moveY = 1;
            if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) input.moveY = -1;
            if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) input.moveX = -1;
            if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) input.moveX = 1;
            for(auto& enemyTank : enemyTanks){
                InputData enemyInput;
                if(randomCooldown <= 0.0f){
                    randomDir = distr(gen);
                    randomCooldown = 0.3f;
                }
                std::cout << randomDir << std::endl;
                switch(randomDir){
                    case 0:
                        enemyInput.moveY = 1;
                        break;
                    case 1:
                        enemyInput.moveY = -1;
                        break;
                    case 2:
                        enemyInput.moveX = 1;
                        break;
                    case 3:
                        enemyInput.moveX = -1;
                        break;
                    case 4:
                        if(enemyShootCooldown <= 0.0f){
                            auto enemyBullet = enemyTank->shoot();
                            enemyBullets.push_back(enemyBullet);
                            enemyShootCooldown = 0.5f;
                        }
                    default:
                        break;
                }
                TankController::update(*enemyTank, blockingTiles, enemyInput, deltaTime);
                
            }
            if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS && shootCooldown <= 0){
                auto bullet = tank->shoot();
                bullets.push_back(bullet);
                shootCooldown = 0.5f;
            }

            TankController::update(*tank, blockingTiles, input, deltaTime);
            for(auto& bullet : bullets){
                bullet->update(deltaTime, blockingTiles);
            }
            for(auto& explosion : explosions){
                explosion->update(deltaTime);
            }
            for(auto& bullet : enemyBullets){
                bullet->update(deltaTime, blockingTiles);
            }

            for(auto& bullet : bullets){
                if(bullet->isDestroyed()){
                    auto explosion = std::make_shared<Explosion>(tex, pShader, bullet->getPosition());
                    explosions.push_back(explosion);
                }
            }
            for(auto& bullet : enemyBullets){
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
            enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                        [](const auto& b){return b->isDestroyed();}), enemyBullets.end());

            for(auto& explosion : explosions){
                explosion->draw(projectionMatrix);
            }
            tank->draw(projectionMatrix);
            for(auto& enemyTank : enemyTanks){
                enemyTank->draw(projectionMatrix);
            }
            for(auto& backgroundTile : backgroundTiles){
                backgroundTile->draw(projectionMatrix);
            }

            for(auto& bullet : bullets){
                bullet->draw(projectionMatrix);
            }
            for(auto& bullet : enemyBullets){
                bullet->draw(projectionMatrix);
            }

            for(auto& foregroundTile : foregroundTiles){
                foregroundTile->draw(projectionMatrix);
            }
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

