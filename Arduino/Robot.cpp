#include <Arduino.h>
#include <ArduinoSTL.h>
#include "Robot.h"
#include "Motor.h"
#include <algorithm> 

namespace Delta
{

Robot::Robot(const Geometry& g, MOTOR_t type)
  : c_geometry(g)
  , m_gyros({Gyro{GYRO_PIN::_1}, Gyro{GYRO_PIN::_2}, Gyro{GYRO_PIN::_3}}) //red
  , m_motors({create_motor(type), create_motor(type), create_motor(type), create_motor(type), create_motor(type), create_motor(type)}) //red
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
  //if(DEBUG) std::cout << "START " << std::endl;
  for (auto g : m_gyros)
  {
    g.setup();
  }
  for (size_t i = 0; i < m_motors.size(); ++i)
  {
    m_motors.at(i)->setup(i);
  }
  m_magnet.setup();
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
    int X{0};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    X = m_gyros.at(i).get(GD::Xacc);
    while(X != 0)
    {
      if(DEBUG) std::cout << "Gyro #" << i+1 << " X position: " << X << " so I rotate for " << -6*X << " steps" ;
          m_motors.at(i)->rotate(-6*X);
          delay(30);
          m_gyros.at(i).read();
          X = m_gyros.at(i).get(GD::Xacc);
      if(DEBUG) std::cout << " and get: " << X << std::endl;
    }
  }

  int Yacc{0};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    Yacc = m_gyros.at(i).get(GD::Yacc);
    while(Yacc != 0)
    {
      if(DEBUG) std::cout << "Gyro #" << i+1 << " y position: " << Yacc << " so I rotate for " << -3*Yacc << " steps";
          m_motors.at(i+3)->rotate(-3*Yacc);
          delay(30);
          m_gyros.at(i).read();
          Yacc = m_gyros.at(i).get(GD::Yacc);
      if(DEBUG) std::cout << " and get: " << Yacc << std::endl;
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
    X = m_gyros.at(i).get(GD::Xacc);
    if(DEBUG) std::cout << "START " << i+1 << std::endl;
    while (X > 10 || X < -10)
    {
      if(m_gyros.at(i).get(GD::Xacc) != gyro_error && m_gyros.at(i).get(GD::Yacc) != gyro_error)
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
       } while (X == m_gyros.at(i).get(GD::Xacc));
       X = m_gyros.at(i).get(GD::Xacc);}
    }
  }
}

void Robot::achieve(int x_1, int x_2, int x_3, int y_1, int y_2, int y_3)
{
    int XtoAchieve[3] = {x_1, x_2, x_3};
    int YtoAchieve[3] = {y_1, y_2, y_3};
    int X{0};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    X = m_gyros.at(i).get(GD::Xacc);
    while(X != XtoAchieve[i])
    {
      if(DEBUG) std::cout << "Gyro #" << i+1 << " X position: " << X << " so I rotate for " << 6*(XtoAchieve[i] - X) << " steps" ;
          m_motors.at(i)->rotate(6*(XtoAchieve[i] - X));
          delay(100);
          m_gyros.at(i).read();
          X = m_gyros.at(i).get(GD::Xacc);
      if(DEBUG) std::cout << " and get: " << X << std::endl;
    }
  }

  int Yacc{0};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    m_gyros.at(i).read();
    Yacc = m_gyros.at(i).get(GD::Yacc);
    while(Yacc != YtoAchieve[i])
    {
      if(DEBUG) std::cout << "GYaccro #" << i+1 << " Yacc position: " << Yacc << " so I rotate for " << 3*(YtoAchieve[i] - Yacc) << " steps";
          m_motors.at(i+3)->rotate(3*(YtoAchieve[i] - Yacc));
          delay(100);
          m_gyros.at(i).read();
          Yacc = m_gyros.at(i).get(GD::Yacc);
      if(DEBUG) std::cout << " and get: " << Yacc << std::endl;
    }
  }
}

