#pragma once

class Matrix {
    public:
        int rows;
        int cols;
        double** values;

        Matrix(int, int);
        Matrix();

        void destroy();

        double get(int);
        double get(int, int);

        void put(int, double);
        void put(int, int, double);

        void decomp(Matrix*, Matrix*);
};

typedef Matrix matrix_t;