
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(){

    if(!glfwInit()){
        return -1;
    }
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Game", NULL, NULL);
    
    if(!window){
        glfwTerminate();
	return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL()){
        std::cout << "Can't load Glad" << std::endl;
	return -1;
    }

    std::cout << "OpenGL " << GLVersion.major << "," << GLVersion.minor << std::endl;

    glClearColor(0, 1, 0, 1);

    while(!glfwWindowShouldClose(window)){
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(window);

	glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
