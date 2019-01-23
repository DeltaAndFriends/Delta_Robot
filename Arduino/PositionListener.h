#ifndef PositionListener_h
#define PositionListener_h
#include <ArduinoSTL.h>
#include "Gyro.h"
#include "Position.h"

namespace Delta
{

  //! 
  /*!
   * A class for determining the robot's position using the MPU's and a binary gravity sensor.
   */  
  class PositionListener
  {
    public:
    PositionListener();
    ~PositionListener();
    void setup();
    Position get();
    Position PositionListener::getFiltered();
    private:
    std::vector<Gyro> m_gyros;
  };
} // namespace Delta

#endif // PositionListener_h
