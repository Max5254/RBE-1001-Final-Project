#include "Drivetrain.h"


Encoder leftEncoder(1, 2);
Encoder rightEncoder(3, 4);

ControlLoops drivePID;
ControlLoops headingPID;
ControlLoops turnPID;

void initDrivetrain(int LF, int LR, int RF, int RR){
  leftFrontMotor.attach(LF);
  leftRearMotor.attach(LR);
  rightFrontMotor.attach(RF);
  rightRearMotor.attach(RR);

  //PID STUFF
  drivePID.setOutputLimits(-1, 1);
  drivePID.initPID(1, 0, 0, 20); //Kp,Ki,Kd,Dt
  drivePID.setAcceptableRange(0.25); //Acceptable range of sensor

  headingPID.setOutputLimits(-1, 1);
  headingPID.initPID(1, 0, 0, 20); //Kp,Ki,Kd,Dt
  headingPID.setAcceptableRange(0.25); //Acceptable range of sensor

  turnPID.setOutputLimits(-1, 1);
  turnPID.initPID(1, 0, 0, 20); //Kp,Ki,Kd,Dt
  turnPID.setAcceptableRange(0.25); //Acceptable range of sensor

}

void tankDrive(double leftInput, double rightInput){
  int left = frcToServo(leftInput);
  int right = frcToServo(rightInput);
  leftFrontMotor.write(left);
  leftRearMotor.write(left);
  rightFrontMotor.write(right);
  rightRearMotor.write(right);
}

void arcadeDrive(double throttle, double turn){
    tankDrive(throttle + turn , turn - throttle);
}

int frcToServo(double input){
    return 90 + (input * 90.0);
}
int getLeftEncoder(){
  return leftEncoder.read();
}
int getRightEncoder(){
  return rightEncoder.read();
}
int getAverageEncoder(){
  return (getRightEncoder() + getLeftEncoder()) / 2;
}
void resetEncoders(){
  leftEncoder.write(0);
  rightEncoder.write(0);
}
int getGyro(){
  //TODO:  do some math and return an angle
  return 0;
}

bool driveInRange(){
  return drivePID.inRange(250) && headingPID.inRange(250);
}

void driveDistance(double distance){


}

bool turnInRange(){
  return turnPID.inRange(250);
}

void turnAngle(int angle){

}
