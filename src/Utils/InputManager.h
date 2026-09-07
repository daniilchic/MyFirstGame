#pragma once
#include "GamePlay/InputData.h"
#include "GLFW/glfw3.h"

InputData readInput(GLFWwindow* pWindow){
    InputData input;
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) input.moveY = 1;
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) input.moveY = -1;
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) input.moveX = -1;
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) input.moveX = 1;
    if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS) input.fire = true;
    return input;
}
