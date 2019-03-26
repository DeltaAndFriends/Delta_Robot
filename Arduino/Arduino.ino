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

int arr1[18][6] = {
  {453, 444, 187, -9, 0, -9},
{0, 0, 0, 9, 0, 0},
{-267, 258, -498, 258, -53, -169},
{0, 0, 0, -9, 0, 0},
{-36, 36, 747, -249, 18, 160},
{0, 0, 9, 0, 9, -9},
{-649, -80, 80, -9, 71, -107},
{0, 0, 0, 0, 0, 0},
{702, -142, 258, 18, -44, 133},
{9, 0, 0, 0, 0, 0},
{-240, -907, -169, -373, 187, 27},
{9, 0, 0, 18, 0, 0},
{604, 560, -71, 391, -178, 44},
{0, 0, 0, 0, 0, 0},
{-658, -178, -427, -356, -302, -364},
{0, 0, 0, -9, 0, 0}
};

int arr3[9][6] = { //try magnet
{480, 453, 196, -53, -62, -53}, 
{-400, -18, -444, 489, 36, 116}, 
{71, 302, 747, -427, 36, -62}, 
{-587, -204, -80, -18, 71, -178}, 
{640, 18, 356, 9, -107, 178}, 
{-311, -907, -284, -258, 187, 36}, 
{702, 533, 62, 293, -160, 27}, 
{-222, -516, -613, -36, -382, 160}, 
{-498, 169, 178, 222, 631, 62}
};

int arr2[8][6] = { //weird moves
  {215, 224, 35, -81, -7, 68},
  {444, -354, 235, -6, -86, 177},
  {-602, 321, 168, -187, 216, -259},
  {-279, 253, -564, 662, -189, -378}, 
  {787, -530, -86, -413, -452, 607},
  {-693, -166, 411, -541, 574, -240}, 
  {477, 372, -578, 704, -484, 128},
  {-788, -233, 239, -356, 475, -333}
};

int arrn[23][6] = {
  {436, 498, 178, -62, -36, -36},
{0, 0, 0, 0, 9, 9},
{-44, -702, -400, -18, -98, 62},
{364, -27, 9, 53, -116, 382},
{-18, 98, 89, -9, -27, -9},
{-533, 9, -36, 89, 231, -231},
{-44, 862, 702, -36, 36, -204},
{9, 0, 0, 0, 0, 0},
{231, -844, -631, -18, -71, 89},
{347, 0, -71, -36, -36, 347},
{-9, 53, 53, -9, -18, 53},
{-693, 98, 160, 196, 347, -249},
{133, 516, 684, -142, -178, -240},
{0, 0, 0, 0, 9, 0},
{311, -560, -702, 62, -62, 178},
{249, -53, -169, -116, -80, 293},
{-18, 53, 44, -18, -36, 27},
{-462, 0, 89, 133, 231, -178},
{347, 187, 533, -36, -133, -204},
{0, 0, 0, 0, 0, 0},
{151, -391, -764, -53, -142, 453},
{-44, 187, 142, -44, 9, 9},
{-160, 98, 151, -80, -338, -649}
};

 int arrc[72][6] = { 
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
{28, -42, 9, 0, 0, 0}
};

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

