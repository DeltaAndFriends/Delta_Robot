#pragma once
#include <vector>

class Matrix {
public:
    using value_t = double;
    class Index
    {
        size_t m_row{0};
        size_t m_column{0};
    public:
        Index(size_t row, size_t column);

        size_t row() const {return m_row;}
        size_t column() const {return m_column;}
        size_t mult() const {return m_row * m_column; }
        size_t calc_raw_index(const Index& in) const;
    };

    Matrix(const std::vector<value_t>& arr, size_t columns);

    Matrix(size_t row, size_t column, value_t default_val = 0.0);

    Matrix(const Matrix& other);

    ~Matrix() = default;

    Matrix& operator+=(const Matrix& other);

    Matrix& operator-=(const Matrix& other);

    Matrix operator+(const Matrix& other);

    Matrix operator*(const Matrix& other) const;

    Matrix& operator=(const Matrix& other);

    value_t& operator[](const Index& index);

    value_t operator[](const Index& index) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
private:
    Index m_index;
    std::vector<value_t> m_values;

};
