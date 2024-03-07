#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "SpaceInvaders.h"
#include "GridLayoutHelper.h"

#define GLFW_INCLUDE_GLCOREARB

//Originally set to 800x600
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int key, int action, int mode);

SpaceInvaders Game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(void){
    GridLayoutHelper::CreateGridLayoutForEnemies();
    // Initialize the library
    if (!glfwInit())
        return -1;

    //Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current
    glfwMakeContextCurrent(window);

    //Init GLEW
    if (glewInit() != GLEW_OK)
        std::cout << "Error in GLEW" << std::endl;

    // Set keys callback for Up and Down
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Game.Init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    bool bound = false;
    //Game Loop    

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Poll for and process events
        glfwPollEvents();

        // Process inputs here
        Game.ProcessInput(deltaTime);

        // Update
        Game.Update(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT);

        // Render
        Game.Render();

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, int key, int action, int mode){
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Game.Mouse[key] = true;
        else if (action == GLFW_RELEASE)
            Game.Mouse[key] = false;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
               Game.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            Game.Keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // Make sure the viewport matches the new window dimensions
    // Note that width and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}