#pragma once

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>



#include "Ball.h"
#include "Matrix.h"


class Pendulum {
    public:
        static constexpr double GRAVITY = 9.81;
        static constexpr double STR_LENGTH = .5; //m
        static constexpr double RENDER_STR_LENGTH = 0.2; //% of window
        static constexpr double RENDER_BALL_RADIUS = RENDER_STR_LENGTH / 10; //% of window

        static int RANDOMSEED;

        Pendulum(int);
        render(GLFWwindow*, double x, double y, double dT);

        void solve(double**);

    private:
        int nBalls;
        ball_t* balls;
        uint32_t rootColor;
        void matrixA(matrix_t* a);
        void matrixB(matrix_t* b);
        

};

typedef Pendulum pendulum_t;