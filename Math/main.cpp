#include <iostream>
#include "algo.h"
using std::cout;
using std::endl;

int main()
try
{
  triplet fin {0, 0, 30},         // values to achieve
  angles {0, 0, 0};               // angles for rotating

  triplet teta;
  teta = algo(fin, angles);
  cout << teta.x << " " << teta.y << " " << teta.z << endl;

  return 0;
}
catch(...)
{
  return -1;
}
