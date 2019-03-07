#include <Arduino.h>
#include <ArduinoSTL.h>
#include "Robot.h"
#include "Motor.h"
//#include "Motor.cpp"
#include "Stepper.h"
#include <algorithm> 

namespace Delta
{

Robot::Robot(const Geometry& g, MOTOR_t type)
  : c_geometry(g)
  , m_gyros({Gyro{GYRO_PIN::_1}, Gyro{GYRO_PIN::_2}, Gyro{GYRO_PIN::_3}}) //create 3 gyros on pins _1, _2, _3 from Config.h
  , m_motors({create_motor(type), create_motor(type), create_motor(type), create_motor(type), create_motor(type), create_motor(type)}) //create 6 motors
{
}

Robot::~Robot()
{
  for (auto m : m_motors)
  {
    delete m;
  }
}

void Robot::setup()
{
  steppersInit();
  for (auto g : m_gyros) //setup gyros
  {
    g.setup();
  }
  for (size_t i = 0; i < m_motors.size(); ++i) //setup motors
  {
    m_motors.at(i)->setup(i);
  }
  m_magnet.setup(); //setup magnet
}

void Robot::set_scenario(const std::vector<Scenario>& scenario)
{
  m_scenario = &scenario;
}

bool Robot::process(INTERAPTION i)
{
  bool p = false;
  for (auto& scena : *m_scenario)
  {
    if (scena.m_int == i)
    {
      work(scena.m_scena);
      p = true;
    }
  }
  return p;
}

void Robot::gyro()
{
  if(!DEBUG) return;
  
  for (size_t i = 0; i < m_gyros.size(); ++i)
  {
    m_gyros.at(i).read();
    std::cout << m_gyros.at(i).get_raw(GD::Xacc) << (i == (m_gyros.size() - 1) ? "\n" : " | ");
  }
}

void Robot::home()
{
  
    double pitch{0};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    pitch = m_gyros.at(i).get(Angle::pitch);
    while(pitch > 1 || pitch < -1)
    {
      if(DEBUG) std::cout << "Gyro #" << i+1 << " pitch: " << pitch << " so I rotate for " << -6*pitch << " steps" ;
          m_motors.at(i)->rotate(-6*pitch);
          delay(30);
          m_gyros.at(i).read();
          pitch = m_gyros.at(i).get(Angle::pitch);
      if(DEBUG) std::cout << " and get: " << pitch << std::endl;
    }
  }

  double roll{0};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    roll = m_gyros.at(i).get(Angle::roll);
    while(roll  > 1 || roll < -1)
    {
      if(DEBUG) std::cout << "Gyro #" << i+1 << " roll: " << roll << " so I rotate for " << 3*roll << " steps";
          m_motors.at(i+3)->rotate(3*roll);
          delay(30);
          m_gyros.at(i).read();
          roll = m_gyros.at(i).get(Angle::roll);
      if(DEBUG) std::cout << " and get: " << roll << std::endl;
    }
  }
}

