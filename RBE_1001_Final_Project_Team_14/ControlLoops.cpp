#include "ControlLoops.h"

//ControlLoops::ControlLoops(){}

void ControlLoops::setOutputLimits(double lowLimit, double upLimit){
  this->lowLimit = lowLimit;
  this->upLimit = upLimit;
  limitsSet = true;
}

void ControlLoops::initPID(double Kp, double Ki, double Kd, double Dt){
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
  this->Dt = Dt;
}

void ControlLoops::scaleInput(double lowIn, double highIn, double lowOut, double highOut){
  this->lowIn = lowIn;
  this->highIn = highIn;
  this->lowOut = lowOut;
  this->highOut = highOut;
  valuesScaled = true;
}

double ControlLoops::PID(double setpoint , double actual) {
  if(valuesScaled){
    actual = scale(actual);
  }

  error = setpoint - actual;
  errorDeriv = (error - errorLast) / Dt;
  errorInt += error;
  PIDOutput = (Kp * error + Kd * errorDeriv + Ki * errorInt);
  if(PIDOutput > upLimit && limitsSet){ PIDOutput = upLimit;}
  if(PIDOutput < lowLimit && limitsSet){ PIDOutput = lowLimit;}

  if(enabled){ return PIDOutput;
  }else{ return 0;}

  errorLast = error;
}

void ControlLoops::enablePID(bool enabled){
  this->enabled = enabled;
}

void ControlLoops::setAcceptableRange(double range){
  this->range=range;
}

bool ControlLoops::inRange(){
  return error > -range && error < range;
}

bool ControlLoops::inRange(int delay){
  return booleanDelay(inRange(), delay);
}

bool ControlLoops::booleanDelay(bool latch, int delay){
  if(!latch){
    triggerStart = millis();
    return false;
  } else {
    return millis() - triggerStart > delay;
  }
}

double ControlLoops::scale(double input){
  return (input - lowIn) * (highOut - lowOut) / (highIn - lowIn) + highOut;
}
