#include "robot.h"

double armSetpoint;
Encoder leftEncoder(leftEncoderA, leftEncoderB);
Encoder rightEncoder(rightEncoderA, rightencoderB);

void initRobot(){
  initArm();
  initDrivetrain();
}

//PID constants
double kP_Drive = 0.085; //old .8
double kDriveGoodRange = 1.8;
const double kArmGoodRange = 0.15; //.08
double kP_Arm = 4; //4.5

bool enteringTeleop; // used for setting arm position after auto
void teleopPeriodic(DFW dfw){
  //DRIVE
  //tankDrive(servoToFrc(dfw.joysticklv()) ,servoToFrc(dfw.joystickrv()));
  arcadeDrive(servoToFrc(dfw.joysticklv()),servoToFrc(dfw.joystickrh()));

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

  //ARM
  if(dfw.up() == 0 || dfw.l1() == 0){
    setArm(ManualUp);
    enteringTeleop = false;
  }
  else if(dfw.down() == 0 || dfw.l2() == 0){
    setArm(ManualDown);
    enteringTeleop = false;
  }
  else if (enteringTeleop){ //If enteringTeleop from certain auto mode set arm to catch the eggs
    kP_Arm = 6;
    armSetpoint = 0.73;
    setArm(PID);
  }
  else{
    setArm(Off);
  }
}


int autoState = 0;
//Set auto based on value given by the button
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
}

//Drives to the barn to catch eggs
void justBARN(bool red){
  switch(autoState){
      case 0:
        enteringTeleop = true; //set flag so arm goes to egg catching position after auto
        tankDrive(0,0);
        resetEncoders();
        autoState = 1;
        break;

      case 1:
        driveDistance(-37);  //Driving 37 inches
        if(driveInRange(250)){
          resetEncoders();
          autoState = 2; }
        break;

      case 2:
        //Turning -60 degrees
        if(red){
          turnAngle(60);
        }else{
          turnAngle(-60);}
        if(turnInRange(250)){
          autoState = 3;
          resetEncoders();}
        break;

      case 3:
        kP_Drive = 0.15;
        if(red){
          driveDistance(-16);  //Driving 16 inches
        }else{
          driveDistance(-6);  //Driving 6 inches
        }
        if(driveInRange(250)){
          resetEncoders();
          autoState = 4; }
        break;

      case 4:
        //Turning 60 degrees
        if(red){
          turnAngle(-60);
        }else{
          turnAngle(60);}
        if(turnInRange(250)){
          autoState = 5;
          resetEncoders();}
        break;

  	case 5:
      arcadeDrive(-1, 0);
  		setArm(PID);
  		armSetpoint = 0.66;  //Setting the arm kArmMiddleSetpoint
  		if(digitalRead(barnButton) == 0){
  			autoState = 6;}
  		break;


  	case 6:
  		tankDrive(0,0);
      setArm(ManualUp);
      setIntake(OFF);
      if(getArm() > 0.85){
        autoState = 7; }
  		break;

    case 7:
      tankDrive(0,0);
      //wiggle the arm down for catching eggs
      setArm(Off);
      delay(3000);
      setArm(ManualDown);
      delay(500);
      setArm(Off);
      autoState = 8;
      break;
	} //Done with switch
}

