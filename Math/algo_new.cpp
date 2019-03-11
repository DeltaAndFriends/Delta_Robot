#include <iostream>
#include <math.h>
#include <string>
#include "Math.h"
#include "Math.cpp"
using std::vector;

double mid(double x1, double x2) {
    return (x1 + x2) / (double) 2;
}

void chapter(std::string label) {
    std::cout << std::endl << label << std::endl;
    std::cout << "------------------------------------" << std::endl << std::endl;
}


Point3D algo(const Point3D& wanted, const Point3D& angles) {

	// Step 0. Preparing data.
	// Contemporary data
	double middles[3][3]{ // for iteration
		{0, 0, 0}, // middle_16
		{0, 0, 0}, // middle_23
		{0, 0, 0}  // middle_45
	};

	//
	// Step 1. Calculating new coordinates of (1) - (6).
	// Temporary data
  vector<vector<double>> a_arr = {
  {	1, 0, 0, 0	},
  {	0, 1, 0, 0	},
  {	0, 0, 1, 0	},
  {	0, 0, 0, 1	}
  };

  vector<vector<double>> b_x_arr = {
  {	1, 0, 0, 0							},
  {	0, cos(angles.x), -sin(angles.x), 0	},
  {	0, sin(angles.x), cos(angles.x), 0	},
  {	0, 0, 0, 1							}
  };

  vector<vector<double>> b_y_arr = {
  {	cos(angles.y), 0, -sin(angles.y), 0	},
  {	0, 1, 0, 0							},
  {	sin(angles.y), 0, cos(angles.y), 0	},
  {	0, 0, 0, 1							}
  };

  vector<vector<double>> b_z_arr = {
  {	cos(angles.z), -sin(angles.z), 0, 0	},
  {	sin(angles.z), cos(angles.z), 0, 0	},
  {	0, 0, 1, 0							},
  {	0, 0, 0, 1							}
  };

  vector<vector<double>> c_arr = {
  {	1, 0, 0, wanted.x	},
  {	0, 1, 0, wanted.y	},
  {	0, 0, 1, wanted.z	},
  {	0, 0, 0, 1			}
  };

  Matrix a(a_arr, 4, 4);
  Matrix b_x(b_x_arr, 4, 4);
  Matrix b_y(b_y_arr, 4, 4);
  Matrix b_z(b_z_arr, 4, 4);
  Matrix c(c_arr, 4, 4);

  if (DEBUG) {
    chapter("Given data: Matrix a ... c");
    std::cout << a << std::endl << std::endl
      << b_x << std::endl << std::endl
      << b_y << std::endl << std::endl
      << b_z << std::endl << std::endl
      << c << std::endl << std::endl;
  }

  Matrix precalc(4, 4);
  precalc = a * b_x * b_y * b_z * c;

  if (DEBUG) {
    chapter("PRECALC = A * B_X * ... * C");
    std::cout << precalc << std::endl;
  }

  Matrix final[6] {
    Matrix(4, 1),
    Matrix(4, 1),
    Matrix(4, 1),
    Matrix(4, 1),
    Matrix(4, 1),
    Matrix(4, 1)
  };

  for (int i = 0; i < 6; i++) {
    final[i].reshape(4, 1);
    final[i] = precalc * Matrix({ {triangle_dim[i].x}, {triangle_dim[i].y}, {triangle_dim[i].z}, {1} }, 4, 1);
    std::cout << final[i] << std::endl;
  }


}


int main() {
  Point3D fin(0, 0, 30),         // values to achieve
  angles (0, 0, 0);               // angles for rotating

  triplet teta;
  teta = algo(fin, angles);
  // std::cout << teta.x << " " << teta.y << " " << teta.z << std::endl;
}
