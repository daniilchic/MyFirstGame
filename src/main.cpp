
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"

const float TILE_SIZE      = 16.0f;
const float TEXTURE_SIZE   = 128.0f;
const float TILE_UV        = TILE_SIZE / TEXTURE_SIZE;
const int   TILES_PER_ROW  = static_cast<int>(TEXTURE_SIZE  / TILE_SIZE);
const int   TOTAL_TILES    = TILES_PER_ROW * TILES_PER_ROW;

int g_currentTile = 0;
glm::vec2 texOffset(0.0f, 0.0f);

GLfloat point[] = {
    -0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,

    -0.5f,   0.5f, 0.0f,
     0.5f,   0.5f, 0.0f,
     0.5f,  -0.5f, 0.0f
}; 

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat textureCoordinats[] = {
    0.0f,       TILE_UV,   // левый верх
    TILE_UV,    0.0f,      // правый низ
    0.0f,       0.0f,      // левый низ
    0.0f,       TILE_UV,   // левый верх
    TILE_UV,    TILE_UV,   // правый верх
    TILE_UV,    0.0f       // правый низ
};


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

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint textureCoordinats_vbo = 0;
        glGenBuffers(1, &textureCoordinats_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, textureCoordinats_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinats), textureCoordinats, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, textureCoordinats_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);
            
        while(!glfwWindowShouldClose(pWindow)){
		    glClear(GL_COLOR_BUFFER_BIT);

            
            pDefaultShaderProgram->use();
            pDefaultShaderProgram->setVec2("u_texOffset", texOffset);
	
	    	glBindVertexArray(vao);
            tex->bind();
	    	glDrawArrays(GL_TRIANGLES, 0, 6);

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
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        g_currentTile = (g_currentTile + 1) % TOTAL_TILES;
        int col = g_currentTile % TILES_PER_ROW;
        int row = g_currentTile / TILES_PER_ROW;
        texOffset = glm::vec2(col * TILE_UV, row * TILE_UV);
    }
}

