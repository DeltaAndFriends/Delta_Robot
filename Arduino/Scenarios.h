#ifndef Scenarios_h
#define Scenarios_h

#include "Scenario.h"

namespace Delta
{
  
  const Scena ScenaError
  {
    {STOP, 0}
  };
  
  const Scena ScenaBL1
  {
    {MT1, -420},
    {MT2, -420},
    {MT3, -420},
//    {MR1, -100},
//    {MR2, -100},
//    {MR3, -100},
//    {MR1, 100},
//    {MR2, 100},
//    {MR3, 100},
    {MT1, 300},
    {MR2, 250},
    {MR3, -250},
    {MT1, 300},
    {MR3, -250},
    {MR2, 250},
    {MT1, 300},
    {MT2, -100},
    {MR3, -100},
    {MT3, -100},
    {MR2, 100},
    {MT2, -100},
    {MR2, 100},
    {MT3, -100},
    {MR3, -100},
    {MT2, -100},
    {MR3, -100},
    {MT3, -100},
    {MR2, 100},
    
    //{MT1, -100},
    //{MR1, -150},
    //{MT2, -100},
    //{MR2, -150},
    //{MT3, -100},
    //{MR3, -150},

   /* {MT1, 300},
    {MT2, 150},
    {MT3, 250},
    {MAG, 1},
    {MT2, -150},
    {MAG, 0},*/
  };
  
  const Scena ScenaBL2
  {
    {MT1, 10},
    {MR1, 15}
  };
  
  const Scena ScenaBL3
  {
    {MT1, 10},
    {MR1, 15}
  };

  const Scena Scena_gen(int mt1, int mt2, int mt3, int mr1, int mr2, int mr3)
  {
    Scena steps;
    
    // MOTORS
    std::vector<int> values{mt1, mt2, mt3, mr1, mr2, mr3};
    std::vector<int> signs;

    for(auto &v : values)
    {
      if(v < 0)
      {
        signs.push_back(-1);
        v *= -1;
      }
      else
      {        
        signs.push_back(1);
      }
    }

    while(true)
    {
      bool progress = false;
      for(size_t c = MT1; c < COMMAND::MOTORS; ++c)
      {
        if(values[c])
        {
          steps.push_back({COMMAND(c), 1 * signs[c]});
          --values[c];
          progress = true;
        }
      }
      if(!progress)
      {
        break;
      }
    }

    // Magnet
    steps.push_back({MAG, 1});

    return steps;
  }
  
  const std::vector<Scenario> Scenario1
  {
    {BL1, ScenaBL1},
    {BL2, ScenaBL2},
    {BL3, ScenaBL3},

    {MT1_ERR, ScenaError},
    {MT2_ERR, ScenaError},
    {MT3_ERR, ScenaError},
    {MR1_ERR, ScenaError},
    {MR2_ERR, ScenaError},
    {MR3_ERR, ScenaError},
  };
  
  const std::vector<Scenario> Scenario2
  {    
    {MT1_ERR, ScenaError},
    {MT2_ERR, ScenaError},
    {MT3_ERR, ScenaError},
    {MR1_ERR, ScenaError},
    {MR2_ERR, ScenaError},
    {MR3_ERR, ScenaError},
  };

} // namespace Delta

#endif // Scenarios_h
