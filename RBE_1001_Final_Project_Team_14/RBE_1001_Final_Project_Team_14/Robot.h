#ifndef Robot_h
#define Robot_h

#include "helpers.h"
#include <servo.h>
#include <DFW.h> // DFW include
#include <Encoder.h>

////////
// IO //
////////

//Motors
const int driveTopLeft = 4;
const int driveBottomLeft = 5;
const int driveTopRight = 6;
const int driveBottomRight = 7;
const int armLeftPort = 9;
const int armRightPort = 10;
const int intakePort = 8;
//Digital IO
const int leftEncoderA = 2;
const int leftEncoderB = 3;
const int rightEncoderA = 18;
const int rightencoderB = 20;
const int barnButton = 29;
//Auto button is on 22;
//Analog Input
const int potPort = A0;
// Auto pot is on A1

///////////////////
// DEFINE MOTORS //
///////////////////
static Servo leftTopMotor;
static Servo leftBottomMotor;
static Servo rightTopMotor;
static Servo rightBottomMotor;
static Servo leftArm;
static Servo rightArm;
static Servo intake;

///////////////
// CONSTANTS //
///////////////
const double kArmBottomSetpoint = 0.08;
const double kArmMiddleSetpoint = 0.12;
const double kArmTopSetpoint = 0.4;
//Arm
const double kP_Arm = 1;
const double kArmGoodRange = 0.05;
//Drive auto
const double kP_Drive = 0.1;
const double kDriveGoodRange = 0.5;

///////////////
// FUNCTIONS //
///////////////

//General robot functions
void initRobot();
void teleopPeriodic(DFW dfw);
void autonomousPeriodic(bool colorRed);

//ARM FUNCITONS
void initArm();
void setArm(armState state);
double getArm();
bool armGood();
bool armGood(int delay);
void setIntake(intakeState state);

//DRIVETRAIN FUNCTIONS
void initDrivetrain();
void tankDrive(double leftInput, double rightInput);
void arcadeDrive(double throttle, double turn);
double getLeftEncoder();
double getRightEncoder();
double getAverageEncoder();
void resetEncoders();
int getGyro();
//Auto drivetrain functions
bool driveInRange(int delay);
void driveDistance(double distance);
bool turnInRange(int delay);
void turnAngle(int angle);



#endif
