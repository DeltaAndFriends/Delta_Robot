#include <iostream>
#include <stdexcept>
#include "Matrix.h"

Matrix::Matrix(const std::vector<Matrix::value_t> &arr, size_t columns)
    : m_index{arr.size() / columns, columns}
    , m_values(arr)
{
    if (m_values.size() != m_index.mult()) {
        throw std::invalid_argument("Matrix::Matrix(): Matrices aren't of the same size!");
    }
}

Matrix::Matrix(size_t row, size_t column, value_t default_val)
    : m_index{row, column}
    , m_values(m_index.mult(), default_val)
{
}

Matrix::Matrix(const Matrix &other)
    : m_index{other.m_index}
    , m_values{other.m_values}
{
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    if (m_index.row() == other.m_index.row() && m_index.column() == other.m_index.column()) {
        for(size_t i{0}; i < m_values.size(); ++i) {
            m_values[i] += other.m_values[i];
        }
        return *this;
    }

    throw std::invalid_argument("Matrix::operator+=: Matrices aren't of the same size!");
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (m_index.row() == other.m_index.row() && m_index.column() == other.m_index.column()) {
        for(size_t i{0}; i < m_values.size(); ++i) {
            m_values[i] -= other.m_values[i];
        }
        return *this;
    }

    throw std::invalid_argument("Matrix::operator-=: Matrices aren't of the same size!");
}

Matrix Matrix::operator+(const Matrix& other) {
    if (m_index.row() == other.m_index.row() && m_index.column() == other.m_index.column()) {
        Matrix result{*this};
        for(size_t i{0}; i < m_values.size(); ++i) {
            result.m_values[i] += other.m_values[i];
        }
        return result;
    }

    throw std::invalid_argument("Matrix::operator+: Matrices aren't of the same size!");
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (m_index.column() == other.m_index.row()) {
        Matrix result{m_index.row(), other.m_index.column(), 0};
        for (size_t i = 1; i <= result.m_index.row(); ++i) {
            for (size_t j = 1; j <= result.m_index.column(); ++j) {
                for (size_t k = 1; k <= m_index.column(); ++k) {
                    result[Index{i, j}] += (*this)[Index{i, k}] * other[Index{k, j}];
                }
            }
        }
        return result;
    }

    throw std::invalid_argument("Matrix::operator*: Matrix1's columns has to equal to Matrix2's rows");
}

Matrix::value_t& Matrix::operator[](const Index& index) {
    return m_values[m_index.calc_raw_index(index)];
}

Matrix::value_t Matrix::operator[](const Index& index) const {
    return m_values[m_index.calc_raw_index(index)];
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }
    m_index = other.m_index;
    m_values = other.m_values;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
   for (size_t i = 0; i < m.m_index.mult(); i++) {
       if ((i % m.m_index.column() == 0) && i != 0) os << std::endl;
       os << m.m_values[i] << ' ';
   }
   return os;
}

Matrix::Index::Index(size_t row, size_t column)
    : m_row{row}
    , m_column{column}
{
    if (!m_row || !m_column) {
        throw std::invalid_argument("Matrix::Index::Index: index cannot be 0");
    }
}

size_t Matrix::Index::calc_raw_index(const Matrix::Index &in) const
{
    if (m_row && m_column
            && m_row >= in.m_row && m_column >= in.m_column)
    {
        return (in.m_row - 1) * m_column + in.m_column - 1;
    }
    throw std::invalid_argument("Matrix::Index::calc_raw_index: invalid index");
}
