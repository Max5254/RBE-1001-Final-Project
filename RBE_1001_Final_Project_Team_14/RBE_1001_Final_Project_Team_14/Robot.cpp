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

//driveDistance(24);
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
  pinMode(24, INPUT_PULLUP);
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

double getLeftEncoder(){
  //return (leftEncoder.read() * 2.75 * 3.14) / 360;
  return leftEncoder.read();
}
double getRightEncoder(){
  //return  (rightEncoder.read() * 2.75 * 3.14) / 360;
  return rightEncoder.read();
}
double getAverageEncoder(){
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
//bool driveInRange(int delay){
//  return false;
//}

double drivePower;
bool driveGood = false;
void driveDistance(double distance){
  double driveError = distance - getAverageEncoder();
  drivePower = driveError * kP_Drive;
  driveGood = abs(driveError) < kDriveGoodRange;
  if (drivePower > 1) drivePower = 1;
  if (drivePower < -1) drivePower = -1;
  if (driveGood || digitalRead(24) == 0){
    drivePower = 0;
  }
  arcadeDrive(drivePower, 0);
  //Serial.println(digitalRead(24));
  Serial.println(drivePower);
}

bool turnInRange(int delay){
  return false;
}

void turnAngle(int angle){

}
