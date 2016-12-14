#include "robot.h"

double armSetpoint;
Encoder leftEncoder(leftEncoderA, leftEncoderB);
Encoder rightEncoder(rightEncoderA, rightencoderB);

void initRobot(){
  initArm();
  initDrivetrain();
}

double kP_Drive = 0.08; //old .1
double kDriveGoodRange = 2;
const double kArmGoodRange = 0.15; //.08
double kP_Arm = 4; //4.5

bool enteringTeleop;
void teleopPeriodic(DFW dfw){
  //DRIVE
  //tankDrive(servoToFrc(dfw.joysticklv()) ,servoToFrc(dfw.joystickrv()));
  arcadeDrive(servoToFrc(dfw.joysticklv()),servoToFrc(dfw.joystickrh()));
  //Serial.print(getLeftEncoder());
  //Serial.print(" , ");
  //Serial.println(getRightEncoder());
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

  if(dfw.up() == 0 || dfw.l1() == 0){
    setArm(ManualUp);
    enteringTeleop = false;
  }
  else if(dfw.down() == 0 || dfw.l2() == 0){
    setArm(ManualDown);
    enteringTeleop = false;
  }
  else if (enteringTeleop){
    kP_Arm = 6;
    armSetpoint = 0.73;
    setArm(PID);
  }
  else{
    setArm(Off);
  }
  Serial.println(digitalRead(barnButton));
}

int autoState = 0;
void autonomousPeriodic(bool red, int numAuto){
  switch(numAuto){
    case 1:
      PENtoBARN(red);
      break;
    case 2:
      PENtoPEN(red);
      break;
    case 3:
      justBARN(red);
      break;
    }
  } //Done with Auto

void justBARN(bool red){
  switch(autoState){

	case 0:
    enteringTeleop = true;
    tankDrive(0,0);
		resetEncoders();
		autoState = 1;
		break;

	case 1:
		driveDistance(-30);  //Driving -40 inches
		if(driveInRange(250)){
			resetEncoders();
			autoState = 2; }
		break;

	case 2:
    arcadeDrive(-1, 0);
		setArm(PID);
		armSetpoint = 0.66;  //Setting the arm kArmMiddleSetpoint
		if(digitalRead(barnButton) == 0){
			autoState = 3;}
		break;


	case 3:
		tankDrive(0,0);
    setArm(ManualUp);
    setIntake(OFF);
    if(getArm() > 0.87){
      autoState = 4; }
		break;

  case 4:
    tankDrive(0,0);
    setArm(Off);
    break;


	} //Done with switch

}

void PENtoPEN(bool red){
  switch(autoState){

  case 0:
    enteringTeleop = false;
    tankDrive(0,0);
    resetEncoders();
    autoState = 1;
    break;

  case 1:
    driveDistance(20.5);  //Driving 20 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 2; }
    break;

  case 2:
    //Turning -90 degrees
    if(red){
      turnAngle(-90);
    }else{
      turnAngle(90);}
    if(turnInRange(250)){
      autoState = 3;
      resetEncoders();}
    break;

  case 3:
  if(red){
    driveDistance(16); //12
  } else {
    driveDistance(24);
  }  //Driving 22 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 4; }
    break;

  case 4:
    //Turning 90 degrees
    if(red){
      turnAngle(90);
    }else{
      turnAngle(-90);}
    if(turnInRange(250)){
      autoState = 5;
      resetEncoders();}
    break;

  case 5:
    kP_Drive = 0.12;
    driveDistance(12, 0.5);  //Driving 12 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 6; }
    break;

  case 6:
    setIntake(IN);  //Setting the intake to in
    driveDistance(16, 0.5);  //Driving 12 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 7; }
    break;

  case 7:
    driveDistance(-4);  //Driving -4 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 8; }
    break;

  case 8:
    kP_Arm = 5;
    setArm(PID);
    armSetpoint = 0.7;  //Setting the arm kArmMiddleSetpoint
    if(armInRange(250))
      autoState = 9;
    break;

  case 9:
    setArm(PID);
    driveDistance(10);  //Driving 8 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 10; }
    break;

  case 10:
    setArm(Off);
    setIntake(OUT);  //Setting the intake to out
    tankDrive(0,0);
    break;

  } //Done with switch
}
void PENtoBARN(bool red){
  switch(autoState){

  	case 0:
      enteringTeleop = true;
      tankDrive(0,0);
  		resetEncoders();
  		autoState = 1;
  		break;

  	case 1:
  		driveDistance(20.5);  //Driving 20 inches
  		if(driveInRange(250)){
  			resetEncoders();
  			autoState = 2; }
  		break;

  	case 2:
  		//Turning -90 degrees
  		if(red){
  			turnAngle(-90);
  		}else{
  			turnAngle(90);}
  		if(turnInRange(250)){
  			autoState = 3;
  			resetEncoders();}
  		break;

  	case 3:
  		driveDistance(24);  //Driving 24 inches
  		if(driveInRange(250)){
  			resetEncoders();
  			autoState = 4; }
  		break;

  	case 4:
  		//Turning 90 degrees
  		if(red){
  			turnAngle(90);
  		}else{
  			turnAngle(-90);}
  		if(turnInRange(250)){
  			autoState = 5;
  			resetEncoders();}
  		break;

  	case 5:
      kP_Drive = 0.15;
      kDriveGoodRange = 2.5;
      driveDistance(12);  //Driving 12 inches
  		if(driveInRange(250)){
  			resetEncoders();
  			autoState = 6; }
  		break;

  	case 6:
      setIntake(IN);  //Setting the intake to in
  		driveDistance(16);  //Driving 16 inches
  		if(driveInRange(250)){
  			resetEncoders();
  			autoState = 7; }
  		break;

  	case 7:
      kP_Drive = 0.08;
      driveDistance(-4);  //Driving -5 inches
  		if(driveInRange(250)){
  			resetEncoders();
  			autoState = 8; }
  		break;

  	case 8:
  		//Turning -90 degrees
      setIntake(OFF);
      if(red){
  			turnAngle(-90);
  		}else{
  			turnAngle(90);}
  		if(turnInRange(250)){
  			autoState = 9;
  			resetEncoders();}
  		break;

  	case 9:
  		driveDistance(-28.5);  //Driving -28 inches
  		if(driveInRange(250)){
  			resetEncoders();
  			autoState = 10; }
  		break;

  	case 10:
  		//Turning -90 degrees
  		if(red){
  			turnAngle(-90);
  		}else{
  			turnAngle(90);}
  		if(turnInRange(250)){
  			autoState = 12;
  			resetEncoders();}
  		break;

  	case 11:
      armSetpoint = 0.66;
      setArm(PID);
      driveDistance(- 20);  //Driving - 25 inches
  		if(driveInRange(250) || digitalRead(barnButton) == 0){
  			resetEncoders();
  			autoState = 12; }
  		break;

  	case 12:
      armSetpoint = 0.66;
      setArm(PID);
      arcadeDrive(-1, 0);
      if(digitalRead(barnButton) == 0){
        autoState = 13;
      }
      break;
  case 13:
      arcadeDrive(0, 0);
      setArm(ManualUp);
      if(getArm() > 0.87){ //.87
        autoState = 15;
      }
      break;
  case 14:
    if(getArm() > 0.88){ //.88
    setArm(ManualDown);
    delay(150);
    }
    else {
      setArm(ManualUp);}

    break;

    case 15:
    setArm(Off);
    break;

  	} //Done with switch
}


