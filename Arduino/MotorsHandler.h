#ifndef MotorsHandler_h
#define MotorsHandler_h
#include <ArduinoSTL.h>
#include "Motor.h"
#include "Position.h"
#include "Config.h"

namespace Delta
{
  
  //! 
  /*!
   * 
   */  
  class MotorsHandler
  {
    public:
    MotorsHandler(MOTOR_t type);
    ~MotorsHandler();
    void setup();
    void step(Position p);
    private:
    std::vector<Motor*> m_motors;
  };
} // namespace Delta

#endif // PositionListener_h
