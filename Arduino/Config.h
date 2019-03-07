#ifndef Config_h
#define Config_h

#include <ArduinoSTL.h>
#include "Motor.h"

enum class Angle 
{
  pitch,
  roll,
  yaw
};

const bool DEBUG = false;

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
  EN = 32
};

/// Motor configurations
const Delta::MOTOR_t MOTOR_type = Delta::MOTOR_t::stepper;

const std::vector<Delta::MotorConfig> motors_config = 
{
  //direction, enable, step, steps per revolution
  { 48, 62, 46, 3200 },
  { 61, 56, 60, 3200 },
  { 55, 38, 54, 3200 },
  { 43, 41, 45, 3200 }, 
  { 34, 30, 36, 3200 }, 
  { 28, 24, 26, 3200 }, 
};

//46 PL3
#define M1STEP_H             PORTL |=  0b00001000;
#define M1STEP_L             PORTL &= ~0b00001000;
//60 
#define M2STEP_H             PORTF |=  0b01000000;
#define M2STEP_L             PORTF &= ~0b01000000;
//54 amalog0 PF0
#define M3STEP_H             PORTF |=  0b00000001;
#define M3STEP_L             PORTF &= ~0b00000001;
//45 PL4
#define M4STEP_H             PORTL |=  0b00010000;
#define M4STEP_L             PORTL &= ~0b00010000;
//36 PC1
#define M5STEP_H             PORTC |=  0b00000010;
#define M5STEP_L             PORTC &= ~0b00000010;
//26 PA4
#define M6STEP_H             PORTA |=  0b00010000;
#define M6STEP_L             PORTA &= ~0b00010000;

#endif // Config_h
