#include <Arduino.h>
#include "Motor.h"
#include "Config.h"

namespace Delta
{

  //! A stepper motor class
  /*!
   * A stepper motor divides a full rotation into a number of equal steps.
   */
  class Stepper : public Motor
  {
    public:
      Stepper();
      ~Stepper() override;
      void setup(size_t number) override;
      void rotate(int angle) override;
    private:
      void step (int dir);
      int grad10_to_steps (int grad10);
      size_t m_number{-1};
  };
  
  Stepper::Stepper()
  {
  }
  
  Stepper::~Stepper()
  {
  }
  
  void Stepper::setup(size_t number)
  {
    m_number = number; 
    pinMode(motors_config.at(m_number).EN, OUTPUT);
    digitalWrite(motors_config.at(m_number).EN, LOW);
    pinMode(motors_config.at(m_number).DIR, OUTPUT);
    pinMode(motors_config.at(m_number).ST, OUTPUT);
  }
  
  void Stepper::rotate(int angle)
  {
    int steps = grad10_to_steps(angle);
    int dir = angle ? angle/abs(angle) : 0;
    for(int i = 0; i < steps; ++i)
    {
      step(dir);
    }
  }

  void Stepper::step (int dir)
  {
    switch (dir)
    {
      case 0:
        return;
        break;
      case -1:
        digitalWrite (motors_config.at(m_number).DIR, LOW);
        break;
      case 1:
        digitalWrite (motors_config.at(m_number).DIR, HIGH);
        break;
     }
    digitalWrite (motors_config.at(m_number).ST, HIGH);
    delayMicroseconds(3000);
    digitalWrite (motors_config.at(m_number).ST, LOW);
   }

  int Stepper::grad10_to_steps (int grad10)
  {
    return abs(round(float(grad10)*motors_config.at(m_number).SPR/3600)); 
  }
  
  //! A geared motor class
  /*!
   * A geared motor conveniently packaged together a reduction geartrain. 
   * These are often conveniently packaged together in one unit. 
   * The gear reduction (gear train) reduces the speed of the motor, 
   * with a corresponding increase in torque. Gear ratios range from just a few (e.g. 3) to huge (e.g. 500). 
   * A small ratio can be accomplished with a single gear pair, while a large ratio requires a series of gear reduction steps and thus more gears.   
   * There are a lot of different kinds of gear reduction.
   * In the case of a small transmission ratio N, the unit may be backdrivable, meaning you can turn the output shaft, 
   * perhaps by hand, at angular velocity w and cause the motor to rotate at angular velocity Nw. A larger transmission ratio N may make the unit non-backdrivable. 
   * Each has advantages for different circumstances. Backdrivability depends not just on N, but on many other factors.
   * For large N, often the maximum output torque is limited by the strength of the final gears, rather than by N times the motor's torque. .
   */
  class GearedMotor : public Motor
  {
    public:
      GearedMotor();
      ~GearedMotor() override;
      void setup(size_t number) override;
      void rotate(int angle) override;
  };
  
  GearedMotor::GearedMotor()
  {
  }
  
  GearedMotor::~GearedMotor()
  {
  }
  
  void GearedMotor::setup(size_t number)
  {
  }
  
  void GearedMotor::rotate(int angle)
  {
  }

  //! A servo motor class
  /*!
   * A servo motor allows for precise control of angular or linear position, velocity and acceleration.
   */
  class ServoMotor : public Motor
  {
    public:
      ServoMotor();
      ~ServoMotor() override;
      void setup(size_t number) override;
      void rotate(int angle) override;
  };
  
  ServoMotor::ServoMotor()
  {
  }
  
  ServoMotor::~ServoMotor()
  {
  }
  
  void ServoMotor::setup(size_t number)
  {
  }
  
  void ServoMotor::rotate(int angle)
  {
  }

  ////////////////////////////////////////
  
  Motor* create_motor(MOTOR_t type)
  {
    switch(type)
    {
      case MOTOR_t::stepper:
        return new Stepper{};
        break;
      case MOTOR_t::geared:
        return new GearedMotor{};
        break;
      case MOTOR_t::servo:
        return new ServoMotor{};
        break;
    }
  }

} // namespace Delta
