#ifndef Init_h
#define Init_h

#include "Config.h"
#include "Robot.h"

static const Delta::Geometry c_geom
{{
  robot_main_basis,
  robot_tool_basis,
  robot_lever1,
  robot_lever2_h,
  robot_lever2_w
}};

Delta::Robot robot(c_geom, MOTOR_type);

#endif // Init_h

    
