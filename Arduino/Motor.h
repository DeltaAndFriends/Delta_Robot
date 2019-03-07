#ifndef Motor_h
#define Motor_h
#include <ArduinoSTL.h>

namespace Delta
{

  struct MotorConfig 
  {
    const int DIR;  //direction
    const int EN; //enable
    const int ST; //step
    const int SPR; //steps per revolution
  };
  
  //! A motor types enum.
  enum class MOTOR_t
  {
    stepper, /*!< A stepper motor effectively has multiple "toothed" electromagnets arranged around a central gear-shaped piece of iron. */
    geared, /*!< A geared motor refers to a combination of a motor plus a reduction geartrain. */
    servo, /*!< A servo motor consists of a suitable motor coupled to a sensor for position feedback. . */
  };

  
  //! An abstruct motor class.
  /*!
   * An interface class for implementing different motor types.
   */  
  class Motor
  {
    public:
      virtual ~Motor() {}
      virtual void setup(size_t number) = 0;

      
      /** 
       * A function to rotate.
       * @param angle [grad*10]
       */
      virtual void rotate(int angle) = 0; 
  };
 
  /**
   * A stand alone function for creation motors.
   * @param type of motor
   */
  Motor* create_motor(MOTOR_t type);

  class Stepper : public Motor
  {
    public:
      Stepper();
      ~Stepper() override;
      void setup(size_t number) override;
      void rotate(int angle) override;
      int grad_to_steps (double grad);
    private:
      void step (int dir);
      int grad10_to_steps (int grad10);
      
      size_t m_number{-1};
  };
} // namespace Delta

#endif // Motor_h
