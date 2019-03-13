
#include "Stepper.h"

void  adjustSpeedScales();
void  setNextInterruptInterval();


void M1Step() {
  M1STEP_H
  M1STEP_L
}

void M2Step() {
  M2STEP_H
  M2STEP_L
}

void M3Step() {
  M3STEP_H
  M3STEP_L
}

void M4Step() {
  M4STEP_H
  M4STEP_L
}

void M5Step() {
  M5STEP_H
  M5STEP_L
}

void M6Step() {
  M6STEP_H
  M6STEP_L
}

void M1DIR(int dir) {
  digitalWrite(48, dir);
}
void M2DIR(int dir) {
  digitalWrite(61, dir);
}
void M3DIR(int dir) {
  digitalWrite(55, dir);
}
void M4DIR(int dir) {
  digitalWrite(43, dir);
}
void M5DIR(int dir) {
  digitalWrite(34, dir);
}
void M6DIR(int dir) {
  digitalWrite(28, dir);
}
void resetStepperInfo( stepperInfo& si ) {
  si.n = 0;
  si.d = 0;
  si.di = 0;
  si.stepCount = 0;
  si.rampUpStepCount = 0;
  si.rampUpStepTime = 0;
  si.totalSteps = 0;
  si.stepPosition = 0;
  si.movementDone = false;
}
#define NUM_STEPPERS 6
volatile stepperInfo steppers[NUM_STEPPERS];


void resetStepper(volatile stepperInfo& si) {
  si.c0 = si.acceleration;
  si.d = si.c0;
  si.di = si.d;
  si.stepCount = 0;
  si.n = 0;
  si.rampUpStepCount = 0;
  si.movementDone = false;
  si.speedScale = 1;

  float a = si.minStepInterval / (float)si.c0;
  a *= 0.676;

  float m = ((a*a - 1) / (-2 * a));
  float n = m * m;

  si.estStepsToSpeed = n;
}

volatile byte remainingSteppersFlag = 0;

float getDurationOfAcceleration(volatile stepperInfo& s, unsigned int numSteps) {
  /*float d = s.c0;
  float totalDuration = 0;
  for (unsigned int n = 1; n < numSteps; n++) {
    d = d - (2 * d) / (4 * n + 1);
    totalDuration += d;
  }
  return totalDuration;*/
  return s.c0 * sqrt(1.138 * numSteps + numSteps);
}

void prepareMovement(int whichMotor, long steps) {
  if(steps == 0){
    return;
  }
  volatile stepperInfo& si = steppers[whichMotor];
  si.dirFunc( steps < 0 ? HIGH : LOW );
  si.dir = steps > 0 ? 1 : -1;
  si.totalSteps = abs(steps);
  resetStepper(si);
  
  remainingSteppersFlag |= (1 << whichMotor);

  unsigned long stepsAbs = abs(steps);

  if ( (2 * si.estStepsToSpeed) < stepsAbs ) {
    // there will be a period of time at full speed
    unsigned long stepsAtFullSpeed = stepsAbs - 2 * si.estStepsToSpeed;
    float accelDecelTime = getDurationOfAcceleration(si, si.estStepsToSpeed);
    si.estTimeForMove = 2 * accelDecelTime + stepsAtFullSpeed * si.minStepInterval;
  }
  else {
    // will not reach full speed before needing to slow down again
    float accelDecelTime = getDurationOfAcceleration( si, stepsAbs / 2 );
    si.estTimeForMove = 2 * accelDecelTime;
  }
}

volatile byte nextStepperFlag = 0;

void setNextInterruptInterval() {

  bool movementComplete = true;

  unsigned long mind = 999999;
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ((1 << i) & remainingSteppersFlag) && steppers[i].di < mind ) {
      mind = steppers[i].di;
    }
  }

  nextStepperFlag = 0;
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ! steppers[i].movementDone )
      movementComplete = false;
    if ( ((1 << i) & remainingSteppersFlag) && steppers[i].di == mind )
      nextStepperFlag |= (1 << i);
  }

  if ( remainingSteppersFlag == 0 ) {
    TIMER1_INTERRUPTS_OFF
    OCR1A = 65500;
  }

  OCR1A = mind;
}

