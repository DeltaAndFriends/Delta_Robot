#pragma once
// #include <stdexcept>
#include "Matrix.h"

Matrix& Matrix::operator+=(const Matrix& b) {
//    if (len != b.len || width != b.width) throw std::invalid_argument("Matrixes aren't of the same size!");
    for(int i = 0; i < len; i++) values[i] += b.values[i];
}

Matrix& Matrix::operator-=(const Matrix& b) {
//    if (len != b.len || width != b.width) throw std::invalid_argument("Matrixes aren't of the same size!");
    for(int i = 0; i < len; i++) values[i] -= b.values[i];
}

Matrix Matrix::operator+(const Matrix& b) {
//    if (len != b.len || width != b.width) throw std::invalid_argument("Matrixes aren't of the same size!");
    Matrix c(b);

    for(int i = 0; i < len; i++)
        c.values[i] += values[i];

    return c;
}

Matrix Matrix::operator*(const Matrix& b) const {
    // if (width == 1 && b.width > width) return b*(*this);


    Matrix c(b.len, width);


    for (short i = 0; i < len; i++) {
        for (short j = 0; j < b.width; j++) {
            for (short k = 0; k < width; k++) {
                c[i * width + j] += values[i * width + k] * b[k * b.width + j];
            }
        }
    }


    return c;
}


// for (short i = 0; i < b.len; i++)
//         for(short j = 0; j < width; j++)
//             c.values[j + i*width] += values[j + i*width] * b.values[i + j*width];



    // if (b.width == 1) {
    //     Matrix c(b);
    //     for (i = 0; i < c.len*width; i++)
    //         // for (j = 0; j < width; j++)
    //             c.values[i] += b.values[i] * values[i];

    //     return c;
    // } else

    // {
// Matrix Matrix::operator*()


double& Matrix::operator[](int i) {
    return values[i];
}

double Matrix::operator[](int i) const {
    return values[i];
}

Matrix& Matrix::operator=(const Matrix& b) {
    if (values == b.values) return *this;
    values = b.values;
    width = b.width;
    len = b.len;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
   for (int i = 0; i < m.len; i++) {
       if ((i % m.width == 0) && i != 0) os << std::endl;
       os << m.values[i] << ' ';
   }
   return os;
}