bool oddLoop = true;
void loop() {
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  delay(10);

 while(0){
   gyro_on_demand();
   delay(100);
    }

    while(1){
    robot.achieve(0,0,0,0,0,0);
    robot.achieve(0,0,0,0,0,0);
    
    //robot.achieve(0,0,0,0,0,0);
    //throwThings();
    collectThings();
    //collectToTrash();
    robot.achieve(0,0,0,0,0,0);
    drawCircle();
    robot.achieve(0,0,0,0,0,0);
    doStrangeMoves();
    


  }
}
void throwThings(){
    int k = 1;
    //robot.moveMotors(-178/k, 289/k, 590/k, 0, 0, 0);
    for(int j = 0; j < 1u; j++){
      
      for(int i = 0; i < sizeof(arr1)/(6*sizeof(int)); i++){
       robot.moveMotors(
       -arr1[i][0]/k,
       -arr1[i][1]/k,
       -arr1[i][2]/k,
       arr1[i][3]/k,
       arr1[i][4]/k,
       arr1[i][5]/k);
       Serial.println(i);
       if(i == 0 || i == 3 || i == 5 || i == 7 || i == 8 || i == 11 || i == 12 || i == 15){ //0, 5, 6, 13
        Serial.println("!");
        digitalWrite(32, !digitalRead(32));
        delay(100);
      }
      } 
      
    }
}
void doStrangeMoves(){
    int k = 1;
    //robot.moveMotors(-178/k, 289/k, 590/k, 0, 0, 0);
    for(int j = 0; j < 1u; j++){
      
      for(int i = 0; i < sizeof(arr2)/(6*sizeof(int)); i++){
       robot.moveMotors(
       -arr2[i][0]/k,
       -arr2[i][1]/k,
       -arr2[i][2]/k,
       arr2[i][3]/k,
       arr2[i][4]/k,
       arr2[i][5]/k);
       Serial.println(i);
     
      } 
      
    }
}

void drawCircle(){
    int k = 1;
    robot.moveMotors(-178/k, 289/k, 590/k, 0, 0, 0);
    for(int j = 0; j < 1u; j++){
      
      for(int i = 0; i < sizeof(arrc)/(6*sizeof(int)); i++){
       robot.moveMotors(
       -arrc[i][0]/k,
       -arrc[i][1]/k,
       -arrc[i][2]/k,
       arrc[i][3]/k,
       arrc[i][4]/k,
       arrc[i][5]/k);
      } 
      
    }
}
void collectThings(){
  int k = 1;
    //robot.moveMotors(-178/k, 289/k, 590/k, 0, 0, 0);
    
    for(int j = 0; j < 1u; j++){
      
      for(int i = 0; i < sizeof(arr1)/(6*sizeof(int)); i++){
       robot.moveMotors(
       -arr1[i][0]/k,
       -arr1[i][1]/k,
       -arr1[i][2]/k,
       arr1[i][3]/k,
       arr1[i][4]/k,
       arr1[i][5]/k);
       Serial.println(i);
       if(oddLoop){
        if(i == 5 || i == 12){
          Serial.println("!");
          digitalWrite(32, false);
          delay(100);
        }
        if(i == 0 || i == 6 ){
          Serial.println("!");
          digitalWrite(32, true);
          delay(100);
        }
       } else {
        if(i == 0 || i == 8){
          Serial.println("!");
          digitalWrite(32, false);
          delay(100);
        }
        if(i == 5 || i == 11 ){
          Serial.println("!");
          digitalWrite(32, true);
          delay(100);
        }
        
       }
      } 
      
    }
    oddLoop = !oddLoop;
}
void collectToTrash(){
  int k = 1;      
      for(int i = 0; i < sizeof(arrn)/(6*sizeof(int)); i++){
       robot.moveMotors(
       -arrn[i][0]/k,
       -arrn[i][1]/k,
       -arrn[i][2]/k,
       arrn[i][3]/k,
       arrn[i][4]/k,
       arrn[i][5]/k);
       Serial.println(i);
        if(i == 0 || i == 1 || i == 6 || i == 7 || i == 12 || i == 13 || i == 18 || i == 19){
          Serial.println("!");
          digitalWrite(32, true);
          delay(100);
        }
        if(i == 4 || i == 5 || i== 11 || i == 17  ){
          Serial.println("!");
          digitalWrite(32, false);
          delay(100);
        }
    } 
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

    Serial.print("{");
    for (int i = 0; i < 6; ++i)
    {
    Serial.print(round((gyro_current_data[i] - gyro_last_data[i])*80/9));
    Serial.print(", ");
    gyro_last_data[i] = gyro_current_data[i];
    }
    Serial.println("\b\b},");
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
