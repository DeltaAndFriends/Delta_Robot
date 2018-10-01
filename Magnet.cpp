#include "Magnet.h"
#include "Config.h"
#include <Arduino.h>

namespace Delta
{

  Magnet::Magnet()
  {    
  }
  
  void Magnet::setup()
  {
    pinMode(MAG_PIN::EN, OUTPUT);
    digitalWrite (MAG_PIN::EN, LOW);
  }
  
  void Magnet::switch_m(bool b)
  {
    m_on = b;
    digitalWrite (MAG_PIN::EN, m_on ? HIGH : LOW);
  }
  
} // namespace Delta
