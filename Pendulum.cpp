#include <cstdint>
#include <random>
#include <stdio.h>

#include "Pendulum.h"
#include "glfwUtils.h"

int Pendulum::RANDOMSEED = 17;

Pendulum::Pendulum(int nBallsP) {
    nBalls = nBallsP;

    // color
    rootColor = 0xffffffff;

    // setup balls
    balls = (ball_t*) malloc(nBalls * sizeof(ball_t));
    double rootX = 0;
    double rootY = 0;
    for (int i=0; i < nBalls; i++) {
        balls[i] = Ball(&rootX, &rootY);
    }

}

Pendulum::render(GLFWwindow* window, double windowX, double windowY, double dT) {

    double* accels;
    solve(&accels);

    drawCircle(windowX, windowY, Pendulum::RENDER_BALL_RADIUS, rootColor); // draw root
    double rootX = 0; double rootY = 0;

    for (int i=0; i < nBalls; i++) {
        balls[i].ngAccel = accels[i];
        balls[i].tick(&rootX, &rootY, dT);
    }

    for (int i=0; i < nBalls; i++) {
        balls[i].render(window, windowX, windowY);
        if (i == 0) { drawLine(windowX, windowY, balls[i].trueX, balls[i].trueY); }
        else { drawLine(balls[i-1].trueX, balls[i-1].trueY, balls[i].trueX, balls[i].trueY); }
    }

    return 0;

}

void Pendulum::solve(double** accels) {
    *accels = (double*) malloc(nBalls * sizeof(double));
    matrix_t a, b;
    matrixA(&a);
    matrixB(&b);

    /*
        Ax = B, solve for x
        A := LU
        LUx = B, solve for x
        Ux := Y
        Ly = B, Ux = y
        Solve Ly=B for y
        Solve Ux = y for x
    
    */
    matrix_t lower, upper;
    a.decomp(&lower, &upper);

    // Solve equations - IT WORKS !! OMGGGG YAYYY YIPPEE

    double y[nBalls];
    for (int n=0; n < nBalls; n++) {
        double sum = 0;
        for (int i=0; i < n; i++) {
            sum += y[i] * lower.get(n,i);
        }
        y[n] = b.get(n) - sum;
    }

    for (int n=nBalls-1; n >= 0; n--) {
        double sum = 0;
        for (int i=nBalls-1; i > n; i--) {
            sum += (*accels)[i] * upper.get(n,i);
        }
        (*accels)[n] = (y[n] - sum) / upper.get(n,n);
    }


}

void Pendulum::matrixA(matrix_t* a) {
    /*
        a in M_{N_BALLS, N_BALLS}(R)
        a[i][j] = l * cos(theta_i - theta_j) * (N_BALLS - max(i,j)) * Ball.MASS
    */
    *a = Matrix(nBalls, nBalls);
    for (int i=0; i < nBalls; i++) {
        for (int j=0; j < nBalls; j++) {
            a->put(i,j, Pendulum::STR_LENGTH * std::cos(balls[i].angle - balls[j].angle) * (nBalls - std::max(i,j)) * Ball::MASS);
        }
    }
}

void Pendulum::matrixB(matrix_t* b) {
    /*
        b in M_{N_BALLS,1}(R)
        for i in range(N_BALLS):
            matB[i] = -Pendulum.GRAVITY * math.sin(self.balls[i].theta) * (N_BALLS - i) * Ball.MASS

            for j in range(N_BALLS):
                matB[i] -= Pendulum.LENGTH * 
                (
                    math.sin(self.balls[i].theta - self.balls[j].theta) + 
                    (self.balls[j].angularVel**2) * 
                    math.sin(self.balls[i].theta - self.balls[j].theta)
                )
                * (N_BALLS - max(i,j))
                * Ball.MASS

    */

    *b = Matrix(nBalls, 1);
    for (int i=0; i < nBalls; i++) {
        b->put(i, -Pendulum::GRAVITY * std::sin(balls[i].angle) * (nBalls - i) * Ball::MASS);
        for (int j=0; j < nBalls; j++) {
            b->put(i, b->get(i) - (Pendulum::STR_LENGTH * (nBalls - std::max(i,j)) * Ball::MASS * ((balls[j].ngVel * balls[j].ngVel) * std::sin(balls[i].angle - balls[j].angle)) + std::sin(balls[i].angle - balls[j].angle)));
        }
    }
}