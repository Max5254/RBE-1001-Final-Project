#include "robot.h"

int armSetpoint;

void initRobot(){
  initArm(driveFrontLeft,driveRearLeft,driveFrontRight,driveFrontLeft);
  initDrivetrain(armLeft,armRight,intakePort,potPort);
}


void teleopPeriodic(DFW dfw){
  //DRIVE
  //tankDrive(dfw.joysticklv() ,dfw.joystickrv());
  arcadeDrive(dfw.joysticklv(),dfw.joystickrh());

  //ARM
  if(dfw.down() == 0){
    armSetpoint = kArmBottomSetpoint;
  }
  else if(dfw.right() == 0){
    armSetpoint = kArmMiddleSetpoint;
  }
  else if(dfw.up() == 0){
    armSetpoint = kArmTopSetpoint;
  }
  setArm(armSetpoint);

  //INTAKE
  if(dfw.one() == 0){
    setIntakeIn();
  }
  else if(dfw.two() == 0){
    setIntakeOut();
  }
  else if(dfw.three() == 0){
    setIntakeOff();
  }






}


void autonomousPeriodic(){


}
