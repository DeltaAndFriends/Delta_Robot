#pragma once
#include <vector>
//#include <iostream>
using std::vector;

class Matrix {
    vector<double> values;
    int width;
    size_t len;

    public:
        Matrix(const std::vector<double>& arr, int w) :
            values(arr),
            width(w),
            len(values.size()) {};

        Matrix(int length, int w, double val = 0) : width(w), len(length) {
            values = vector<double>(length, val);
        }

        Matrix(const Matrix& a) :
            values(a.values),
            width(a.width),
            len(a.len) {};

        ~Matrix() {};

        Matrix& operator+=(const Matrix& b);

        Matrix& operator-=(const Matrix& b);

        Matrix operator+(const Matrix& b);

        Matrix operator*(const Matrix& b) const;

        Matrix& operator=(const Matrix& b);

        double& operator[](int i);

        double operator[](int i) const;

       friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};