///////////////
////  ARM  ////
///////////////
double armPosition, error, armPower;
bool armGood = false;
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
      if (armPower > 1) armPower = 1;
      if (armPower < -1) armPower = -1;
      armGood = abs(error) < kArmGoodRange;
      if (armGood){
        armPower = 0;
      }
      break;
    }
    leftArm.write(frcToServo(armPower));
    rightArm.write(frcToServo(-armPower));
}

double getArm() {
  return scaleValues(analogRead(potPort),0,1023,0,1);
}

bool armInRange(){
  return armGood;
}
bool armInRange(int delay){
  return booleanDelay(armGood, delay);
}

void setIntake(intakeState state){
  switch (state) {

    case IN:
    intakeSpeed = 0;
    break;

    case OUT:
    intakeSpeed = 180;
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
  pinMode(barnButton, INPUT_PULLUP);
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
//returns encoder value in inches
//reading * wheel diameter * pi / ticsPerRevolution
double getLeftEncoder(){
  return (leftEncoder.read() * 2.75 * 3.14) / 360;
}
double getRightEncoder(){
  return  (rightEncoder.read() * 2.75 * 3.14) / 360;
}
double getAverageEncoder(){
  return (getRightEncoder() + getLeftEncoder()) / 2;
}
void resetEncoders(){
  leftEncoder.write(0);
  rightEncoder.write(0);
}
int getGyro(){
  return (getLeftEncoder() - getRightEncoder()) / 0.26; //magic number calculated by turning robot 90deg and measuring encoder reading
}

//AUTO DRIVE FUNCTIONS


double drivePower;
bool driveGood = false;
void driveDistance(double distance){
  double driveError = distance - getAverageEncoder();
  drivePower = driveError * kP_Drive;
  driveGood = abs(driveError) < kDriveGoodRange;
  if (drivePower > 1) drivePower = 1;
  if (drivePower < -1) drivePower = -1;
  if (driveGood || digitalRead(barnButton) == 0){
    drivePower = 0;
  }
  arcadeDrive(drivePower, 0);
  //Serial.println(digitalRead(barnButton));
  //Serial.println(drivePower);
}

void driveDistance(double distance, double maxSpeed){
  double driveError = distance - getAverageEncoder();
  drivePower = driveError * kP_Drive;
  driveGood = abs(driveError) < kDriveGoodRange;
  if (drivePower > maxSpeed) maxSpeed = 1;
  if (drivePower < -maxSpeed) maxSpeed = -1;
  if (driveGood || digitalRead(barnButton) == 0){
    drivePower = 0;
  }
  arcadeDrive(drivePower, 0);
  //Serial.println(digitalRead(barnButton));
  //Serial.println(drivePower);
}




bool driveInRange(int delay){
  return booleanDelay(driveGood,delay);
}

const double kP_Turn = 0.085; //old .08
const double kTurnGoodRange = 2.5;

double turnPower;
bool turnGood = false;
void turnAngle(int angle){
  double turnError = angle - getGyro();
  turnPower = turnError * -kP_Turn;
  turnGood = abs(turnError) < kTurnGoodRange;
  if (turnPower > 1) turnPower = 1;
  if (turnPower < -1) turnPower = -1;
  if (turnGood || digitalRead(barnButton) == 0){
    turnPower = 0;
  }
  arcadeDrive(0, turnPower);
  Serial.println(turnPower);
}

bool turnInRange(int delay){
  return booleanDelay(turnGood,delay);
}