ISR(TIMER1_COMPA_vect)
{
  unsigned int tmpCtr = OCR1A;

  OCR1A = 65500;

  for (int i = 0; i < NUM_STEPPERS; i++) {

    if ( ! ((1 << i) & remainingSteppersFlag) )
      continue;

    if ( ! (nextStepperFlag & (1 << i)) ) {
      steppers[i].di -= tmpCtr;
      continue;
    }

    volatile stepperInfo& s = steppers[i];

    if ( s.stepCount < s.totalSteps ) {
      s.stepFunc();
      s.stepCount++;
      s.stepPosition += s.dir;
      if ( s.stepCount >= s.totalSteps ) {
        s.movementDone = true;
        remainingSteppersFlag &= ~(1 << i);
      }
    }

    if ( s.rampUpStepCount == 0 ) {
      s.n++;
      s.d = s.d - (2 * s.d) / (4 * s.n + 1);
      if ( s.d <= s.minStepInterval ) {
        s.d = s.minStepInterval;
        s.rampUpStepCount = s.stepCount;
      }
      if ( s.stepCount >= s.totalSteps / 2 ) {
        s.rampUpStepCount = s.stepCount;
      }
      s.rampUpStepTime += s.d;
    }
    else if ( s.stepCount >= s.totalSteps - s.rampUpStepCount ) {
      s.d = (s.d * (4 * s.n + 1)) / (4 * s.n + 1 - 2);
      s.n--;
    }

    s.di = s.d * s.speedScale; // integer
  }

  setNextInterruptInterval();

  TCNT1  = 0;
}


void runAndWait() {
  adjustSpeedScales();
  setNextInterruptInterval();
  TCNT3 = 65500;
  TIMER1_INTERRUPTS_ON
  while ( remainingSteppersFlag );
  remainingSteppersFlag = 0;
  nextStepperFlag = 0;
}

void adjustSpeedScales() {
  float maxTime = 0;
  
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ! ((1 << i) & remainingSteppersFlag) )
      continue;
    if ( steppers[i].estTimeForMove > maxTime )
      maxTime = steppers[i].estTimeForMove;
  }

  if ( maxTime != 0 ) {
    for (int i = 0; i < NUM_STEPPERS; i++) {
      if ( ! ( (1 << i) & remainingSteppersFlag) )
        continue;
      steppers[i].speedScale = maxTime / steppers[i].estTimeForMove;
    }
  }
}

void steppersInit(){
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 1000;                             // compare value
  TCCR1B |= (1 << WGM12);                   // CTC mode
  TCCR1B |= ((1 << CS11) | (1 << CS10));    // 64 prescaler
  interrupts();

  double accel = 1600;
  double del = 250;
  steppers[0].dirFunc = M1DIR;
  steppers[0].stepFunc = M1Step;
  steppers[0].acceleration = accel;
  steppers[0].minStepInterval = del;
  
  steppers[1].dirFunc = M2DIR;
  steppers[1].stepFunc = M2Step;
  steppers[1].acceleration = accel;
  steppers[1].minStepInterval = del;

  steppers[2].dirFunc = M3DIR;
  steppers[2].stepFunc = M3Step;
  steppers[2].acceleration = accel;
  steppers[2].minStepInterval = del;

  steppers[3].dirFunc = M4DIR;
  steppers[3].stepFunc = M4Step;
  steppers[3].acceleration = accel;
  steppers[3].minStepInterval = del;

  steppers[4].dirFunc = M5DIR;
  steppers[4].stepFunc = M5Step;
  steppers[4].acceleration = accel;
  steppers[4].minStepInterval = del;

  steppers[5].dirFunc = M6DIR;
  steppers[5].stepFunc = M6Step;
  steppers[5].acceleration = accel;
  steppers[5].minStepInterval = del;
}
void test(){
  for (int i = 0; i < NUM_STEPPERS; i++) {
    prepareMovement( i, 800 );
    runAndWait();
  }
  prepareMovement( 0, 8000 );
  prepareMovement( 1,  800 );
  prepareMovement( 2, 2400 );
  prepareMovement( 3, 800 );
  prepareMovement( 4, 1600 );
  prepareMovement( 5, 800 );
  runAndWait();
  delay(1000);
  prepareMovement( 0, -8000 );
  prepareMovement( 1,  1600 );
  prepareMovement( 2, -2400 );
  prepareMovement( 3, -800 );
  prepareMovement( 4, 2400 );
  prepareMovement( 5, -800 );
  runAndWait();
}
