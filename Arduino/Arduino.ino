// sudo chmod a+rw /dev/ttyUSB0
#include "Init.h"
#include "Scenarios.h"

const int buttonPin = 16;    // the number of the pushbutton pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
bool State = false;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {
  Serial.begin(2000000);
  pinMode(buttonPin, INPUT);
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
  {
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

int arr[72][6] = {
    {21, -39, 13, 0, 0, 0},
    {17, -37, 15, 0, 0, 0},
    {13, -35, 18, 0, 0, 0},
    {9, -32, 20, 0, 0, 0},
    {5, -30, 22, 0, 0, 0},
    {1, -27, 25, 0, 0, 0},
    {-3, -24, 28, 0, 0, 0},
    {-7, -22, 30, 0, 0, 0},
    {-11, -19, 33, 0, 0, 0},
    {-14, -17, 35, 0, 0, 0},
    {-18, -14, 38, 0, 0, 0},
    {-22, -12, 40, 0, 0, 0},
    {-26, -10, 41, 0, 0, 0},
    {-29, -8, 42, 0, 0, 0},
    {-33, -6, 43, 0, 0, 0},
    {-36, -4, 43, 0, 0, 0},
    {-38, -2, 43, 0, 0, 0},
    {-41, 0, 42, 0, 0, 0},
    {-42, 1, 40, 0, 0, 0},
    {-43, 3, 38, 0, 0, 0},
    {-44, 5, 35, 0, 0, 0},
    {-43, 7, 32, 0, 0, 0},
    {-42, 9, 28, 0, 0, 0},
    {-41, 11, 25, 0, 0, 0},
    {-39, 13, 21, 0, 0, 0},
    {-37, 15, 17, 0, 0, 0},
    {-35, 18, 13, 0, 0, 0},
    {-32, 20, 9, 0, 0, 0},
    {-30, 22, 5, 0, 0, 0},
    {-27, 25, 1, 0, 0, 0},
    {-24, 28, -3, 0, 0, 0},
    {-22, 30, -7, 0, 0, 0},
    {-19, 33, -11, 0, 0, 0},
    {-17, 35, -14, 0, 0, 0},
    {-14, 38, -18, 0, 0, 0},
    {-12, 40, -22, 0, 0, 0},
    {-10, 41, -26, 0, 0, 0},
    {-8, 42, -29, 0, 0, 0},
    {-6, 43, -33, 0, 0, 0},
    {-4, 43, -36, 0, 0, 0},
    {-2, 43, -38, 0, 0, 0},
    {0, 42, -41, 0, 0, 0},
    {1, 40, -42, 0, 0, 0},
    {3, 38, -43, 0, 0, 0},
    {5, 35, -44, 0, 0, 0},
    {7, 32, -43, 0, 0, 0},
    {9, 28, -42, 0, 0, 0},
    {11, 25, -41, 0, 0, 0},
    {13, 21, -39, 0, 0, 0},
    {15, 17, -37, 0, 0, 0},
    {18, 13, -35, 0, 0, 0},
    {20, 9, -32, 0, 0, 0},
    {22, 5, -30, 0, 0, 0},
    {25, 1, -27, 0, 0, 0},
    {28, -3, -24, 0, 0, 0},
    {30, -7, -22, 0, 0, 0},
    {33, -11, -19, 0, 0, 0},
    {35, -14, -17, 0, 0, 0},
    {38, -18, -14, 0, 0, 0},
    {40, -22, -12, 0, 0, 0},
    {41, -26, -10, 0, 0, 0},
    {42, -29, -8, 0, 0, 0},
    {43, -33, -6, 0, 0, 0},
    {43, -36, -4, 0, 0, 0},
    {43, -38, -2, 0, 0, 0},
    {42, -41, 0, 0, 0, 0},
    {40, -42, 1, 0, 0, 0},
    {38, -43, 3, 0, 0, 0},
    {35, -44, 5, 0, 0, 0},
    {32, -43, 7, 0, 0, 0},
    {28, -42, 9, 0, 0, 0},
    {28, -42, 9, 0, 0, 0}};

void loop() {
  delay(10);

  while(1){
    gyro_on_demand();
    delay(100);
  }

  while(1){

    robot.achieve(0,0,0,0,0,0);
    robot.achieve(0,0,0,0,0,0);

    int k = 1;
    robot.moveMotors(-178/k, 289/k, 590/k, 0, 0, 0);
    for(int j = 0; j < 10; j++){
      digitalWrite(32, !digitalRead(32));
      for(int i = 0; i < 72; i++){
       robot.moveMotors(
       -arr[i][0]/k,
       -arr[i][1]/k,
       -arr[i][2]/k,
       -arr[i][3]/k,
       -arr[i][4]/k,
       -arr[i][5]/k);
      } 
    }
  }
  while(1);
}

float gyro_last_data[6] = {0, 0, 0, 0, 0, 0};
float gyro_current_data[6] = {0, 0, 0, 0, 0, 0};

void gyro_vector() {
    for(int i = 0; i < 3; i++){
      robot.m_gyros.at(i).read();
    }

    for(int i = 0; i < 3; ++i)
    {
      gyro_current_data[i] = robot.m_gyros.at(i).getAvg(Angle::pitch);
      gyro_current_data[i+3] = robot.m_gyros.at(i).getAvg(Angle::roll);
    }

    Serial.print("(");
    for (int i = 0; i < 6; ++i)
    {
    Serial.print(round((gyro_current_data[i] - gyro_last_data[i])*80/9));
    Serial.print(", ");
    gyro_last_data[i] = gyro_current_data[i];
    }
    Serial.println("\b\b)");
      delay(100);
  }

  void gyro_check() {
    for(int i = 0; i < 3; i++){
      robot.m_gyros.at(i).read();
    }
  
      Serial.print(robot.m_gyros.at(0).get(Angle::pitch));
      Serial.print(", ");
      Serial.print(robot.m_gyros.at(1).get(Angle::pitch));
      Serial.print(", ");
      Serial.print(robot.m_gyros.at(2).get(Angle::pitch));
      Serial.print(", ");

      Serial.print(robot.m_gyros.at(0).get(Angle::roll));
      Serial.print(", ");
      Serial.print(robot.m_gyros.at(1).get(Angle::roll));
      Serial.print(", ");
      Serial.print(robot.m_gyros.at(2).get(Angle::roll));

      Serial.println();
      delay(100);
  /*
    for(int i = 0; i < 3; ++i){
      robot.m_gyros.at(i).read();
      Serial.print(robot.m_gyros.at(i).get(Angle::pitch));
      Serial.print(", ");
    }
    
    for(int i = 0; i < 2; ++i){
      Serial.print(robot.m_gyros.at(i).get(Angle::roll));
      Serial.print(", ");
    }
    Serial.println(robot.m_gyros.at(2).get(Angle::roll));
*/
  }

  int gyroFlag = 0;
  
  void gyro_on_demand(){
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        State = !State;
      }
    }
  }

  if(buttonState && gyroFlag)
  {
    gyro_vector(); //gyro_check(); //yay finally 
    gyroFlag = 0;
  }
  if(!buttonState){
    gyroFlag = 1;
  }
  
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
