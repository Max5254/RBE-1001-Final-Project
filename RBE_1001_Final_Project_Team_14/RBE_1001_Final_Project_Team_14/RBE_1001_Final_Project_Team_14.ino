#include <Arduino.h>

/* This is the RBE 1001 Template as of 7/1/15 This Template
  is designed to run the autonomous and teleop sections of the final
  competition. Write and test your autonomous and teleop code on your
  own, then simply copy paste your code into the appropriate section
  and make sure the time passed to each function corresponds with the
  time in seconds that each set of code should run. IE
  autonomous(20); will run for 20 seconds after the transmitter is
  turned on.The code will not start until the controller is turned on, connected, and start is pressed.
  There are print statements commented out that were used to test */


#include <DFW.h> // DFW include
#include <Servo.h> // servo library
#include "Robot.h"
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(40,41,42,43,44,45);
int autoPotPin = A1;
int autoButtonPin = 22;
bool colorRed;


int ledpindebug = 13; //Wireless controller Debug pin. If lit then there is no communication.

DFW dfw(ledpindebug);  // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds
Servo rightmotor; // Servo object
Servo leftmotor; // Servo object

void setup() {
  Serial.begin(9600); // Serial output begin. Only needed for debug
  dfw.begin(9600, 1); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"
  pinMode(autoButtonPin, INPUT_PULLUP);
  lcd.begin(16, 2);
  while (dfw.start() == 0) { //waits for controller to init
    Serial.println("init");
    dfw.update();
    delay(20);
  }
  // put your setup code here, to run once:
  initRobot();

}

void autonomous(volatile unsigned long time) // function definition
{
  Serial.println("Waiting for start"); //prints Teleop over serial (usb com port)
  while (dfw.start() == 1) { // waits for start button
    setAuto();
    //printPot(0);
    //Serial.println("waiting for start");
    dfw.update();
    delay(20);
  }
  dfw.end();
  Serial.println("Auto"); //prints Teleop over serial (usb com port)
  lcd.clear();
  resetEncoders();
  unsigned long startTime = millis(); // sets start time of autonomous
  time = time * 1000; // modifies milliseconds to seconds
  while ((millis() - startTime <= time) && (dfw.select())) // compares start time to time entered in the autonomous function and
  {
    // The select button can be used to skip the autonomous code.
    // Enter Autonomous User Code Here
    printEncoders(0);
    printGyro(1);
    autonomousPeriodic(colorRed);

    //Serial.println("Autonomous"); //prints Autonomous over serial (usb com port)
    delay(20); //delay to prevent spamming the serial port and to keep servo and dfw libraries happy

  }
  dfw.begin(9600,1);
}



void teleop(unsigned long time) { // function definition
  Serial.println("TeleOp"); //prints Teleop over serial (usb com port)
  lcd.clear();
  unsigned long startTime2 = millis(); // sets start time of teleop
  time = time * 1000; // modifies milliseconds to seconds
  while (millis() - startTime2 <= time) // compares start time to time entered in the teleop function
  {
    //tank drive code next 4 lines
    dfw.update();// Called to update the controllers output. Do not call faster than every 15ms.
    // Enter Teleop User Code Here
    teleopPeriodic(dfw);
    printEncoders(0);
    printPot(1);
    //lcd.print("Teleop");

    delay(20); //delay to prevent spamming the serial port

  }
  exit(0); // exits program
}


void loop() {

  autonomous(20); //time in seconds to run autonomous code
  teleop(20000); //time in seconds that teleop code will run
}


///////////////////////
//PRINTING TO THE LCD//
///////////////////////
void printEncoders(int line){
  lcd.setCursor(0, line);
  lcd.print((String)getLeftEncoder() + " , " + (String)getRightEncoder());
  //lcd.setCursor(0, line + 1);
  //lcd.print((String)getAverageEncoder());
}
void printGyro(int line){
  lcd.setCursor(0, line);
  lcd.print("Gyro: " + (String)getGyro());
}
void printPot(int line){
  lcd.setCursor(0, line);
  lcd.print("Pot: " + (String)getArm());
}


//TODO: make this be able to adapt to different amounts of auto modes
//TODO: make this actually select an auto
int potValue , autoRange, numAuto = 2;
String desiredMode;
void setAuto(){
  //PRINTS THE AUTO MODE
  lcd.setCursor(0, 0);
  lcd.print("Auto: ");
  potValue = analogRead(autoPotPin);
  autoRange = 1023/numAuto;
  if(potValue < autoRange){
    desiredMode = "Do Nothing";
  }
  else if(potValue > autoRange){
    desiredMode = "PEN to PEN";
  }
  lcd.setCursor(6, 0);
  lcd.print(desiredMode);

  //PRINTS THE COLOR
  lcd.setCursor(0, 1);
  lcd.print("Color: ");
  lcd.setCursor(7,  1);
  if(digitalRead(autoButtonPin)){
    lcd.print("RED ");
    colorRed = true;
  } else {
    colorRed = false;
    lcd.print("BLUE");
  }
}
