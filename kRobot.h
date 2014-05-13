/**************************************************************************************
*
*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*  X  Bonaventure Robotics Laboratory   X
*  X                                    X
*  X  kRobot   Version 3.2              X
*  X  kRobot.h                          X
*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*
*  Defines the interface for the Khepera robot.
*  Copyright (C) 2001, 2002, 2004 Robert M. Harlan
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; version 2 of the License,
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You may obtain a copy of the GNU General Public License by writing to
*   the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
*                                       Boston, MA  02111-1307  USA
*
*  Developed by the Computer Science Departmetn, St. Bonaventure University.
*  Contributors:  Dr. Robert Harlan, Shelley McClarigan, Catherine Mellon,
*                 Brian C. Zimmel, and Josh Goodberry
*  Contact: rharlan@cs.sbu.edu
*           http://web.sbu.edu/cs/roboticsLab
*
*
*  Rev 1.0: Shelley McClarigan, Robert Harlan
*
*     This is the original version of kRobot.  The intention of the kRobot
*     class is to enable students with very little programming experience
*     to write and test code on the Khepera robot.  All low level code has
*     been hidden from the programmer.  The kRobot class allows the programmer
*     to use the effectors and sensors of the robot in numerous ways. (5/25/00)
*
*  Rev 1.1: Catherine Mellon and Brian Zimmel
*
*     Three functions have been added which were needed for navigation.
*          MoveForwardUnit() - moves robot 80mm forward
*          TurnSharpLeft()   - turns robot left 90 degrees in same position
*          TurnSharpRight()  - turns robot right 90 degrees in same position
*     Two functions have been removed: TurnLeftCorner() and TurnRightCorner().
*     These functions were not needed with the addition of the new turn functions.
*     Documentation was also added to the code.  (1/9/01)
*
*  Rev 2.0 : Robert Harlan, Catherine Mellon, and Brian C. Zimmel
*
*     After the first version was complete, the class was restructured to be
*     more user friendly.  Some changes included allowing the user to specify
*     a certain distance or speed for the robot to move.  The units for the two
*     fields are in millimeters and millimeters per second, respectively.  The
*     user is also able to turn the robot a specified degree.  Previous manipulation
*     of robot functions were eliminated to make more information about the robot
*     obtainable to the user, including wheel counters and wheel speeds.  Other
*     minor changes were made to the class as well. (1/26/01)
*
*  Rev 2.1 : Robert Harlan, Catherine Mellon, and Brian C. Zimmel
*
*     The user is now able to specify speed for any movement, including moving a certain
*     distance and turning, which was not an option in the past.  The user is able to set
*     the robots acceleration rate as well. (1/30/01)
*
*  Rev 2.2 : Brian C. Zimmel
*
*     All known bugs involving wheel counters have been fixed. (2/5/01)
*
*  Rev 3.0 : Brian C. Zimmel and Catherine Mellon
*
*     Inheritance was added to the kRobot class to allow additional turrets to be used.
*     kRobot was made a base class and all turrets that use the robots must be a subclass
*     of kRobot.  The only function they are permitted to use, excluding the public functions,
*     is the function to send commands to the robot. Subclasses are also allowed to use
*     verbose field for debugging. The intent of this design is to give the option of other
*     turrets to expand off kRobot or other turrets themselves, all using kRobot to send commands.
*     The function to change the port name was moved out of the private as well to allow subclasses
*     to change the port name if needed.  (4/1/01)
*
*  Rev 3.1 : Brian C. Zimmel and Josh Goodberry
*
*     All functions must now return a value.  If no value was returned previously,
*     it must return a Boolean value.  If the function performs properly, it will
*     return a true value, false otherwise.  (5/22/01)
*
*  Rev 3.2 :  Robert Harlan
*     Serial.VERBOSE (flag for tracing Serial class methods) and kRobot.verbose (flag for
*     tracing kRobot class methods)repaired. kRobot.VerboseOff() sets both to false,
*     kRobot.veboseOn() sets both to true.Default parameter sets both to false when a
*     kRobot object is created. If true is passed verbose is set to true and all
*     communication can be seen. (4/15/04)
*
*     Stop has been modified so that neither wheel speeds or wheel acceleration are
*     modified. Setting them to 0 threw an error in turns unless reset was called, (5/20/04)
*
*
*  Date Created: 3/5/00
*  Last Update: 5/20/04
*
**************************************************************************************/

