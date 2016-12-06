#with open('AutoModes/Test.txt') as f:
#    lines = f.readlines()
fileName = "PENtoBARN"






directory = "AutoModes/" + fileName
robotState = 1
delayTime = 250

printArray = []
lines = [line.rstrip('\n') for line in open(directory +".txt")]

printArray.append(fileName +"(){\n")
printArray.append("int autoState = 0;")
printArray.append("switch(autoState){\n")


def checkIntake():

    if (l[0:9] == "intakeOff"):
        printArray.append("\t\t//Setting the intake to off")
        printArray.append("\t\tsetIntakeOff();")
    elif (l[0:8] == "intakeIn"):
        printArray.append("\t\t//Setting the intake to in")
        printArray.append("\t\tsetIntakeIn();")
    elif (l[0:9] == "intakeOut"):
        printArray.append("\t\t//Setting the intake to out")
        printArray.append("\t\tsetIntakeOut();")

def intakeLine():
    return l[0:9] == "intakeOff" or l[0:8] == "intakeIn" or l[0:9] == "intakeOut"

previousIntakeLine = False

for l in lines:
    if(not previousIntakeLine):
        printArray.append("\tcase " + str(robotState) + ":")

    if (intakeLine()):
        checkIntake()
        previousIntakeLine = True
    else:
        previousIntakeLine = False


    if(l[0:5] == "drive"):  #Drive
        printArray.append("\t\t//Driving " + l[6:] + " inches")
        printArray.append("\t\tdriveDistance(" + l[6:] + ");")
        printArray.append("\t\tif(driveInRange(" + str(delayTime) + "))")
        printArray.append("\t\t\trobotState = " + str(robotState+1) + ";")

    elif(l[0:4] == "turn"):
        printArray.append("\t\t//Turning " + l[5:] + " degrees")
        printArray.append("\t\tturnAngle(" + l[5:] + ");")
        printArray.append("\t\tif(turnInRange(" + str(delayTime) + "))")
        printArray.append("\t\t\trobotState = " + str(robotState + 1) + ";")

    elif(l[0:3] == "arm"):
        printArray.append("\t\t//Setting the arm " + l[4:])
        printArray.append("\t\tsetArm(" + l[4:] + ");")
        printArray.append("\t\tif(armGood(" + str(delayTime) + "))")
        printArray.append("\t\t\trobotState = " + str(robotState + 1) + ";")

    elif(l[0:4] == "stop"):
        printArray.append("\t\ttankDrive(0,0);")
        printArray.append("\t\tsetIntakeOff();")




    if (not intakeLine()):
        printArray.append("\t\tbreak;\n")
        robotState += 1





printArray.append("\t} //Done with switch")
printArray.append("} //Done with Auto")

f = open(directory + ".auto",'w')
for line in printArray:
    f.write(line +"\n") # python will convert \n to os.linesep

f.close() # you can omit in most cases as the destructor will call it



