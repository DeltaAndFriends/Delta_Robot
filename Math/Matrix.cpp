#include <iostream>
#include <vector>
#include <stdexcept>
#include "Matrix.h"


Matrix::Matrix(const std::vector<std::vector<Matrix::value_t>> &arr, size_t _row, size_t columns)
    : row(_row), cols(columns), m_values(arr)
{}

Matrix::Matrix(size_t row, size_t column, value_t default_val)
    : m_index{row, column}
{
	m_values = std::vector < std::vector<value_t>>(row, std::vector<value_t>(cols, 0));
}

Matrix::Matrix(const Matrix &other)
    : m_index{other.m_index}
    , m_values{other.m_values}
{
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    if (m_index.row() == other.m_index.row() && m_index.column() == other.m_index.column()) 
	{
		for(size_t i{0}; i < m_values.size(); ++i) 
		{
			for (size_t j{ 0 }; j < m_values[0].size(); ++j)
			{
				m_values[i][j] += other.m_values[i][j];
			}
        }
        return *this;
    }

    throw std::invalid_argument("Matrix::operator+=: Matrices aren't of the same size!");
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (m_index.row() == other.m_index.row() && m_index.column() == other.m_index.column()) {
        for(size_t i{0}; i < m_values.size(); ++i) 
		{
			for (size_t j{ 0 }; j < m_values[0].size(); ++j)
			{
				m_values[i][j] -= other.m_values[i][j];
			}
			}
        return *this;
    }

    throw std::invalid_argument("Matrix::operator-=: Matrices aren't of the same size!");
}

Matrix Matrix::operator+(const Matrix& other) {
    if (m_index.row() == other.m_index.row() && m_index.column() == other.m_index.column()) {
        Matrix result{*this};
        for(size_t i{0}; i < m_values.size(); ++i) {
			for (size_t j{ 0 }; j < m_values[0].size(); ++j)
			{
				result.m_values[i][j] += other.m_values[i][j];
			}
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
                    result[{i, j}] += (*this)[{i, k}] * other[{k, j}];
                }
            }
        }
        return result;
    }

    throw std::invalid_argument("Matrix::operator*: Matrix1's columns has to equal to Matrix2's rows");
}

Matrix::value_t& Matrix::operator[](const Index& index) {
    return m_values[index.row][index.column]; //TODO something with this shit
}

Matrix::value_t Matrix::operator[](const Index& index) const {
    return m_values[index.row][index.column];
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
   for (size_t i = 0; i < m.m_values.size(); ++i) {
	   for (size_t j = 0; j < m.m_values[0].size(); ++j)
	   {
		   os << m.m_values[i][j] << ' ';
	   }
	   os << std::endl;
   }
   return os;
}

Matrix::Index::Index(const std::pair<size_t, size_t> pos)
	: m_row{ pos.first }
	, m_column{ pos.second }
{
	if (m_row < 0 || m_column < 0) {
		throw std::invalid_argument("Matrix::Index::Index: index cannot be 0");
	}
}

Matrix::Index::Index(size_t row, size_t column)
    : m_row{row}
    , m_column{column}
{
    if (m_row < 0 || m_column < 0) {
        throw std::invalid_argument("Matrix::Index::Index: index cannot be 0");
    }
}
