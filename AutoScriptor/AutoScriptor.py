#VARIABLES TO SET
fileName = "PENtoBARN"   #file name
robotState = 0   #inital state for robot
delayTime = 250  #time that all functions must be in range for before moving on (passed to the booleanDelay function of the PID loops)


directory = "AutoModes/" + fileName  #map directory to AutoModes folder
printArray = []   #create array to fill with statments
previousIntakeLine = False #used for not printing new case when the line is an intake statement
currentEncoderReading = 0

#strip each line of the text file into an array called "lines"
lines = [line.rstrip('\n') for line in open(directory +".txt")]

#add header files to top of auto file
printArray.append("int autoState = " + str(robotState) + ";")
#Takes a boolean of what side of the field the robot is on
printArray.append(fileName +"(bool red){\n")
printArray.append("switch(autoState){\n")

#checking for the states of the intake and the printing out functions
def checkIntake():

    if (l[0:9] == "intakeOff"):
        printArray.append("\t\tsetIntake(OFF);  //Setting the intake to off")
    elif (l[0:8] == "intakeIn"):
        printArray.append("\t\tsetIntake(IN);  //Setting the intake to in")
    elif (l[0:9] == "intakeOut"):
        printArray.append("\t\tsetIntake(OUT);  //Setting the intake to out")

#Checks if the current line is one containing an intake statment
def intakeLine():
    return l[0:9] == "intakeOff" or l[0:8] == "intakeIn" or l[0:9] == "intakeOut"

################
#   Main Loop  #
################
for l in lines:  #loop through each line in text file
    if(not previousIntakeLine): #if the last line wasn't an intake print out a new case
        printArray.append("\tcase " + str(robotState) + ":")

    # check if the current line is an intake
    if (intakeLine()):  #if so print function and set flag
        checkIntake()
        previousIntakeLine = True
    else:  #else reset flag
        previousIntakeLine = False

    #Print out the functions for available commands
    if(l[0:5] == "drive"):  #Drive
        currentEncoderReading += int(l[6:])
        printArray.append("\t\tdriveDistance(" + l[6:] + ");  //Driving " + l[6:] + " inches")
        printArray.append("\t\tif(driveInRange(" + str(delayTime) + ")){")
        printArray.append("\t\t\tresetEncoders();")
        printArray.append("\t\t\tautoState = " + str(robotState+1) + "; }")

    elif(l[0:4] == "turn"): #Turn, adds a case to determine which side of the field you're on and inverts angle if on blue
        printArray.append("\t\t//Turning " + l[5:] + " degrees")
        printArray.append("\t\tif(red){")
        printArray.append("\t\t\tturnAngle(" + l[5:] + ");")
        printArray.append("\t\t}else{")
        printArray.append("\t\t\tturnAngle(" + str(int(l[5:]) * -1) + ");}")
        printArray.append("\t\tif(turnInRange(" + str(delayTime) + ")){")
        printArray.append("\t\t\tautoState = " + str(robotState + 1) + ";")
        printArray.append("\t\t\tresetEncoders();}")

    elif(l[0:3] == "arm"):  #Sets the arm to the specified variable
        printArray.append("\t\tsetArm(PID);")
        printArray.append("\t\tarmSetpoint = " + l[4:] + ";  //Setting the arm " + l[4:])
        printArray.append("\t\tif(armInRange(" + str(delayTime) + "))")
        printArray.append("\t\t\tautoState = " + str(robotState + 1) + ";")

    elif(l[0:4] == "stop"):  #Stops the motors and stays in this state
        printArray.append("\t\ttankDrive(0,0);")
        printArray.append("\t\tsetIntake(OFF);")

    elif(l[0:4] == "init"):  #resets the encoders and moves to next case
        printArray.append("\t\ttankDrive(0,0);")
        printArray.append("\t\tresetEncoders();")
        printArray.append("\t\tautoState = " + str(robotState + 1) + ";")

    elif(not intakeLine()): #If it's not a recognized line print that there is an error
        print("ERROR, LINE NOT VALID")

    if (not intakeLine()):  #if it's a normal line add break and increment to next case
        printArray.append("\t\tbreak;\n")
        robotState += 1

#End of main loop

#print closing braces
printArray.append("\t} //Done with switch")
printArray.append("} //Done with Auto")

#open a file to write to named the same as your text file but with ".auto" as a file extension
f = open(directory + ".auto",'w')
#loop through statements and write to file
for line in printArray:
    f.write(line +"\n") # python will convert \n to os.linesep

f.close() #close file