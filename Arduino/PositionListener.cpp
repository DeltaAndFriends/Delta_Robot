#include <Arduino.h>
#include "PositionListener.h"

namespace Delta
{
  const int ARR_LENGTH = 5;
  const int ARR_OFFSET = 2;
  Position data[ARR_LENGTH];
  std::vector<float> roll_1;
  std::vector<float> roll_2;
  std::vector<float> roll_3;
  std::vector<float> pitch_1;
  std::vector<float> pitch_2;
  std::vector<float> pitch_3;

  PositionListener::PositionListener()
  : m_gyros({Gyro{GYRO_PIN::_1}, Gyro{GYRO_PIN::_2}, Gyro{GYRO_PIN::_3}})
  {
  }
  
  PositionListener::~PositionListener()
  {
  }

  void PositionListener::setup();

  Position PositionListener::get()
  {
    Position p;
      for (size_t i = 0; i < m_gyros.size(); ++i) {
        m_gyros.at(i).read();
        p.roll_1 = m_gyros.at(0).get(GD::Yacc);
        p.pitch_1 = m_gyros.at(0).get(GD::Yacc);
        p.roll_2 = m_gyros.at(1).get(GD::Yacc);
        p.pitch_2 = m_gyros.at(1).get(GD::Yacc);
        p.roll_3 = m_gyros.at(2).get(GD::Yacc);
        p.pitch_3 = m_gyros.at(2).get(GD::Yacc);
      }
    return p;
  }

  Position PositionListener::getFiltered(){
    
    Position p;
    for(size_t i = 0; i < ARR_LENGTH; i++)
    {
      Position gettedPosition = get();
      roll_1.push_back(gettedPosition.roll_1);
      roll_2.push_back(gettedPosition.roll_2);
      roll_3.push_back(gettedPosition.roll_3);
      pitch_1.push_back(gettedPosition.pitch_1);
      pitch_2.push_back(gettedPosition.pitch_2);
      pitch_3.push_back(gettedPosition.pitch_3);
    }

    std::sort(roll_1.begin(), roll_1.end());

    for(size_t i =ARR_OFFSET; i < ARR_LENGTH - ARR_OFFSET; i++)
    {
    p.roll_1 += roll_1.at(i);
    p.roll_2 += roll_2.at(i);
    p.roll_3 += roll_3.at(i);
    p.pitch_1 += pitch_1.at(i);
    p.pitch_2 += pitch_2.at(i);
    p.pitch_3 += pitch_3.at(i);
    }
    p.roll_1 /= ARR_LENGTH - 2*ARR_OFFSET;
    p.roll_2 /= ARR_LENGTH - 2*ARR_OFFSET;
    p.roll_3 /= ARR_LENGTH - 2*ARR_OFFSET;
    p.pitch_1 /= ARR_LENGTH - 2*ARR_OFFSET;
    p.pitch_2 /= ARR_LENGTH - 2*ARR_OFFSET;
    p.pitch_3 /= ARR_LENGTH - 2*ARR_OFFSET;

    
    return p;
  }


} // namespace Delta
