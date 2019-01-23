// sudo chmod a+rw /dev/ttYaccUSB0
#include "Init.h"
#include "Scenarios.h"
//#include "Geometry.h"

void setup() {
  if (DEBUG) Serial.begin(9600);
  if (DEBUG) Serial.println("Hi");
  //if(DEBUG) std::cout << "START " << std::endl;
  robot.setup();
  if (DEBUG) Serial.println("setup ok");
}

void check_mode()
{
  // TODO: instead of static must be mode switcher, e.g. signal from a button
  static int mode = 1;
  static int cur_mode = 0;
  if (cur_mode == mode)
  {    //private:
    return;
  }
  cur_mode = mode;

  using namespace Delta;

  switch (cur_mode)
  {
    case 1:
      robot.set_scenario(Scenario1);
      break;
    case 2:
      robot.set_scenario(Scenario2);
      break;
  }
}

void loop() {
  robot.test();
}
void _loop() {
  delay(100);
  robot.home();
  delay(2000);
  robot.achieve(60, 30, -60, 0, 0, 0);
  delay(2000);
  robot.achieve(40, -60, -10, 0, 0, 0);
  delay(2000);
  robot.achieve(-40, -30, 10, 0, 0, 0);
  delay(2000);
  robot.achieve(0, 50, -50, 0, 10, -10);
  delay(2000);
  robot.achieve(0, -40, 50, 0, 20, -20);
    delay(2000);
  robot.achieve(0, 40, -40, 0, 10, -10);
  delay(2000);
  robot.achieve(0, -50, 40, 0, 0, 0);
    delay(2000);
  robot.achieve(0, 50, -50, -10, 10, 0);
  delay(2000);
  robot.achieve(0, -40, 50, -20, 20, 0);
  delay(2000);
  robot.achieve(0, 40, -40, -10, 10, 0);
  delay(2000);
  robot.achieve(0, -50, 50, 10, 10, 10);
  delay(2000);
  robot.achieve(0, 40, -40, 30, 30, 30);
/*  while(true)
  {
    delay(1000);
    robot.test();
  }
    //if (DEBUG) Serial.println(" ");
    dela(50000000000000000000);*/
}
