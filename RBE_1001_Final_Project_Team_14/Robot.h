#ifndef Robot_h
#define Robot_h

#include "Drivetrain.h"
#include "Arm.h"
#include <DFW.h> // DFW include


//Set ALL the variables
const int kArmBottomSetpoint = 10;
const int kArmMiddleSetpoint = 50;
const int kArmTopSetpoint = 100;

//PORTS
const int armLeft = 0;
const int armRight = 0;

const int driveFrontLeft = 0;
const int driveRearLeft = 0;
const int driveFrontRight = 0;
const int driveRearRight = 0;

const int intakePort = 0;

//Encoders are defined as:
//Left - 0,1
//Right - 2,3
const int potPort = A0;






void initRobot();
void teleopPeriodic(DFW dfw);
void autonomousPeriodic();



#endif
