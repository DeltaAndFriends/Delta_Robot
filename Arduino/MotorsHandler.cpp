#include <Arduino.h>
#include "MotorsHandler.h"

namespace Delta
{
  
  MotorsHandler::MotorsHandler(MOTOR_t type)
  : m_motors({create_motor(type), create_motor(type), create_motor(type), create_motor(type), create_motor(type), create_motor(type)})
  {
  }
  
  MotorsHandler::~MotorsHandler()
  {
  }

  void MotorsHandler::setup();

  void MotorsHandler::step(Position p)
  {
  }
} // namespace Delta
