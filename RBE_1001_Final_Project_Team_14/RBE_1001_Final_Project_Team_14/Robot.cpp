#include "robot.h"

double armSetpoint;
Encoder leftEncoder(leftEncoderA, leftEncoderB);
Encoder rightEncoder(rightEncoderA, rightencoderB);

void initRobot(){
  initArm();
  initDrivetrain();
}


void teleopPeriodic(DFW dfw){
  //DRIVE
  //tankDrive(servoToFrc(dfw.joysticklv()) ,servoToFrc(dfw.joystickrv()));
  arcadeDrive(servoToFrc(dfw.joysticklv()),servoToFrc(dfw.joystickrh()));
  Serial.print(getLeftEncoder());
  Serial.print(" , ");
  Serial.println(getRightEncoder());
  //ARM
  /*if(dfw.down() == 0){
    armSetpoint = kArmBottomSetpoint;
  }
  else if(dfw.right() == 0){
    armSetpoint = kArmMiddleSetpoint;
  }
  else if(dfw.up() == 0){
    armSetpoint = kArmTopSetpoint;
  }

  setArm(PID);
*/
  //INTAKE
  if(dfw.three() == 0){
    setIntake(IN);
  }
  else if(dfw.two() == 0){
    setIntake(OUT);
  }
  else if(dfw.four() == 0){
    setIntake(OFF);
  }

  if(dfw.up() == 0){
    setArm(ManualUp);
  }
  else if(dfw.down() == 0){
    setArm(ManualDown);
  }
  else{
    setArm(Off);
  }
}


void autonomousPeriodic(bool colorRed){


}

///////////////
////  ARM  ////
///////////////
double armPosition, error, armPower;
bool armInRange;
int intakeSpeed;

void initArm() {
  leftArm.attach(armLeftPort, 1000, 2000);
  rightArm.attach(armRightPort, 1000, 2000);
  intake.attach(intakePort, 1000, 2000);
  Serial.println("Arm init"); //prints Teleop over serial (usb com port)

}

void setArm(armState state) {

  switch (state) {

    case ManualUp:
      armPower = 1;
      break;

    case ManualDown:
      armPower = -1;
      break;

    case Off:
      armPower = 0;
      break;

    case PID:
      armPosition = scaleValues(analogRead(potPort),0,1023,0,1);
      error = armSetpoint - armPosition;
      armPower = error * kP_Arm;
      armInRange = abs(error) < kArmGoodRange;
      if (armInRange){
        armPower = 0;
      }
      break;
    }
    leftArm.write(frcToServo(armPower));
    rightArm.write(frcToServo(-armPower));
}

double getArm() {
  return armPosition;
}

bool armGood(){
  return armInRange;
}
bool armGood(int delay){
  return booleanDelay(armInRange, delay);
}

void setIntake(intakeState state){
  switch (state) {

    case IN:
    intakeSpeed = 180;
    break;

    case OUT:
    intakeSpeed = 0;
    break;

    case OFF:
    intakeSpeed = 90;
    break;
  }
  intake.write(intakeSpeed);
}


//////////////////
/// DRIVETRAIN ///
//////////////////



void initDrivetrain(){
  leftTopMotor.attach(driveTopLeft, 1000, 2000);
  leftBottomMotor.attach(driveBottomLeft, 1000, 2000);
  rightTopMotor.attach(driveTopRight, 1000, 2000);
  rightBottomMotor.attach(driveBottomRight, 1000, 2000);
  Serial.println("Drivetrain init");
}

void tankDrive(double leftInput, double rightInput){
  leftTopMotor.write(frcToServo(leftInput));
  rightBottomMotor.write(frcToServo(rightInput));
  leftBottomMotor.write(frcToServo(-leftInput));
  rightTopMotor.write(frcToServo(-rightInput));
}

void arcadeDrive(double throttle, double turn){
    tankDrive(throttle - turn , throttle + turn);
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

//AUTO DRIVE FUNCTIONS
bool driveInRange(int delay){
  return false;
}

void driveDistance(double distance){

}

bool turnInRange(int delay){
  return false;
}

void turnAngle(int angle){

}
