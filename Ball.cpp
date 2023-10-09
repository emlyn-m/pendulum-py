#include <cstdint>
#include <stdlib.h>
#include <chrono>

#include "Ball.h"
#include "Pendulum.h"
#include "Matrix.h"

Ball::Ball(double* _parentX, double* _parentY) {

    ngVel = 0.0;
    ngAccel = 0.0;

    angle = 2.0 * M_PI * ((double) rand()) / RAND_MAX;
    angle = M_PI / 2;
    x = *_parentX + Pendulum::STR_LENGTH * std::sin(angle);
    y = *_parentY - Pendulum::STR_LENGTH * std::cos(angle);


    *_parentX = x;
    *_parentY = y;

    pastX = (double*) malloc(N_PAST * sizeof(double));
    pastY = (double*) malloc(N_PAST * sizeof(double));


    uint32_t chR = (rand() % 255) << 24;
    uint32_t chG = (rand() % 255) << 16;
    uint32_t chB = (rand() % 255) << 8;
    uint64_t chA = 0xc0;
    color = (((chR | chG) | chB) | chA);
}


void Ball::tick(double* parentX, double* parentY, double dt) {
    ngVel += ngAccel * dt;
    angle += ngVel * dt;

    x = *parentX + Pendulum::STR_LENGTH * std::sin(angle);
    y = *parentY + Pendulum::STR_LENGTH * std::cos(angle);

    

    *parentX = x;
    *parentY = y;
}


void Ball::render(GLFWwindow* window, double windowX, double windowY) {

    trueX = windowX + x * Pendulum::RENDER_STR_LENGTH;
    trueY = windowY - y * Pendulum::RENDER_STR_LENGTH;

    for (int i=N_PAST-2; i >= 0; i--) {
        pastX[i+1] = pastX[i];
        pastY[i+1] = pastY[i];

        uint32_t col = color & 0xffffff00;
        col = col | ((N_PAST - i) * 0xa0 / N_PAST);
        // drawCircle(pastX[i+1], pastY[i+1], Pendulum::RENDER_BALL_RADIUS, col);
    }

    pastX[0] = trueX;
    pastY[0] = trueY;

    drawCircle(trueX, trueY, Pendulum::RENDER_BALL_RADIUS, color);
}

