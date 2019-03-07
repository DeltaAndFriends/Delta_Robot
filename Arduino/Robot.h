#ifndef Robot_h
#define Robot_h

#include <ArduinoSTL.h>
#include "Geometry.h"
#include "Gyro.h"
#include "Magnet.h"
#include "Scenario.h"

namespace Delta
{

  class Motor;
  
  //! A delta robot class
  /*!
   * A class for interacting with a robot
   */  
  class Robot
  {
    public:
      /** 
       * A constructor. 
       * @param g describes a robot geometry.
       * @param type of motors.
       */
      Robot(const Geometry& g, MOTOR_t type);
      
      /** 
       * A destructor
       */
      ~Robot();
      
      /**
       * Setup gyro, motors and magnet
       */
      void setup();
      
      /**
       * 
       */
      void set_scenario(const std::vector<Scenario>& scenario);
      
      /**
       * Process interuption
       * @brief Select sequence of actions depending on interuption
       * @param i an interuption number.
       * @return if the interuption number is found in the scenario.
       */
      bool process(INTERAPTION i);

      /**
       * Achieve home position
       */
      
      void home();
      /**
       * Move to the parking position
       */
       
      void oldhome();
      /**
       * Move to the parking position
       */
      void park();
       
      /**
       * 
       */

      void moveMotors(int x_1, int x_2, int x_3, int y_1, int y_2, int y_3);

      void achieve(double x_1, double x_2, double x_3, double y_1, double y_2, double y_3);
      void achieveUpdated(double x_1, double x_2, double x_3, double y_1, double y_2, double y_3);

      void test();

      /**
       * 
       */
      void gyro();
    private:
      const Geometry c_geometry;
      std::vector<Gyro> m_gyros;
      std::vector<Motor*> m_motors;
      Magnet m_magnet;
      const std::vector<Scenario>* m_scenario{nullptr};
      void work(const Scena& scena);
  };

} // namespace Delta

#endif // Robot_h
