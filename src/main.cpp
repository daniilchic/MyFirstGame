
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
        
        auto sprite1 = std::make_shared<Renderer::Sprite>(tex, pShader, glm::vec2(100.0f, 200.0f), glm::vec2(50.0f, 50.0f), 0.f);
       
        auto sprite2 = std::make_shared<Renderer::Sprite>(tex, pShader, glm::vec2(500.0f, 200.0f), glm::vec2(100.0f, 100.0f), 45.f);
        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f); 
        while(!glfwWindowShouldClose(pWindow)){
		    glClear(GL_COLOR_BUFFER_BIT);
	
            sprite1->draw(projectionMatrix);
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
