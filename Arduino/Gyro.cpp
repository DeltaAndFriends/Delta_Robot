#include "Gyro.h"
#include <Arduino.h>
#include <Wire.h>

namespace Delta
{

  const int MPU_addr=0x68;  // I2C address of the MPU-6050
  const int PWR_reg=0x6B;  // PWR_MGMT_1
  const int START_reg=0x3B;  // ACCEL_XOUT_H

  Gyro::Gyro(GYRO_PIN pin)
//  : m_data(size_t(GD::qty))
  : m_pin{size_t(pin)}
  {
//    for(auto &d : m_data)
//    {
//      d = 0;
//    }
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
  
  void Gyro::read()
  {
    digitalWrite (m_pin, HIGH);
    Wire.beginTransmission(MPU_addr);
    Wire.write(START_reg);  // starting register
    int error_code = Wire.endTransmission(false);
    if (error_code && DEBUG)
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
      Wire.requestFrom(MPU_addr,4,true);  // request a total of 14 registers
      m_data[size_t(GD::Xacc)]=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
      m_data[size_t(GD::Yacc)]=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    }
    else
    {
      m_data[size_t(GD::Xacc)] = m_data[size_t(GD::Yacc)] = gyro_error;      
    }
    digitalWrite (m_pin, LOW);    
  }


  int Gyro::get_raw(GD d) const
  {
    int r = m_data[size_t(d)];
    return r;
  }

  float Gyro::get(GD d) const
  {
    int r = (m_data[size_t(d)] - 1000) / 160;// / 175 - 700;
    return r;
  }
      
} // namespace Delta
