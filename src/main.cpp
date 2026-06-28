
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Sprite.h"
#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height);
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode);
void getTileUV(unsigned int tileID, glm::vec2& outOffset, glm::vec2& outScale);
glm::ivec2 g_windowSize(640, 480);

int main(int argc, char** argv){


    if(!glfwInit()){
	std::cout << "glfwInir failed" << std::endl;
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

    if(!gladLoadGL()){
        std::cout << "Can't load Glad" << std::endl;
    	return -1;
    }
    
    Renderer::Sprite::initRenderData();

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    glClearColor(0, 0, 0, 1);
    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex_shader.txt", "res/shaders/fragment_shader.txt"); 
        if(!pDefaultShaderProgram){
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }
        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");
        auto pShader = resourceManager.getShaderProgram("DefaultShader");
        
        auto sprite1 = std::make_shared<Renderer::Sprite>(tex, pShader, glm::vec2(100.0f, 320.0f), glm::vec2(64.0f, 64.0f), 0.f);
        auto sprite2 = std::make_shared<Renderer::Sprite>(tex, pShader, glm::vec2(540.0f, 320.0f), glm::vec2(64.0f, 64.0f), 0.f);
        glm::vec2 offset, scale;
        getTileUV(3, offset, scale);
        sprite1->setUVRegion(offset, scale);
        sprite2->setUVRegion(offset, scale);
        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        float speed = 100.0f; 
        float rotation1 = 0.0f;
        float rotation2 = 180.0f;
        float lastFrameTime = (float)glfwGetTime();
        while(!glfwWindowShouldClose(pWindow)){
            float currentFrameTime = (float)glfwGetTime();
            float deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            glm::vec2 pos1 = sprite1->getPosition();
            glm::vec2 pos2 = sprite2->getPosition();
		    glClear(GL_COLOR_BUFFER_BIT);
	        
            if(glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS){
                pos1.y += speed * deltaTime;
                pos2.y -= speed * deltaTime;
                rotation1 = 270.0f;
                rotation2 = 90.0f;
            }
            if(glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS){
                pos1.x -= speed * deltaTime;
                pos2.x += speed * deltaTime;
                rotation1 = 0.0f;
                rotation2 = 180.0f;
            }
            if(glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS){
                pos1.y -= speed * deltaTime;
                pos2.y += speed * deltaTime;
                rotation1 = 90.0f;
                rotation2 = 270.0f;
            }
            if(glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS){
                pos1.x += speed * deltaTime;
                pos2.x -= speed * deltaTime;
                rotation1 = 180.0f;
                rotation2 = 0.0f;
            }

            sprite1->setRotation(rotation1);
            sprite1->setPosition(pos1);
            sprite1->draw(projectionMatrix);
            sprite2->setRotation(rotation2);
            sprite2->setPosition(pos2);
            sprite2->draw(projectionMatrix);

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
void getTileUV(unsigned int tileID, glm::vec2& outOffset, glm::vec2& outScale) {
    
    const float TEX_W = 384.0f;
    const float TEX_H = 256.0f;
    const float TILE_SIZE = 16.0f;
    const int TILES_PER_ROW = static_cast<int>(TEX_W / TILE_SIZE);
     
    outScale.x = TILE_SIZE / TEX_W;
    outScale.y = TILE_SIZE / TEX_H;
    
    int col = tileID % TILES_PER_ROW;
    int row = tileID / TILES_PER_ROW;
 
    outOffset.x = (col * TILE_SIZE) / TEX_W;
    outOffset.y = 1.0f - ((row + 1) * TILE_SIZE) / TEX_H;
}
