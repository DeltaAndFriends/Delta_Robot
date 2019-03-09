#pragma once
#include <vector>
#include "Config.h"

// includes geometric and algebraic shapes for calculations
//  * Matrix
//    * Index
//  * Point
//  * Sphere

class Matrix {
public:
    using value_t = double;
    class Index {
    private:
      size_t m_row{0};
      size_t m_column{0};
    public:
      Index(size_t row = 0, size_t column = 0);
      Index(const std::pair<size_t, size_t>& pos);

      size_t row() const {return m_row;}
      size_t column() const {return m_column;}
      size_t mult() const {return m_row * m_column;}
    };

	Matrix(const std::vector<std::vector<Matrix::value_t>> &arr, size_t row, size_t columns);
  Matrix(size_t row = 0, size_t column = 0, value_t default_val = 0.0);
  Matrix(const Matrix& other);
  ~Matrix() = default;

  Matrix& operator+=(const Matrix& other);
  Matrix& operator-=(const Matrix& other);
  Matrix operator+(const Matrix& other);
  Matrix operator*(const Matrix& other) const;
  Matrix& operator=(const Matrix& other);

  void reshape(int rows, int cols);

  value_t& operator[](const Index& index);
  value_t operator[](const Index& index) const;

  friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
private:
  Index m_index;
	size_t cols;
	size_t row;
  std::vector<std::vector<value_t>> m_values;
};


class Point3D {
public:
  using value_t = double;
  Point3D(value_t x = 0.0, value_t y = 0.0, value_t z = 0.0)
  : x(x), y(y), z(z) {}
  Point3D(const Point3D& p) : x(p.x), y(p.y), z(p.z) {}

  Point3D& operator-(const Point3D&);
  Point3D& operator-();
  Point3D& operator+(const Point3D&);
  Point3D& operator=(const Point3D&);
  bool operator==(const Point3D&) const;
  friend value_t distance(Point3D a, Point3D b) { return abs(b - a); }
private:
  value_t x;
  value_t y;
  value_t z;
};

class Sphere {
public:
  using value_t = double;
  friend std::pair<Sphere, Plane> find_sphere_intersections(const Sphere&, const Sphere&);
  friend std::pair<Sphere, Plane> find_sphere_intersections(const Sphere&, const Sphere&, const Plane&);
  friend std::pair<Sphere, Plane> find_sphere_intersections(const Sphere&, const Plane&);
  Sphere(Point3D center, value_t radius) : m_center(center), m_radius(radius) {}
private:
  Point3D m_center;
  value_t m_radius;
};

class Plane {
public:
  using value_t = double;
  Plane(Point3D p, cos_x, cos_y, cos_z) :
    m_base_point(p),
    m_cos_x(cos_x),
    m_cos_y(cos_y),
    m_cos_z(cos_z)
  {}
private:
  Point3D m_base_point;
  float m_cos_x, m_cos_y, m_cos_z;
};
