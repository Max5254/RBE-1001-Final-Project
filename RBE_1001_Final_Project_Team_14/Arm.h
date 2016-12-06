#ifndef Arm_h
#define Arm_h

#include <servo.h>
#include "ControlLoops.h"
#include "Arduino.h"


static Servo leftArm;
static Servo rightArm;
static Servo intake;


void initArm(int leftPort, int rightPort, int intakePort, int potPort);
void setArm(int angle);
void setIntakeIn();
void setIntakeOut();
void setIntakeOff();


#endif




