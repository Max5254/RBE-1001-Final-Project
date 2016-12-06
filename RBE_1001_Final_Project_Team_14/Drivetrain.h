#ifndef Drivetrain_h
#define Drivetrain_h

#include <servo.h>
#include <Encoder.h>
#include "ControlLoops.h"


static Servo leftFrontMotor;
static Servo leftRearMotor;
static Servo rightFrontMotor;
static Servo rightRearMotor;



void initDrivetrain(int LF, int LR, int RF, int RR);
void tankDrive(double leftInput, double rightInput);
void arcadeDrive(double throttle, double turn);
int frcToServo(double input);
int getLeftEncoder();
int getRightEncoder();
int getAverageEncoder();
void resetEncoders();
int getGyro();
//AUTO STUFF
bool driveInRange(int delay);
void driveDistance(double distance);
bool turnInRange(int delay);
void turnAngle(int angle);


#endif
