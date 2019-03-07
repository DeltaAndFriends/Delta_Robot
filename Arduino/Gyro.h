#ifndef Gyro_h
#define Gyro_h

#include "Config.h"
#include <ArduinoSTL.h>

 enum class GYRO_DATA
  {
    Xacc,
    Yacc,
    Zacc,
	  Temp,
	  Xgyro,
    Ygyro,
    Zgyro
  };
  static const int gyro_error{-32767};

  using GD = GYRO_DATA;
  
  class Gyro
  {
    public:
      Gyro(GYRO_PIN pin);
      ~Gyro();
      void setup();
      void read();
      int get_raw(GD d) const;
      double getacc(GD d) const;
      double get(Angle a);
    private:
      const int p_tilt_switch = 23; 
      int tilt_switch; 
      std::vector<int> m_data;
      size_t m_pin;
  };

#endif // Gyro_h
