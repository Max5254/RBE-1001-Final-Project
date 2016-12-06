#ifndef ControlLoops_h
#define ControlLoops_h

#include "Arduino.h"

class ControlLoops{

private:
  double error, errorDeriv, errorLast, errorInt, PIDOutput, Dt, Kp, Ki, Kd;
  double lowLimit, upLimit, range = 0;
  bool limitsSet = false, enabled = true;
  int triggerStart;

  double inputValue;

  double  lowIn,  highIn,  lowOut,  highOut;
  bool valuesScaled = false;




  bool booleanDelay(bool latch, int delay);
  double scale(double input);



public:
  //ControlLoops();
  void setOutputLimits(double lowLimit, double upLimit);
  void initPID(double Kp, double Ki, double Kd, double Dt);
  double PID(double actual , double setpoint);
  void enablePID(bool enabled);
  void setAcceptableRange(double range);
  bool inRange();
  bool inRange(int delay);
  void scaleInput(double lowIn, double highIn, double lowOut, double highOut);
  double getValue();

};


#endif