void Robot::oldhome() //it is not good, but it works (it doesn't)
{
  int X{0};
  const int threshold = 1; 
  const int step = 5;
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    X = m_gyros.at(i).get(Angle::pitch);
    if(DEBUG) std::cout << "START " << i+1 << std::endl;
    while (X > 10 || X < -10)
    {
      if(m_gyros.at(i).get(Angle::pitch) != gyro_error && m_gyros.at(i).get(Angle::roll) != gyro_error)
      {
       if(DEBUG) std::cout << X << " " << i+1 << std::endl;
       m_motors.at(i)->rotate(step * (X < -threshold ? 1 : -1));
       bool x_no_change = false;
       do
       {
         delay(1);
         m_gyros.at(i).read();
         if(x_no_change && DEBUG) std::cout << "gyro doesn't change after turn" << std::endl;
         x_no_change = true;
       } while (X == m_gyros.at(i).get(Angle::pitch));
       X = m_gyros.at(i).get(Angle::pitch);}
    }
  }
}
void Robot::moveMotors(int x_1, int x_2, int x_3, int y_1, int y_2, int y_3){
  prepareMovement( 0, x_1 );
  prepareMovement( 1,  x_2 );
  prepareMovement( 2, x_3 );
  prepareMovement( 3, y_1 );
  prepareMovement( 4, y_2 );
  prepareMovement( 5, y_3 );
  runAndWait();
}
void Robot::achieve(double x_1, double x_2, double x_3, double y_1, double y_2, double y_3)
{
  double XtoAchieve[3] = {x_1, x_2, x_3};
  double YtoAchieve[3] = {y_1, y_2, y_3};
  double X{0};
  double Y{0};
  const int tolerance = 1000;
  const int gyrosSize = m_gyros.size();
  for(int i = 0; i < gyrosSize; i++){
    Stepper* motor = m_motors.at(i);
    delay(100);
    m_gyros.at(i).read();
    X = m_gyros.at(i).get(Angle::pitch);
    Y = m_gyros.at(i).get(Angle::roll);
    
   //Serial.println(motor->grad_to_steps(X-XtoAchieve[i]));
   //Serial.println(motor->grad_to_steps(YtoAchieve[i])-Y);
   prepareMovement(i, motor->grad_to_steps(X-XtoAchieve[i]));
   prepareMovement(i+3, motor->grad_to_steps(YtoAchieve[i]-Y));
    
  }
  //Serial.println("rw1");
  
  runAndWait();
  /*for (size_t i = 0; i < gyrosSize; ++i) {    
    while(abs(XtoAchieve[i]-X) > tolerance)
    {
      //if(DEBUG) std::cout << "Gyro #" << i+1 << " X position: " << X << " so I rotate for " << 3*(XtoAchieve[i] - X) << " steps" ;
          Stepper* motor = m_motors.at(i);
          prepareMovement(i, motor->grad_to_steps(X - XtoAchieve[i]));
          runAndWait();
          m_gyros.at(i).read();
          X = m_gyros.at(i).get(Angle::pitch);
      //if(DEBUG) std::cout << " and get: " << X << std::endl;
    }
    while(abs(YtoAchieve[i]-Y) > tolerance)
    {
      //if(DEBUG) std::cout << "Gyro #" << i+1 << " X position: " << X << " so I rotate for " << 3*(XtoAchieve[i] - X) << " steps" ;
          Stepper* motor = m_motors.at(i+3);
          prepareMovement(i+3, motor->grad_to_steps(YtoAchieve[i]-Y));
          runAndWait();
          m_gyros.at(i).read();
          X = m_gyros.at(i).get(Angle::roll);
      //if(DEBUG) std::cout << " and get: " << X << std::endl;
    }
  }*/
}
void Robot::achieveUpdated(double x_1, double x_2, double x_3, double y_1, double y_2, double y_3)
{
    double XtoAchieve[3] = {x_1, x_2, x_3};
    double YtoAchieve[3] = {y_1, y_2, y_3};
    double X{0};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    X = m_gyros.at(i).get(Angle::pitch);
    while(X >= XtoAchieve[i] + 20 || X <= XtoAchieve[i] - 20)
    {
      if(DEBUG) std::cout << "Gyro #" << i+1 << " X position: " << X << " so I rotate for " << 3*(XtoAchieve[i] - X) << " steps" ;
          m_motors.at(i)->rotate(3*(XtoAchieve[i] - X));
          delay(100);
          m_gyros.at(i).read();
          X = m_gyros.at(i).get(Angle::pitch);
      if(DEBUG) std::cout << " and get: " << X << std::endl;
    }
  }

  double Y{0};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    Y = m_gyros.at(i).get(Angle::roll);
    while(Y >= YtoAchieve[i]+20|| Y <= YtoAchieve[i]-20)
    {
      if(DEBUG) std::cout << "Gyro #" << i+1 << " Y position: " << Y << " so I rotate for " << 3*(YtoAchieve[i] - Y) << " steps";
          m_motors.at(i+3)->rotate(3*(Y - YtoAchieve[i]));
          delay(100);
          m_gyros.at(i).read();
          Y = m_gyros.at(i).get(Angle::roll);
      if(DEBUG) std::cout << " and get: " << Y << std::endl;
    }
  }
}

void Robot::park()
{
  for (size_t i = 0; i < 3; ++i) {
    m_gyros.at(1).read();
    while (m_gyros.at(i).get(Angle::pitch) > -680)
    {
        if(DEBUG) std::cout << m_gyros.at(i).get(Angle::pitch) << " parking " << i+1 << std::endl;
        m_motors.at(i)->rotate(-5);
    }
      delay(10);
      m_gyros.at(i).read();
    }
}

