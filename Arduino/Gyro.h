#ifndef Gyro_h
#define Gyro_h

#include <ArduinoSTL.h>
#include "Config.h"

namespace Delta
{

  enum class GYRO_DATA
  {
    X,
    Y,
    qty
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
      int get(GD d) const;
    private:
      std::vector<int> m_data;
      size_t m_pin;
  };

} // namespace Delta

#endif // Gyro_h