/**************************************************************************************
* La compilacion condicional permite que el programa controle la ejecucion de las
* directivas del preprocesador y la compilacion del codigo del programa. Cada una de
* las directivas condicionales del preprocesador se evalua a una expresion entera
* constante que determinara si el codigo se compila. Ver el siguiente ejemplo:
* #if !defined(NULL)
*    #define NULL 0
* #endif
* Estas directivas determinan si la constante simbolica NULL ya esta definida, sino
* la define. #ifndef es la abraviacion de #if !defined(<constante>).
**************************************************************************************/
#ifndef kRobot_H
#define kRobot_H

#include <iostream>
#include <stdlib.h>
//#include <strstream> It is deprecated use <sstream> instead
#include <sstream>
#include <time.h>
#include "apstring.h"
#include "serial.h"

/**************************************************************************************
* La instruccion "using" nos permite utilizar la version corta de cada nombre del
* programa de la biblioteca estandar de C++ o de cualquier espacio de nombres
* especificado por el programador, por ejemplo cout en vez de std::cout.
* Cada "namespace" define un alcance en donde se colocan los indentificadores y
* variables globales.
**************************************************************************************/
using namespace std;

// SerCom constants for basic commands
const apstring STOP      = "D,0,0\n";//Setea la velocidad de los dos motores en cero
const apstring RESET     = "G,0,0\n";//Pone los contadores de posicion de cada motor en cero
const apstring READPOS   = "H\n";//Lee los contadores de posicion de cada motor
const apstring READSPEED = "E\n";//Lee la velocidad instantanea de ambos motores