void Robot::test()
{
  double pitch[3] = {0, 0, 0};
  double roll[3] = {0, 0, 0};
   for (size_t i = 0; i < m_gyros.size(); ++i) {
      m_gyros.at(i).read();
      pitch[i] = m_gyros.at(i).get(Angle::pitch);
      roll[i] = m_gyros.at(i).get(Angle::roll);
    }
    //std::sort(Z_in, Z_in + 5);
  if(DEBUG) std::cout << "Gyro #1 pitch: " << pitch[0] << ", gyro #2 pitch: " << pitch[1] << ", gyro #3 pitch: " << pitch[2] << 
  ", gyro #1 roll: " << roll[0] << ", gyro #2 roll: " << roll[1] << ", gyro #3 roll: " << roll[2] << std::endl;
}
/*{
    int X[3] = {0, 50, -50};
    int Y[3] = {0, 10, -10};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    int X_t[5] = {0, 0, 0, 0, 0};
    for (size_t j = 0; j < 5; ++j)
    {
      m_gyros.at(i).read();
      X_t[j] = m_gyros.at(i).get(Angle::pitch);
      if(DEBUG) std::cout << " gyro #" << i << ": " << X_t[i];
    }
    int X_o[5] = {0, 0, 0, 0, 0};
    std::sort(X_t, X_o);
    X[i] = X_o[2];
  }
//  Y!!!!!
  if(DEBUG) std::cout << X[0] << ", " << X[1] << ", " << X[2] << ", " << Y[0] << ", " << Y[1] << ", " << Y[2] << std::endl;
  /*if(DEBUG) std::cout << "Gyro #1 X position: " << X[0] << ", gyro #2 X position: " << X[1] << ", gyro #3 X position: " << X[2] << 
  ", gyro #1 Y position: " << Y[0] << ", gyro #2 Y position: " << Y[1] << ", gyro #3 Y position: " << Y[2] << std::endl;*/

//  for(int s = 0; s > -400; s -= 100)
//  {
//    for(size_t i = 0; i < COMMAND::MR1; ++i)
//    {
//      m_motors.at(i)->rotate(-100); 
//    }
//  }//home
//
//  for(int s = 0; s > -500; s -= 100)
//  {
//    for(size_t i = COMMAND::MR1; i <= COMMAND::MR3; ++i)
//    {
//      m_motors.at(i)->rotate(-100); 
//    }
//  }//turn one way
//
//  delay(500);
//  
//  for(int s = 0; s > -1000; s -= 100)
//  {
//    for(size_t i = COMMAND::MR1; i <= COMMAND::MR3; ++i)
//    {
//      m_motors.at(i)->rotate(100); 
//    }
//  }//turn the other way
//
//  delay(500);
//
//  for(int s = 0; s > -500; s -= 100)
//  {
//    for(size_t i = COMMAND::MR1; i <= COMMAND::MR3; ++i)
//    {
//      m_motors.at(i)->rotate(-100); 
//    }
//  }//turn one way
//
//  for(int s = 0; s > -900; s -= 100)
//  {
//    for(auto a : {COMMAND::MT1, COMMAND::MR2, COMMAND::MR3})
//    {
//        m_motors.at(a)->rotate(a == COMMAND::MR3 ? -100 : 100); 
//    }
//  }
//
//  delay(1000);
//
//  for(int s = 0; s > -900; s -= 100)
//  {
//    for(auto a : {COMMAND::MT1, COMMAND::MR2, COMMAND::MR3})
//    {
//        m_motors.at(a)->rotate(a == COMMAND::MR3 ? 100 : -100); 
//    }
//  } 
//
//  delay(500);
//  
//  for(int s = 0; s > -400; s -= 100)
//  {
//    for(size_t i = 0; i < COMMAND::MR1; ++i)
//    {
//      m_motors.at(i)->rotate(100); 
//    }
//  }//unhome (park)

void Robot::work(const Scena& scena)
{
  for (auto step : scena)
  {
    if (step.m_cmd < COMMAND::MOTORS)
    {
      m_motors[step.m_cmd]->rotate(step.m_value);
    }
    for (auto g : m_gyros)
    {
      g.read();
      if(DEBUG) std::cout << g.get(Angle::pitch) << ", " << g.get(Angle::roll) << " | ";
    }
    if(DEBUG) std::cout << std::endl;

    if (step.m_cmd == COMMAND::MAG)
    {
      m_magnet.switch_m(step.m_value ? true : false);
    }
    // TODO: Elaborate other commands
  }
}

} // namespace Delta
