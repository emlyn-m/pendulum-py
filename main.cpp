#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <cstdlib>
#include <chrono>

// #define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
// #include <glad/gl.h>

#include "Ball.h"
#include "Matrix.h"
#include "Pendulum.h"

constexpr int N_BALLS = 10;
constexpr double TIMESCALE = 1;

void glfw_error(int err, const char* description) {
    fputs(description, stderr);
}

// keypress handler
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char* argv[]) {

    glfwSetErrorCallback(glfw_error);
    if (!glfwInit()) { exit(EXIT_FAILURE); }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA - remove if needed for framerate
    
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "peNd", NULL, NULL);

    if (!window) { exit(EXIT_FAILURE); }
    glfwMakeContextCurrent(window); // make context current
    glfwSetKeyCallback(window, key_callback); // register keypress listener
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND); // enable rgba mode

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);


    // setup everything
    uint64_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    srand(microseconds_since_epoch);
    pendulum_t pend = Pendulum(N_BALLS);

    // main loop
    double previousTime;
    double currentTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // get events
        glfwSwapInterval(0);

        glViewport(0, 0, width, height);
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Measure speed
        previousTime = currentTime;
        currentTime = glfwGetTime();

        // begin logic
        pend.render(window, 0, .25, currentTime - previousTime);
        // end logic

        glfwSwapBuffers(window);
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();

}