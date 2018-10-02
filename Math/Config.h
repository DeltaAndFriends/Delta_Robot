#ifndef Config_h
#define Config_h

// #include <ArduinoSTL.h>
// #include "Motor.h"

const bool DEBUG = true;

///// Robot Geomentry
// work base radiuses
const size_t robot_main_basis = 10; // mm
const size_t robot_tool_basis = 10; // mm
// work lever distances
const size_t robot_lever1 = 10; // mm
const size_t robot_lever2_w = 10; // mm
const size_t robot_lever2_h = 10; // mm
// limitations
const size_t lim_tilt_UP = 10; // grad
const size_t lim_tilt_DOWN = 10; // grad
const size_t lim_rotate_CW = 10; // grad
const size_t lim_rotate_CCW = 10; // grad

///// Constants for matrix calculations
struct triplet {
  double x;
  double y;
  double z;
};

// coordinates of points (1), (2), (3), (4), (5), (6)
const triplet coord[6] {
    {2.6, 2.1, 0},
    {0.5, 3.3, 0},
    {-3.1, 1.2, 0},
    {-3.1, -1.2, 0},
    {0.5, -3.3, 0},
    {2.6, -2.1, 0}
};
// linear sizes of the device held by robot
const triplet sizes {6.3, 31, 15};



/// Gyroscope configurations
//! A gyro turn on pins enum.
enum class GYRO_PIN {
  _1 = 37,
  _2 = 39,
  _3 = 35,
};


/// Magnet configurations
//! A magnet turn on pins enum.
enum MAG_PIN {
  EN = 33
};

// /// Motor configurations
// const Delta::MOTOR_t MOTOR_type = Delta::MOTOR_t::stepper;

// const std::vector<Delta::MotorConfig> motors_config =
// {
//   //direction, enable, step, steps per revolution
//   { 48, 62, 46, 3200 },
//   { 61, 56, 60, 3200 },
//   { 55, 38, 54, 3200 },
//   { 43, 41, 45, 1600 },
//   { 34, 30, 36, 1600 },
//   { 28, 24, 26, 1600 },
// };

#endif // Config_h
