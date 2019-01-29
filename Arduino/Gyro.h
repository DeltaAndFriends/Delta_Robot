#ifndef Gyro_h
#define Gyro_h

#include <ArduinoSTL.h>

/// Gyroscope configurations
//! A gyro turn on pins enum.
enum class GYRO_PIN {
  _1 = 37,
  _2 = 39,
  _3 = 35,
};

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
      double get(GD d) const;
    private:
      std::vector<int> m_data;
      size_t m_pin;
  };

#endif // Gyro_h
