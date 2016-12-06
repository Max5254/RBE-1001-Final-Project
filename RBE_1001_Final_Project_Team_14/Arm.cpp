#include "Arm.h"


ControlLoops armPID;
int mPotPort;

void initArm(int leftPort, int rightPort, int intakePort, int potPort) {
  leftArm.attach(leftPort);
  rightArm.attach(rightPort);
  intake.attach(intakePort);
  int mPotPort = potPort;
  armPID.setOutputLimits(-1, 1);
  armPID.scaleInput(0,1023,0,150);
  armPID.initPID(1, 0, 0, 20); //Kp,Ki,Kd,Dt
  armPID.setAcceptableRange(0.25); //Acceptable range of sensor
}

void setArm(int angle) {
  armPID.PID(angle, analogRead(mPotPort));
}
double getArm() {
  return armPID.getValue();
}

bool armGood(int delay){
  return armPID.inRange(delay);
}
void setIntakeIn() {
  intake.write(180);
}
void setIntakeOut() {
  intake.write(0);
}
void setIntakeOff() {
  intake.write(90);
}
