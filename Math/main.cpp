#include <iostream>
#include <vector>
#include "Matrix.h"
#include "algo.h"

using std::cout;
using std::endl;
using std::vector;

int main()
{
  triplet fin {0, 0, 30},         // values to achieve
  angles {0, 0, 0};               // angles for rotating

  triplet teta;
  teta = algo(fin, angles);
  cout << teta.x << " " << teta.y << " " << teta.z << endl;


  system("pause");
  system("pause");
  std::cin.get();
  std::cin.get();
  return 0;
}
