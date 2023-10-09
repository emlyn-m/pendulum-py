#pragma once

#include "Matrix.h"
#include "glfwUtils.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <random>
#include <cmath>

class Ball {


    public:

        static constexpr double MASS = 0.5; //kg

        Ball(double*, double*);
        void tick(double*, double*, double);
        void render(GLFWwindow*, double, double);

        double x;
        double y;
        double trueX;
        double trueY;

        double ngAccel;
        double ngVel;
        double angle;

        uint64_t color;

    private:
        static constexpr int N_PAST = 8;

        double* pastX;
        double* pastY;
};

typedef Ball ball_t;