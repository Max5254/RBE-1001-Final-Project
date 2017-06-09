# WPI B16 Team 14's RBE 1001 final project robot code

Video of NEST scoring autonomous working: https://goo.gl/photos/t9ZCUKBD9ZbDSFzN7

Our robot code is broken up into multiple files to ease clarity. The template file is only used to call our autonomous/teleop functions and print values to the LCD screen. We are able to select which autonomous mode we want to run using a button to toggle through the available modes, a jumper to select the color, and an LCD to display the current state. 

To make it easier for us to quickly develop autonomous modes we wrote a python script that parses a plain text file and generates a C++ switch statement which is saved as a .auto file to be copied into our robot code. Each state runs a specific robot feature using the functions in our robot code and moves on to the next state when a certain condition is met. The program also automatically adds statements to reverse turns depending on which side of the field the robot starts on.  An example input and output of this program is included. 

![alt text](http://i.imgur.com/O5fFYRml.jpg)
