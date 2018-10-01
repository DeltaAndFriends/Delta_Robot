#ifndef Scenario_h
#define Scenario_h

#include <ArduinoSTL.h>

namespace Delta
{
  
  enum INTERAPTION {
    // Critical interaptions
    // Tilt motors errors
    MT1_ERR,
    MT2_ERR,
    MT3_ERR,
    // Rotate motors errors
    MR1_ERR,
    MR2_ERR,
    MR3_ERR,
    CRITICAL_ERR, // Marker for critical interaptions
    
    // Bar loaded
    BL1 = CRITICAL_ERR,
    BL2,
    BL3,

    // Demo program
    DEMO,
  };
  
  enum COMMAND {
    // Tilt motors
    MT1,
    MT2,
    MT3,
    // Rotate motors
    MR1,
    MR2,
    MR3,
    MOTORS,
    // Others
    MAG = MOTORS, // Magnetic
    STOP // Stop all executions
  };
  
  struct Step {
    COMMAND m_cmd;
    int m_value;
  };

  using Scena = std::vector<Step>;
  
  struct Scenario {
    INTERAPTION m_int;
    const Scena m_scena;
  };

} // namespace Delta

#endif // Scenario_h