void PENtoPEN(bool red){
  switch(autoState){
case 0:
      enteringTeleop = true; //set flag so arm goes to egg catching position after auto
      tankDrive(0,0);
      resetEncoders();
      autoState = 1;
      break;

    case 1:
      driveDistance(21.5);  //Driving 21.5 inches
      if(driveInRange(250)){
        resetEncoders();
        autoState = 2; }
      break;

    case 2:
      //Turning -88 degrees
      if(red){
        turnAngle(-88);
      }else{
        turnAngle(90);}
      if(turnInRange(250)){
        autoState = 3;
        resetEncoders();}
      break;

    case 3:
    if(red){
      driveDistance(15);
  }else{
      driveDistance(26);  //Driving 26 inches
  }
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
      kP_Drive = 0.15; //set kP for pushing the PEN
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
    driveDistance(-4);  //Driving -4 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 8; }
    break;

  case 8:
    kP_Arm = 5;
    setArm(PID);
    armSetpoint = 0.58;  //Setting the arm kArmMiddleSetpoint
    if(armInRange(250))
      autoState = 9;
    break;

  case 9:
    setArm(PID);
    driveDistance(10);  //Driving 10 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 10; }
    break;

  case 10:
    setArm(Off);
    setIntake(OUT);  //Setting the intake to out
    tankDrive(0,0);
    delay(3200);
    autoState = 11;
    break;
  case 11:
    setArm(PID);
    armSetpoint = 0.7;  //Setting the arm kArmMiddleSetpoint
    setIntake(OFF);
    driveDistance(-6);  //Driving -6 inches
    if(driveInRange(250)){
      resetEncoders();
      autoState = 12; }
    break;
   case 12:

    setArm(Off);
      //Turning -90 degrees
      if(red){
        turnAngle(-90);
      }else{
        turnAngle(90);}
      if(turnInRange(250)){
        autoState = 13;
        resetEncoders();}
      break;
   case 13:
   if(red){
    driveDistance(-44);
   }else{
    driveDistance(-30);
   }
    if(driveInRange(250)){
      resetEncoders();
      autoState = 14; }
    break;
    case 14:

    setArm(Off);
      //Turning -90 degrees
      if(red){
        turnAngle(-90);
      }else{
        turnAngle(90);}
      if(turnInRange(250)){
        autoState = 15;
        resetEncoders();}
      break;
  }
}
void PENtoBARN(bool red){
  switch(autoState){

  	case 0:
      enteringTeleop = true; //set flag so arm goes to egg catching position after auto
      tankDrive(0,0);
  		resetEncoders();
  		autoState = 1;
  		break;

  	case 1:
  		driveDistance(21.5);  //Driving 21.5 inches
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
  		driveDistance(26);  //Driving 26 inches
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
      kP_Drive = 0.15; //set kp for pushing PEN
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
  		driveDistance(-27.5);  //Driving -27.5 inches
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
      driveDistance(- 20);  //Driving - 20 inches
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
      if(getArm() > 0.85){ //.87
        autoState = 15;
      }
      break;
  case 14:
    if(getArm() > 0.85){ //.88
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
  Serial.println("Arm init"); //prints arm init over serial (usb com port)

}

void setArm(armState state) { //arm state selector

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
      armPosition = scaleValues(analogRead(potPort),0,1023,0,1); //scale make PID constants smaller
      error = armSetpoint - armPosition;
      armPower = error * kP_Arm;
      if (armPower > 1) armPower = 1; //limit to witin max output
      if (armPower < -1) armPower = -1; //limit to witin min output
      armGood = abs(error) < kArmGoodRange; //see if within range
      if (armGood){
        armPower = 0;
      }
      break;
    }
    leftArm.write(frcToServo(armPower));
    rightArm.write(frcToServo(-armPower));
}

double getArm() { //returns scaled arm pot value
  return scaleValues(analogRead(potPort),0,1023,0,1);
}

bool armInRange(){ //returns if arm is in range
  return armGood;
}
bool armInRange(int delay){ //returns if arm has been in range for "delay"
  return booleanDelay(armGood, delay);
}

void setIntake(intakeState state){ //intake state selector
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

//throttle is forwards/backwards and turn is right/left
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
  return (getLeftEncoder() - getRightEncoder()) / 0.26;
  //scaling factor calculated by turning robot 90deg and measuring encoder reading
}

//AUTO DRIVE FUNCTIONS
double drivePower;
bool driveGood = false;
double minDrivePower = .13;

void driveDistance(double distance){ //P to a certain encoder distance
  double driveError = distance - getAverageEncoder();
  drivePower = driveError * kP_Drive;
  driveGood = abs(driveError) < kDriveGoodRange;
  if (drivePower > 1) drivePower = 1;
  if (drivePower < -1) drivePower = -1;
    else if (!driveGood && abs(drivePower) < minDrivePower){
    drivePower = minDrivePower*drivePower/abs(drivePower);
  }
  if (driveGood || digitalRead(barnButton) == 0){
    drivePower = 0;
  }
  arcadeDrive(drivePower, 0);
  //Serial.println(digitalRead(barnButton));
  //Serial.println(drivePower);
}

void driveDistance(double distance, double maxSpeed){ //P to a cetain encoder distance limiting the max power to "maxSpeed"
  double driveError = distance - getAverageEncoder();
  drivePower = driveError * kP_Drive;
  driveGood = abs(driveError) < kDriveGoodRange;
  if (drivePower > maxSpeed) maxSpeed = 1;
  if (drivePower < -maxSpeed) maxSpeed = -1;
  else if (!driveGood && abs(drivePower) < minDrivePower){
    drivePower = minDrivePower*drivePower/abs(drivePower);
  }
  if (driveGood || digitalRead(barnButton) == 0){
    drivePower = 0;
  }
  arcadeDrive(drivePower, 0);
  //Serial.println(digitalRead(barnButton));
  //Serial.println(drivePower);
}

bool driveInRange(int delay){ //returns true if drive has been in range for "delay"
  return booleanDelay(driveGood,delay);
}

const double kP_Turn = 0.07;
const double kTurnGoodRange = 1.1;
const double kMinTurnPower = 0.3;

double turnPower;
bool turnGood = false;
void turnAngle(int angle){ //turns to certain angle from the gyro** (calculated through encoders)
  double turnError = angle - getGyro();
  turnPower = turnError * -kP_Turn;
  turnGood = abs(turnError) < kTurnGoodRange;
  if (turnPower > 1) turnPower = 1; //limit power
  else if (turnPower < -1) turnPower = -1;
  else if (!turnGood && abs(turnPower) < kMinTurnPower) turnPower = kMinTurnPower*turnPower/abs(turnPower);

  if (turnGood || digitalRead(barnButton) == 0){
    turnPower = 0;
  }
  arcadeDrive(0, turnPower);
  Serial.println(turnPower);
}

bool turnInRange(int delay){ //returns true if desired turn angle is in range for "delay"
  return booleanDelay(turnGood,delay);
}
