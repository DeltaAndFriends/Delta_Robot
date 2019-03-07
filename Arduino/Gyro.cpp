#include "Gyro.h"
#include <Arduino.h>
#include <Wire.h>

  const int MPU_addr=0x68;  // I2C address of the MPU-6050
  const int PWR_reg=0x6B;  // PWR_MGMT_1
  const int START_reg=0x3B;  // ACCEL_XOUT_H

  Gyro::Gyro(GYRO_PIN pin)
  : m_pin{size_t(pin)}
  {
    for(auto &d : m_data)
    {
      d = 0;
    }
  }
  
  Gyro::~Gyro()
  {
    
  }

  void Gyro::setup()
  {
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, HIGH);
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(PWR_reg);
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    if ( Wire.endTransmission(true) )   
    {  
      Serial.println("Error setupWire");
    }
    digitalWrite (m_pin, LOW); 
  }
  
  void Gyro::read() //TODO: read each value into a vector of size a, find the medium value by removing the top and bottom 10% and find the average of what's left. Try a 3, 5, 10, 30 and compare them with excel.
  {
    digitalWrite (m_pin, HIGH);
    Wire.beginTransmission(MPU_addr);
    Wire.write(START_reg);  // starting register
    int error_code = Wire.endTransmission(false);
    if (error_code)
    {
      switch (error_code)
      {
      case 1:
      std::cout << "data too long to fit in transmit buffer";
      break;
      case 2:
      std::cout << "unsuccessful transmit of address";
      break;
      case 3:
      std::cout << "unsuccessful transmit of data";
      break;
      default:
      std::cout << "other error";
      break;
      }
    }

    if (!error_code)
    {
      Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
      m_data[size_t(GD::Xacc)]=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
      m_data[size_t(GD::Yacc)]=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
	  m_data[size_t(GD::Zacc)]=Wire.read()<<8|Wire.read(); 
      m_data[size_t(GD::Temp)]=Wire.read()<<8|Wire.read();
	  m_data[size_t(GD::Xgyro)]=Wire.read()<<8|Wire.read(); 
      m_data[size_t(GD::Ygyro)]=Wire.read()<<8|Wire.read();
	  m_data[size_t(GD::Zgyro)]=Wire.read()<<8|Wire.read(); 
    }
    else
    {
      m_data[size_t(GD::Xacc)] = m_data[size_t(GD::Yacc)] = gyro_error;      
    }
    digitalWrite (m_pin, LOW);    
  }


  int Gyro::get_raw(GD d) const
  {
    return m_data[size_t(d)];
  }

  double Gyro::getacc(GD d) const
  {
    return m_data[size_t(d)]/16384.0; // get acceleration in gs
  }
  
  double Gyro::get(Angle a)
  {
    tilt_switch = digitalRead(p_tilt_switch) ? -1 : 1;
    
    switch (a)
    {
      case Angle::pitch :
      return tilt_switch * atan2(getacc(GD::Xacc), sqrt(getacc(GD::Yacc)*getacc(GD::Yacc) + getacc(GD::Zacc)*getacc(GD::Zacc)))*180/3.14159265; //ADD G SENSOR TO POSITION LISTENER AND 
      //MULTIPLY BOTH PITCH AND ROLL BY -1 IF THE SENSOR IS UPSIDE DOWN
      //atan(X/sqrt(Y^2+Z^2)
      break;
      case Angle::roll :
      return tilt_switch * -atan2(getacc(GD::Yacc), sqrt(getacc(GD::Xacc)*getacc(GD::Xacc) + getacc(GD::Zacc)*getacc(GD::Zacc)))*180/3.14159265;
      //-atan(Y/sqrt(X^2+Z^2)
      break;
      default : break;
    }
  }