class kRobot
{
/**************************************************************************************
* DATOS MIEMBROS Y METODOS PROTEGIDOS
**************************************************************************************/
protected:

//display debugging messages
 bool verbose;

/**************************************************************************************
* Purpose:         Send command to Khepera and return response
* Preconditions:   Commands are in the form of a capital letter,
*                  followed by any parameters needed, ending with
*                  a carriage return and line feed.
* Postconditions:  Responses are in the form of a lowercase letter
*                  corresponding to the capital letter command, then
*                  any data it needs to transmit and a carriage return.
**************************************************************************************/
apstring send(apstring command);

/**************************************************************************************
* DATOS MIEMBROS Y METODOS PUBLICOS
**************************************************************************************/
public:

/**************************************************************************************
* Purpose:        Constructor permits setting of serial port and verbose status
* Preconditions:  newPortName is valid serial port if given
* Postconditions: verbose set to false unless true is passed as second parameter
*                 (can be changed later with the verboseOn() method)
*                 serial port set to parameter, if none given port set
*                 to "/dev/ttya"; Connection to robot opened.
* To create a kRobot in verbose mode so that initial communication can be seen,
* kRobot r("/dev/ttyS0", true);
**************************************************************************************/
kRobot(apstring newPortName = "/dev/ttyS0", bool verboseStatus = false);


/**************************************************************************************
* Purpose:        Closes connection and deletes serial port.
* Preconditions:  None.
* Postconditions: Connection closed and serial port deleted.
**************************************************************************************/
~kRobot();
int cerrar();

/**************************************************************************************
* Purpose:        Resets the wheel counters to zero. Sets speed and acceleration
*                 to default settings.
* Preconditions:  Serial communications open.
* Postconditions: Counters set to 0; Speed and acceleration are se to default.
*                 Returns true if completed sucessfully, false otherwise.
**************************************************************************************/
bool reset();

/**************************************************************************************
* Purpose:        Informs user if a serial connection has been established.
* Preconditions:  None.
* Postconditions: Returns true if serial connection is open, false otherwise.
**************************************************************************************/
bool isConnected();

/**************************************************************************************
* WHEEL MOVEMENT FUNCTIONS
**************************************************************************************/

/**************************************************************************************
* Purpose:        Makes robot move forward indefinitely at speeds in private fields
* Preconditions:  Serial communications open.
* Postconditions: Robot moves forward indefinitely. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool moveForward();

/**************************************************************************************
* Purpose:        Makes robot move forward n millimeters.
*                 Range is 0 < distance <= 670000 mm.
* Preconditions:  Serial communications open; distance in millimeters.
* Postconditions: Robot moves forward n millimeters. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool moveForwardDistance(int distance);

/**************************************************************************************
* Purpose:        Makes robot move backwards indefinitely at speeds in private fields
* Preconditions:  Serial communications open.
* Postconditions: Robot moves backwards indefinitely. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool moveBackward();

/**************************************************************************************
* Purpose:        Makes robot move backward n millimeters.
*                 Range is 0 < distance <= 670000 mm.
* Preconditions:  Serial communications open; distance in millimeters.
* Postconditions: Robot moves backwards n millimeters. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool moveBackwardDistance(int distance);

/**************************************************************************************
* Purpose:        Stops the movement of the robot.
* Preconditions:  Serial communications open; robot in motion.
* Postconditions: Robot stops. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool stop();

/**************************************************************************************
* Purpose:        Makes robot turn left degrees passed as parameter.
* Preconditions:  0<=degrees<=360; Serial communications open.
* Postconditions: Robot turns right n degrees. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool turnLeft(int degree);

/**************************************************************************************
* Purpose:        Makes robot turn right degrees passed as parameter.
* Preconditions:  0<=degrees<=360; Serial communications open.
* Postconditions: Robot turns right n degrees. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool turnRight(int degree);

/**************************************************************************************
* WHEEL COUNTER FUNCTIONS
**************************************************************************************/

/**************************************************************************************
* Purpose:        Reads left wheel counter.
* Preconditions:  Serial communications open.
* Postconditions: Returns counter (+ forward, - reverse)
**************************************************************************************/
int getLeftWheelCounter();

/**************************************************************************************
* Purpose:        Reads right wheel counter.
* Preconditions:  Serial communications open.
* Postconditions: Returns counter (+ forward, - reverse)
**************************************************************************************/
int getRightWheelCounter();

/**************************************************************************************
* Purpose:        Reads the speed and direction (+/-) of left wheel/motor in mm/sec.
* Preconditions:  Serial communications open.
* Postconditions: Returns speed and direction of left wheel
**************************************************************************************/
int getLeftWheelSpeed();

/**************************************************************************************
* Purpose:        Reads the speed and direction (+/-) of left wheel/motor in mm/sec.
* Preconditions:  Serial communications open.
* Postconditions: Returns speed and direction of right wheel
**************************************************************************************/
int getRightWheelSpeed();

/**************************************************************************************
* Purpose:        Sets the speed and direction (+/-) of wheel/motor in mm/sec.
*                 Range is 0 < speed <= 1000 mm/sec.
* Preconditions:  Serial communications open.
* Postconditions: Private fields are updated. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool setWheelSpeed(int leftSpeed, int rightSpeed);

/**************************************************************************************
* Purpose:        Reads the acceleration of left wheel/motor in mm/sec^2.
* Preconditions:  Serial communications open.
* Postconditions: Returns acceleration of left wheel
**************************************************************************************/
int getLeftWheelAcceleration();

/**************************************************************************************
* Purpose:        Reads the acceleration of right wheel/motor in mm/sec^2.
* Preconditions:  Serial communications open.
* Postconditions: Returns acceleration of right wheel
**************************************************************************************/
int getRightWheelAcceleration();

/**************************************************************************************
* Purpose:        Sets the acceleration and direction (+/-) of wheel/motor in mm/sec^2.
*                 Range is 0 < acceleration <= 396875 mm/sec^2.
* Preconditions:  Serial communications open.
* Postconditions: Private fields are updated. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool setWheelAcceleration(int leftAcc, int rightAcc);

/**************************************************************************************
* SENSOR FUNCTIONS
**************************************************************************************/

/**************************************************************************************
* Purpose:        Reads the values of each of eight light sensors.
* Preconditions:  Serial communication open.
* Postconditions: Stores values of light sensors in an array, indexed
*                 0..7, as labeled on Khepera diagram. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool readLightSensors();

/**************************************************************************************
* Purpose:        Displays the values of each of eight light sensors.
* Preconditions:  Sensors updated; serial communication open.
* Postconditions: Values of light sensors displayed, indexed
*                 0..7 as labeled on Khepera diagram. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool writeLightSensors();

/**************************************************************************************
* Purpose:        Displays the value of requested sensor as numbered
*                on Khepera diagram.
* Preconditions:  0<=s<8; Sensors updated; serial communication open.
* Postconditions: Value returned, 0<=val<1024, larger magnitidue is less light
**************************************************************************************/
int getLightSensor(int s);

/**************************************************************************************
* Purpose:        Reads the values of each of eight proximity sensors.
* Preconditions:  Serial communication open.
* Postconditions: Stores values of proximity sensors in an array, indexed
*                 0..7 as labeled on Khepera diagram. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool readProxSensors();

/**************************************************************************************
* Purpose:        Displays the values of the proximity sensors to the screen.
* Preconditions:  Array contains values.
* Postconditions: Values of proximity sensors displayed, indexed
*                 0..7 as labeled on Khepera diagram. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool writeProxSensors();

/**************************************************************************************
* Purpose:        Displays the value of requested sensor as numbered
*                 on Khepera diagram.
* Preconditions:  0<=s<8; Sensors updated; serial communication open.
* Postconditions: Value returned, 0<=val<1024, larger magnitidue is closer.
**************************************************************************************/
int getProxSensor(int s);

/**************************************************************************************
* OTHER FUNCTIONS
**************************************************************************************/

/**************************************************************************************
* Purpose:        Display all of the debugging messages on the screen.By default
*                 kRobot verbose and Serial VERBOSE set to true.
* Preconditions:  *SerialPort created by OpenConnection()
* Postconditions: Serial.VERBOSE and kRobot.verbose set to true. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool verboseOn();

/**************************************************************************************
* Purpose:        Stops the displaying of all the dubugging messages on the screen.
* Preconditions:  *SerialPort created by OpenConnection()
* Postconditions: Serial.VERBOSE and kRobot.verbose set to false. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool verboseOff();

/**************************************************************************************
* Purpose:         Gets the port name of the serial port if different than the default.
*                  Run before OpenConnection().
* Preconditions:   Parameter is initiallized.
* Postconditions:  Stores port name in private field. Returns true if
*                  completed sucessfully, false otherwise.
**************************************************************************************/
bool setPortName(apstring port);

private:

/**************************************************************************************
* Purpose:        Opens the connection to the serial port.
* Preconditions:  Port is defined.
* Postconditions: Connection opened to given port. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool openConnection();

/**************************************************************************************
* Purpose:         Close the serial connection.
* Preconditions:   Connection must be open.
* Postconditions:  Connection is closed to port. Returns true if
*                  completed sucessfully, false otherwise.
**************************************************************************************/
bool closeConnection();

/**************************************************************************************
* Purpose:         Pauses robot so commands are carried out
* Preconditions:   Goal counters for wheels are given
* Postconditions:  Robot is paused until wheel counters are close to goal.
*                  Returns true if  completed sucessfully, false otherwise.
**************************************************************************************/
bool pauseRobot(int leftCounter, int rightCounter);

/**************************************************************************************
* Purpose:         Updates class wheel counters with robot's wheel counters
* Preconditions:   Serial communication open.
* Postconditions:  Private wheel counters data is updated. Returns true if
*                  completed sucessfully, false otherwise.
**************************************************************************************/
bool updateWheelCounters();


//name of port to connect to
apstring portname;

//variable holding state of connection
bool openCon;

//Serial port of object
Serial *SerialPort;

//vectors to track values of sensors and motors
 int wheelSpeeds[2];
 int wheelAccelerations[2];
 int wheelCounters[2];
 int LightSensors[8];
 int ProxSensors[8];

};//Fin de class kRobot

/**************************************************************************************
* NON-MEMBER FUNCTIONS
**************************************************************************************/
/**************************************************************************************
* Purpose:        Gives the robot time to complete an action, if needed.
* Preconditions:  Parameter must be initiallized.
* Postconditions: Pauses program.
**************************************************************************************/
bool wait(double time);

/**************************************************************************************
* Purpose:        Converts the integer num passed into an apstring and returns it.
*                 Used to create a command string where integer values are passed
* Preconditions:  num is an integer
* Postconditions: returns string representation of integer
**************************************************************************************/
apstring intToString(int num);
/**************************************************************************************
* END OF THE NON-MEMBER FUNCTIONS
**************************************************************************************/


#endif
