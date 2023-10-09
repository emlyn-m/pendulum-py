#include <stdlib.h>
#include <cstdio>

#include "Matrix.h"

Matrix::Matrix(int r, int c) {
    rows = r;
    cols = c;

    values = (double**) malloc(rows * sizeof(double));
    for (int i=0; i < rows; i++) {
        values[i] = (double*) malloc(cols * sizeof(double));
        for (int j=0; j < cols; j++) {
            i[values][j] = 0;
        }
    }
}
Matrix::Matrix() {}


void Matrix::destroy() {
    for (int i=0;  i < rows; i++) {
        free(values[i]);
    }
    free(values);
}

double Matrix::get(int i, int j) { return values[i][j]; }
double Matrix::get(int i) { return values[i][0]; }
void Matrix::put(int i, double v) { values[i][0] = v; }
void Matrix::put(int i, int j, double v) { values[i][j] = v; }


void Matrix::decomp(matrix_t* lower, matrix_t* upper) {
    *lower = Matrix(rows, rows);
    *upper = Matrix(rows, rows);

    for (int i=0; i < rows; i++) {
        for (int k=i; k < rows; k++) {
            double sum = 0;
            for (int j=0; j < i; j++) {
                sum += lower->get(i,j) * upper->get(j,k);
            }
            upper->put(i, k, values[i][k] - sum);
        }

        for (int k=i; k < rows; k++) {
            if (i == k) {
                lower->put(i,i, 1.0);
            } else {
                double sum = 0;
                for (int j=0; j < i; j++) {
                    sum += lower->get(k,j) * upper->get(j,i);
                }
                lower->put(k,i, (values[k][i] - sum) / upper->get(i,i));
            }
        }
    }
}