void Robot::park()
{
  for (size_t i = 0; i < 3; ++i) {
    m_gyros.at(1).read();
    while (m_gyros.at(i).get(GD::Xacc) > -680)
    {
        if(DEBUG) std::cout << m_gyros.at(i).get(GD::Xacc) << " parking " << i+1 << std::endl;
        m_motors.at(i)->rotate(-5);
    }
      delay(10);
      m_gyros.at(i).read();
    }
}

void Robot::test()
{
  int X[3] = {0, 0, 0};
  int Y[3] = {0, 0, 0};

  Position p = pos_listen.getFiltered();
  X[0] = p.roll_1;
  X[1] = p.roll_2;
  X[2] = p.roll_3;
  Y[0] = p.pitch_1;
  Y[1] = p.pitch_2;
  Y[2] = p.pitch_3;
  //  for (size_t i = 0; i < m_gyros.size(); ++i) {
  //     m_gyros.at(i).read();
  //     X[i] = m_gyros.at(i).get(GD::Xacc);
  //     Yacc[i] = m_gyros.at(i).get(GD::Yacc);
  //   }

    if(X[0] > test_max)
    {
      test_max = X[0];
    }

    if(X[0] < test_min)
    {
      test_min = X[0];
    }

    std::cout << "Min: " << test_min << " Max: " << test_max << " Current: " << X[0] << std::endl;
    //std::sort(Z_in, Z_in + 5);
  //if(DEBUG) std::cout << "GYaccro #1 X position: " << X[0] << ", gYaccro #2 X position: " << X[1] << ", gYaccro #3 X position: " << X[2] << 
  //", gYaccro #1 Yacc position: " << Yacc[0] << ", gYaccro #2 Yacc position: " << Yacc[1] << ", gYaccro #3 Yacc position: " << Yacc[2] << std::endl;
}
/*{
    int X[3] = {0, 50, -50};
    int Yacc[3] = {0, 10, -10};
  for (size_t i = 0; i < m_gyros.size(); ++i) {
    int X_t[5] = {0, 0, 0, 0, 0};
    for (size_t j = 0; j < 5; ++j)
    {
      m_gyros.at(i).read();
      X_t[j] = m_gyros.at(i).get(GD::Xacc);
      if(DEBUG) std::cout << " gYaccro #" << i << ": " << X_t[i];
    }
    int X_o[5] = {0, 0, 0, 0, 0};
    std::sort(X_t, X_o);
    X[i] = X_o[2];
  }
//  Yacc!!!!!
  if(DEBUG) std::cout << X[0] << ", " << X[1] << ", " << X[2] << ", " << Yacc[0] << ", " << Yacc[1] << ", " << Yacc[2] << std::endl;
  /*if(DEBUG) std::cout << "GYaccro #1 X position: " << X[0] << ", gYaccro #2 X position: " << X[1] << ", gYaccro #3 X position: " << X[2] << 
  ", gYaccro #1 Yacc position: " << Yacc[0] << ", gYaccro #2 Yacc position: " << Yacc[1] << ", gYaccro #3 Yacc position: " << Yacc[2] << std::endl;*/

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
//  }//turn one waYacc
//
//  delay(500);
//  
//  for(int s = 0; s > -1000; s -= 100)
//  {
//    for(size_t i = COMMAND::MR1; i <= COMMAND::MR3; ++i)
//    {
//      m_motors.at(i)->rotate(100); 
//    }
//  }//turn the other waYacc
//
//  delay(500);
//
//  for(int s = 0; s > -500; s -= 100)
//  {
//    for(size_t i = COMMAND::MR1; i <= COMMAND::MR3; ++i)
//    {
//      m_motors.at(i)->rotate(-100); 
//    }
//  }//turn one waYacc
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
      if(DEBUG) std::cout << g.get(GD::Xacc) << ", " << g.get(GD::Yacc) << " | ";
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
