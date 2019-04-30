#include "Math.h"

Matrix::Matrix(const std::vector<std::vector<value_t>> &arr, size_t _row, size_t _column)
    : row(_row), cols(_column), m_values(arr), m_index{ _row, _column }
{}

Matrix::Matrix(size_t _row, size_t _column, value_t default_val)
    : m_index{_row, _column}, row(_row), cols(_column)
{
	m_values = std::vector < std::vector<value_t>>(row, std::vector<value_t>(cols, 0));
}

Matrix::Matrix(const Matrix &other)
    : m_index{other.m_index}
    , m_values{other.m_values}
    , row{other.row}
    , cols{other.cols}
{
}

void Matrix::reshape(int rows, int cols) {
	m_values = std::vector < std::vector<value_t>>(row, std::vector<value_t>(cols, 0));
    this->cols = cols;
    this->row = rows;
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
    if (this->cols == other.row) {
        Matrix result{this->row, other.cols, 0};
        for (size_t i = 0; i < result.row; ++i) {
            for (size_t j = 0; j < result.cols; ++j) {
                for (size_t k = 0; k < this->cols; ++k) {
                    result[{i, j}] += (*this)[{i, k}] * other[{k, j}];
                }
            }
        }
        return result;
    }

    throw std::invalid_argument("Matrix::operator*: Matrix1's columns has to equal to Matrix2's rows");
}

value_t& Matrix::operator[](const Index& index) {
    return m_values[index.row()][index.column()];
}

value_t Matrix::operator[](const Index& index) const {
    return m_values[index.row()][index.column()];
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }
    m_index = other.m_index;
    m_values = other.m_values;
    cols = other.cols;
    row = other.row;
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

Matrix::Index::Index(const std::pair<size_t, size_t>& pos)
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


Point3D& Point3D::operator-(const Point3D& p) const {
  Point3D ans(x - p.x, y - p.y, z - p.z);
  return ans;
}

Point3D& Point3D::operator-() const{
  Point3D ans(-x, -y, -z);
  return ans;
}

Point3D& Point3D::operator+(const Point3D& p) const {
  Point3D ans(x + p.x, y + p.y, z + p.z);
  return ans;
}

Point3D& Point3D::operator=(const Point3D& p) {
  x = p.x;
  y = p.y;
  z = p.z;
  return *this;
}

Point3D& Point3D::operator*(const value_t v) const {
  Point3D ans(v*x, v*y, v*z);
  return ans;
}

Point3D& Point3D::operator/(const value_t v) const {
  Point3D ans(x/v, y/v, z/v);
  return ans;
}

bool Point3D::operator==(const Point3D& p) const {
  return bool(x == p.x && y == p.y && z == p.z);
}

bool Sphere::operator==(const Sphere& s) const {
    return bool(m_center == s.m_center && m_radius == s.m_radius);
}


// When using this function, check the return type
std::variant<Circle, double, std::nullopt_t> find_sphere_intersections(const Sphere& a, const Sphere& b) {
  double d = distance(a.m_center, b.m_center);

  if (d > a.m_radius + b.m_radius || d < abs(a.m_radius - b.m_radius))
  {
    return std::nullopt;
  }
  else if (a == b)
  {
    return std::numeric_limits<double>::infinity();
  }

  double r_a = (pow(a.m_radius, 2) - pow(b.m_radius, 2) + pow(d, 2)) / (2 * d);

  Sphere result_sphere(
      a.m_center + (a.m_center - b.m_center)*a.m_radius/d,
      sqrt(pow(a.m_radius, 2) - pow(r_a, 2))
  );

  // l(a, b, c) -> a^2 + b^2 + c^2
  const auto l = [](const Point3D& p) -> value_t {
      return pow(p.get_x(), 2) + pow(p.get_y(), 2) + pow(p.get_z(), 2);
  };

  // http://www.ambrsoft.com/TrigoCalc/Sphere/TwoSpheres/Intersection.htm
  // A transitional part of the algorithm
  // It was created to calculate x2-x1, y2-y1, z2-z1
  Point3D p(b.m_center - a.m_center);
    // TODO: Check if A, B, C < 0
  value_t A = 2*p.get_x();
  value_t B = 2*p.get_y();
  value_t C = 2*p.get_z();
  value_t D = l(a.m_center) - l(b.m_center);
  value_t mod_N = sqrt(l(p));

  value_t cos_x = A / mod_N;
  value_t cos_y = B / mod_N;
  value_t cos_z = C / mod_N;

  Plane result_plane(
      result_sphere.m_center,
      cos_x,
      cos_y,
      cos_z
  );

  return std::pair(result_sphere, result_plane);
}
