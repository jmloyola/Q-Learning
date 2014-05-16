/**************************************************************************************
*
*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*  X  Bonaventure Robotics Laboratory   X
*  X                                    X
*  X  kRobot   Version 3.2              X
*  X  kRobot.cpp                        X
*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*
*  Defines the interface for the Khepera robot.
*
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
* Rev 3.2 :  Robert Harlan
*     Serial.VERBOSE (flag for tracing Serial class methods) and kRobot.verbose (flag for
*     tracing kRobot class methods)repaired. kRobot.VerboseOff() sets both to false,
*     veboseOn() sets both to true. Default setting is true for both. (4/15/04)
*
*     Stop has been modified so that neither wheel speeds or wheel acceleration are
*     modified. Setting them to 0 threw an error in turns unless reset was called, (5/20/04)
*
*  Date Created: 3/5/00
*  Last Update   5/20/04
*
**************************************************************************************/

#include "kRobot.h"

/**************************************************************************************
* METODOS PROTEGIDOS
**************************************************************************************/

/**************************************************************************************
* Purpose:         Send command to Khepera and return response
* Preconditions:   Commands are in the form of a capital letter,
*                  followed by any parameters needed, ending with
*                  a carriage return and line feed.
* Postconditions:  Responses are in the form of a lowercase letter
*                  corresponding to the capital letter command, then
*                  any data it needs to transmit and a carriage return.
**************************************************************************************/
apstring kRobot::send(apstring command)
{
  apstring reply;
  reply = SerialPort->Talk(command);
  return reply;
}//end send(apstring)

/**************************************************************************************
* METODOS PUBLICOS
**************************************************************************************/

/**************************************************************************************
* Purpose:        Constructor permits setting of serial port and berbose status.
* Preconditions:  newPortName is valid serial port if given.
* Postconditions: verbose set to false (changed later  with the verboseOff() method)
*                 serial port set to parameter, if none given port set
*                 to "/dev/ttya"; Connection to robot opened.
*       Note, if you want to see initial communication, create a kRobot object
*       as follows:   kRobot r("/dev/ttya", true);
**************************************************************************************/
kRobot::kRobot(apstring newPortName, bool verboseStatus)
{
  char ch;

  //debugger is initially set on
  //turn off with verboseOff();
  verbose = verboseStatus;

  portname = newPortName;

  wheelCounters[0] = 0;
  wheelCounters[1] = 0;
  wheelSpeeds[0] = 10;
  wheelSpeeds[1] = 10;
  wheelAccelerations[0] = 35;
  wheelAccelerations[1] = 35;

  cout << "\n\nkRobot version 3.2 \nBonaventure Robotics Laboratory" << endl
       << "Press any key to continue: ";
  cin  >> ch;
  if (!openConnection())
    {
      openCon = false;
      cout << "ERROR: Connection failed to open." << endl;
    }
  else
    {
      openCon = true;
      stop();
      reset();
    }

}//end constructor with port parameter

/**************************************************************************************
* Purpose:        Closes connection and deletes serial port.
* Preconditions:  None.
* Postconditions: Connection closed and serial port deleted.
**************************************************************************************/
kRobot::~kRobot()
{
//  closeConnection();
//  delete SerialPort;
}//end destructor
int kRobot::cerrar()
{
  closeConnection();
  delete SerialPort;
  return 0;
}

/**************************************************************************************
* Purpose:        Resets the wheel counters to zero. Sets speed and acceleration
*                 to default settings.
* Preconditions:  Serial communications open.
* Postconditions: Counters set to 0; Speed and acceleration are set to default.
*                 Returns true if completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::reset()
{
  apstring reply = "";
  reply = send(RESET);

  //if the wrong reply is given from the robot return false
  if (reply[0] != 'g')
    {
      return false;
    }
  wheelCounters[0] = 0;
  wheelCounters[1] = 0;
  wheelSpeeds[0] = 10;
  wheelSpeeds[1] = 10;
  wheelAccelerations[0] = 35;
  wheelAccelerations[1] = 35;
    if (!updateWheelCounters())
    {
      if (verbose)
	cout << "Error in reset" << endl;
      return false;
    }
  return true;
}//end reset()

/**************************************************************************************
* Purpose:        Informs user if a serial connection has been established.
* Preconditions:  None.
* Postconditions: Returns true if serial connection is open, false otherwise.
**************************************************************************************/
bool kRobot::isConnected()
{
  return openCon;
}

/********************************************************************************/

/****WHEEL MOVEMENT FUNCTIONS****/

/**************************************************************************************
* Purpose:        Makes robot move forward indefinitely at speeds in private fields
* Preconditions:  Serial communications open.
* Postconditions: Robot moves forward indefinitely. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::moveForward()
{
  apstring strLeftWheel, strRightWheel, strCommand, strReply;

  //convert wheel speed integers to strings for communication
  strLeftWheel = intToString(wheelSpeeds[0]);
  strRightWheel = intToString(wheelSpeeds[1]);

  //send robot
  strCommand = "D," + strLeftWheel + "," + strRightWheel + "\n";

  strReply = send(strCommand);

  if(verbose)
    cout << strReply << endl;

  if (strReply[0] != 'd')
    {
      return false;
    }

  return true;
}//end moveForward()

/**************************************************************************************
* Purpose:        Makes robot move forward n millimeters.
*                 Range is 0 < distance <= 670000 mm.
* Preconditions:  Serial communications open; distance in millimeters.
* Postconditions: Robot moves forward n millimeters. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::moveForwardDistance(int distance)
{
  updateWheelCounters();
  int rotations, leftCounter, rightCounter, comma;
  apstring leftWheel, rightWheel, command, reply;
  apstring wheelCountReply;
  apstring strLeftWheel, strRightWheel;
  apstring strLeftAcc, strRightAcc;


  //convert wheel speed and acceleration integers to strings for communication

  strLeftWheel = intToString(wheelSpeeds[0]);
  strRightWheel = intToString(wheelSpeeds[1]);
  strLeftAcc = intToString(wheelAccelerations[0]);
  strRightAcc = intToString(wheelAccelerations[1]);

  //set wheel speed and acceleration
  command = "J," + strLeftWheel + "," + strLeftAcc + "," + strRightWheel + "," + strRightAcc + "\n";
  reply = send(command);
  if(verbose)
    cout << reply << endl;

  //if do not get back correct response return false
  if (reply[0] != 'j')
    {
      return false;
    }

  //convert distance in mm to wheel rotations; add to current count
  rotations = (distance*1000)/80;
  leftCounter = wheelCounters[0] + rotations;
  rightCounter = wheelCounters[1] + rotations;

  //convert wheel counter integer to strings for communication
  leftWheel = intToString(leftCounter);
  rightWheel = intToString(rightCounter);

  //send robot
  command = "C," + leftWheel + "," + rightWheel + "\n";

  reply = send(command);
  if(verbose)
    cout << reply << endl;

  if (reply[0] != 'c')
    return false;

  //pause program until robot moves given distance, if fails return false
  if (!pauseRobot(leftCounter,rightCounter))
    return false;

  //update wheel counters, if fails return false;
  if (!updateWheelCounters())
    return false;

  return true;

}//end moveForwardDistance(int)

/**************************************************************************************
* Purpose:        Makes robot move backwards indefinitely at speeds in private fields
* Preconditions:  Serial communications open.
* Postconditions: Robot moves backwards indefinitely. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::moveBackward()
{
  apstring strLeftWheel, strRightWheel, strCommand, strReply;

  //convert wheel speed integers to strings for communication
  strLeftWheel = intToString(wheelSpeeds[0]);
  strRightWheel = intToString(wheelSpeeds[1]);

  //send robot
  strCommand = "D,-" + strLeftWheel + ",-" + strRightWheel + "\n";

  strReply = send(strCommand);

  if(verbose)
    cout << strReply << endl;

 if (strReply[0] != 'd')
    {
      return false;
    }

  return true;
}//end moveBackward()

/**************************************************************************************
* Purpose:        Makes robot move backward n millimeters.
*                 Range is 0 < distance <= 670000 mm.
* Preconditions:  Serial communications open; distance in millimeters.
* Postconditions: Robot moves backwards n millimeters. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::moveBackwardDistance(int distance)
{
  updateWheelCounters();
  int rotations, leftCounter, rightCounter, comma;
  apstring leftWheel, rightWheel, command, reply;
  apstring wheelCountReply;
  apstring strLeftWheel, strRightWheel;
  apstring strLeftAcc, strRightAcc;

  //convert wheel speed and acceleration integers to strings for communication
  strLeftWheel = intToString(wheelSpeeds[0]);
  strRightWheel = intToString(wheelSpeeds[1]);
  strLeftAcc = intToString(wheelAccelerations[0]);
  strRightAcc = intToString(wheelAccelerations[1]);

  //set wheel speed and acceleration
  command = "J," + strLeftWheel + "," + strLeftAcc + "," + strRightWheel + "," + strRightAcc + "\n";
  reply = send(command);
  if(verbose)
    cout << reply << endl;

  //if do not get back correct responce return false
  if (reply[0] != 'j')
    {
      return false;
    }

  //convert distance in mm to wheel rotaions; add to current count
  rotations = (distance*1000)/80;
  leftCounter = wheelCounters[0] - rotations;
  rightCounter = wheelCounters[1] - rotations;

  //convert wheel counter integers to strings for communication
  leftWheel = intToString(leftCounter);
  rightWheel = intToString(rightCounter);

  //send robot
  command = "C," + leftWheel + "," + rightWheel + "\n";

  reply = send(command);
  if(verbose)
    cout << reply << endl;

  if (reply[0] != 'c')
    return false;

  //pause program until robot moves given distance, if fails return false
  if (!pauseRobot(leftCounter,rightCounter))
    return false;

  //update wheel counters, if fails return false;
  if (!updateWheelCounters())
    return false;

  return true;

}//end moveBackwardDistance(int)

/**************************************************************************************
* Purpose:        Stops the movement of the robot.
* Preconditions:  Serial communications open; robot in motion.
* Postconditions: Robot stops. Returns true if completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::stop()
{
  //if fails, return false
  if (!updateWheelCounters())
    return false;
  apstring reply = send(STOP);

  //if wrong response is given, return false
  if (reply[0] != 'd')
    return false;

  if (!updateWheelCounters())
    return false;

  return true;
}//end stop()

/**************************************************************************************
* Purpose:        Makes robot turn left degrees passed as parameter.
* Preconditions:  0<=degrees<=360; Serial communications open.
* Postconditions: Robot turns right n degrees. Returns true if
//                completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::turnLeft(int degree)
{
  //if fails return false
  if (!updateWheelCounters())
    return false;

  int counter, leftCounter, rightCounter, comma;
  apstring leftWheel, rightWheel, command, reply;
  apstring wheelCountReply;
  apstring strLeftWheel, strRightWheel;
  apstring strLeftAcc, strRightAcc;

  //convert wheel speed and acceleration integers to strings for communication
  strLeftWheel = intToString(wheelSpeeds[0]);
  strRightWheel = intToString(wheelSpeeds[1]);
  strLeftAcc = intToString(wheelAccelerations[0]);
  strRightAcc = intToString(wheelAccelerations[1]);

  // stop the robot before turning
  reply = send(STOP);
  if (verbose)
  {
	cout << "stop sent from turn left, reply is " << reply << endl;
  }
  if (reply[0] != 'd')
    return false;

  //set wheel speed and acceleration
  command = "J," + strLeftWheel + "," + strLeftAcc + "," + strRightWheel + "," + strRightAcc + "\n";

  reply = send(command);
  if(verbose)
    cout << reply << endl;

  if (reply[0] != 'j')
    return false;


  //convert distance in mm to wheel rotations; add to current count
  counter = (1035*degree)/180;
  //rounds degree up insted of truncating number.
  if (((1035*degree)%180) >= 90)
    counter++;
  leftCounter = wheelCounters[0] - counter;
  rightCounter = wheelCounters[1] + counter;

  //convert wheel counter integers to strings for communication
  leftWheel = intToString(leftCounter);
  rightWheel = intToString(rightCounter);

  //Stop the robot if moving
  send(STOP);

  //send robot
  command = "C," + leftWheel + "," + rightWheel + "\n";

  reply = send(command);
  if(verbose)
    cout << reply << endl;

  if (reply[0] != 'c')
    return false;

  //pause program until robot moves given distance, if fails return false
  if (!pauseRobot(leftCounter,rightCounter))
    return false;

  //update wheel counters, if fails return false;
  if (!updateWheelCounters())
    return false;

  return true;

}//end turnLeft(int)

/**************************************************************************************
* Purpose:        Makes robot turn right degrees passed as parameter.
* Preconditions:  0<=degrees<=360; Serial communications open.
* Postconditions: Robot turns right n degrees.  Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::turnRight(int degree)
{

  //if fails return false;
  if (!updateWheelCounters())
    return false;

  int counter, leftCounter, rightCounter, comma;
  apstring leftWheel, rightWheel, command, reply;
  apstring wheelCountReply;
  apstring strLeftWheel, strRightWheel;
  apstring strLeftAcc, strRightAcc;

  // stop the robot before turning
  reply = send(STOP);
  if (verbose)
  {
	cout << "stop sent, reply is " << reply << endl;
  }


  //convert wheel speed and acceleration integers to strings for communication
  strLeftWheel = intToString(wheelSpeeds[0]);
  strRightWheel = intToString(wheelSpeeds[1]);
  strLeftAcc = intToString(wheelAccelerations[0]);
  strRightAcc = intToString(wheelAccelerations[1]);

  //set wheel speed and acceleration
  command = "J," + strLeftWheel + "," + strLeftAcc + "," + strRightWheel + "," + strRightAcc + "\n";

  reply = send(command);
  if(verbose)
    cout << reply << endl;

  //if reply not correct return false;
  if (reply[0] != 'j')
    return false;

  //convert distance in mm to wheel rotaions; add to current count
  counter = (1035*degree)/180;
  //rounds degree up instead of truncating number.
   if (((1035*degree)%180) >= 90)
    counter++;
  leftCounter = wheelCounters[0] + counter;
  rightCounter = wheelCounters[1] - counter;

  //convert wheel counter integers to strings for communication
  leftWheel = intToString(leftCounter);
  rightWheel = intToString(rightCounter);

  //Stop the robot if moving
  send(STOP);

  //send robot
  command = "C," + leftWheel + "," + rightWheel + "\n";

  reply = send(command);
  if(verbose)
    cout << reply << endl;

  //if reply not correct return false;
  if (reply[0] != 'c')
    return false;

  //pause program until robot moves given distance, if fails return false
  if (!pauseRobot(leftCounter,rightCounter))
    return false;

  //update wheel counters, if fails return false;
  if (!updateWheelCounters())
    return false;

  return true;


}//end turnRight(int)

/********************************************************************************/

/**** WHEEL COUNTER FUNCTIONS ****/
/**************************************************************************************
* Purpose:        Reads left wheel counter.
* Preconditions:  Serial communications open.
* Postconditions: Returns counter (+ forward, - reverse)
**************************************************************************************/
int kRobot::getLeftWheelCounter()
{
  updateWheelCounters();
  return wheelCounters[0];
}//end getLeftWheelCounter()

/**************************************************************************************
* Purpose:        Reads right wheel counter.
* Preconditions:  Serial communications open.
* Postconditions: Returns counter (+ forward, - reverse)
**************************************************************************************/
int kRobot::getRightWheelCounter()
{
  updateWheelCounters();
  return wheelCounters[1];
}//end getRightWheelCounter()

/**************************************************************************************
* Purpose:        Reads the speed and direction (+/-) of left wheel/motor in mm/sec.
* Preconditions:  Serial communications open.
* Postconditions: Returns speed and direction of left wheel
**************************************************************************************/
int kRobot::getLeftWheelSpeed()
{
  return (wheelSpeeds[0]*1000)/127;
}//end getLeftWheelSpeed()

/**************************************************************************************
* Purpose:        Reads the speed and direction (+/-) of right wheel/motor in mm/sec.
* Preconditions:  Serial communications open.
* Postconditions: Returns speed and direction of right wheel
**************************************************************************************/
int kRobot::getRightWheelSpeed()
{
  return (wheelSpeeds[1]*1000)/127;
}//end getRightWheelSpeed()

/**************************************************************************************
* Purpose:        Sets the speed and direction (+/-) of wheel/motor in mm/sec.
*                 Range is 0 < speed <= 1000 mm/sec.
* Preconditions:  Serial communications open.
* Postconditions: Private fields are updated. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::setWheelSpeed(int leftSpeed, int rightSpeed)
{
  leftSpeed = (leftSpeed*127)/1000;
  rightSpeed = (rightSpeed*127)/1000;
  wheelSpeeds[0] = leftSpeed;
  wheelSpeeds[1] = rightSpeed;
  return true;
}//end setWheelSpeed()

/**************************************************************************************
* Purpose:        Reads the acceleration of left wheel/motor in mm/sec^2.
* Preconditions:  Serial communications open.
* Postconditions: Returns acceleration of left wheel
**************************************************************************************/
int kRobot::getLeftWheelAcceleration()
{
  return wheelAccelerations[0]*3125;
}//end getLeftWheelAcceleration()

/**************************************************************************************
* Purpose:        Reads the acceleration of right wheel/motor in mm/sec^2.
* Preconditions:  Serial communications open.
* Postconditions: Returns acceleration of right wheel
**************************************************************************************/
int kRobot::getRightWheelAcceleration()
{
  return wheelAccelerations[1]*3125;
}//end getRightWheelAcceleration()

/**************************************************************************************
* Purpose:        Sets the acceleration and direction (+/-) of wheel/motor in mm/sec^2.
*                 Range is 0 < acceleration <= 396875 mm/sec^2.
* Preconditions:  Serial communications open.
* Postconditions: Private fields are updated. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::setWheelAcceleration(int leftAcc, int rightAcc)
{
  if (leftAcc < 3125)
    leftAcc = 1;
  else
    leftAcc = (leftAcc)/3125;
  if (rightAcc < 3125)
    rightAcc = 1;
  else
    rightAcc = (rightAcc)/3125;
  wheelAccelerations[0] = leftAcc;
  wheelAccelerations[1] = rightAcc;
  return true;
}//end setWheelAcceleration(int,int)

/********************************************************************************/

/**** SENSOR FUNCTIONS ****/
/**************************************************************************************
* Purpose:        Reads the values of each of eight light sensors.
* Preconditions:  Serial communication open.
* Postconditions: Stores values of light sensors in an array, indexed
*                 0..7, as labeled on Khepera diagram.  Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::readLightSensors()
{
  apstring reply = send("O\n");
  int comma;

  if (verbose)
    cout << reply << endl;

  //if correct response not given return false
  if (reply[0] != 'o')
    return false;

  reply = reply.substr(2,reply.length()-1);

  for(int i=0; i<8; i++) {
    comma = reply.find(',');
    LightSensors[i] = atoi(reply.substr(0,comma+1).c_str());
    reply = reply.substr(comma+1, reply.length() - comma + 1);
  }
  LightSensors[7] = atoi(reply.c_str());
  return true;

}//end readLightSensors()

/**************************************************************************************
* Purpose:        Displays the values of each of eight light sensors.
* Preconditions:  Sensors updated; serial communication open.
* Postconditions: Values of light sensors displayed, indexed
*                 0..7 as labeled on Khepera diagram. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::writeLightSensors()
{
  cout << "Light: " << '\t';

  for (int i=0; i<8; i++)
    cout << LightSensors[i] << '\t';
  cout << endl;

  return true;
}//end writeLightSensors()

/**************************************************************************************
* Purpose:        Displays the value of requested sensor as numbered
*                 on Khepera diagram.
* Preconditions:  0<=s<8; Sensors updated; serial communication open.
* Postconditions: Value returned, 0<=val<1024, larger magnitidue is less light
**************************************************************************************/
int kRobot::getLightSensor(int s)
{
  return LightSensors[s];
}//end getLightSensors(int)

/**************************************************************************************
* Purpose:        Reads the values of each of eight proximity sensors.
* Preconditions:  Serial communication open.
* Postconditions: Stores values of proximity sensors in an array, indexed
*                 0..7 as labeled on Khepera diagram. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::readProxSensors()
{
  apstring reply = send("N\n");
  int comma;

  if (verbose)
    cout << " reading sensors... " <<  reply << endl;

  //if correct responce not given return false;
  if (reply[0] != 'n')
    return false;
  reply = reply.substr(2,reply.length()-1);

  for(int i=0; i<8; i++) {
    comma = reply.find(',');

    ProxSensors[i] = atoi(reply.substr(0,comma+1).c_str());
    reply = reply.substr(comma+1, reply.length() - comma + 1);
  }
  ProxSensors[7] = atoi(reply.c_str());
  return true;
}//end readProxSensors()

/**************************************************************************************
* Purpose:        Displays the values of the proximity sensors to the screen.
* Preconditions:  Array contains values.
* Postconditions: Values of proximity sensors displayed, indexed
*                 0..7 as labeled on Khepera diagram. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::writeProxSensors()
{
  cout << "Prox: " << '\t';

  for (int i=0; i<8; i++)
    cout << ProxSensors[i] << '\t';
  cout << endl;

  return true;
}//end writeProxSensors()

/**************************************************************************************
* Purpose:        Displays the value of requested sensor as numbered
*                 on Khepera diagram.
* Preconditions:  0<=s<8; Sensors updated; serial communication open.
* Postconditions: Value returned, 0<=val<1024, larger magnitidue is closer.
**************************************************************************************/
int kRobot::getProxSensor(int s)
{
  return ProxSensors[s];
}//end getProxSensor(int)


/********************************************************************************/

/**** OTHER FUNCTIONS ****/
/**************************************************************************************
* Purpose:        Display all of the debugging messages on the screen.
* Preconditions:  *SerialPort created by OpenConnection()
* Postconditions: Serial.VERBOSE and kRobot.verbose set to true. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::verboseOn()
{
  verbose = true;
  SerialPort->VerboseOn();

  cout << "khepera is in VERBOSE mode" << endl;
  return true;
}//end verboseOn()

/**************************************************************************************
* Purpose:        Stops the displaying of all the dubugging messages on the screen.
* Preconditions:  None.
* Postconditions: Serial.VERBOSE and kRobot.verbose set to false. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::verboseOff()
{
  verbose = false;
  SerialPort->VerboseOff();
  return true;
}//end verboseOff()

/**************************************************************************************
* Purpose:       Gets the port name of the serial port if different than the default.
*                Run before openConnection().
* Precondition:  Parameter is initiallized.
* Postcondition: Stores port name in private field. Returns true if
*                completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::setPortName(apstring port)
{
  if (verbose)
    cout << "Changing connection to robot. Please wait..." << endl;

  //if connection cannot be closed, return false;
  if (!closeConnection())
    return false;
  portname = port;
  //if new connection is not opened return false;

  if (!openConnection())
    return false;

  if (verbose)
    cout << "A new connection has been made on: " << port << endl;
  return true;
}//end getPortName()

/**********************PRIVATE FUNCTIONS***********************/
/**************************************************************************************
* Purpose:       Opens the connection to the serial port.
* Precondition:  Port is defined.
* Postcondition: Connection opened to given port. Returns true if
*                completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::openConnection()
{
  if (verbose)
    cout << "kRobot->Opening connection to khepera..." << endl;

  SerialPort = new Serial(verbose);
  if (!SerialPort->Open(portname.c_str()))
    {
      if (verbose)
	cout << "kRobot->Connection failed to open" << endl;
      return false;
    }

  if (verbose)
    cout << "kRobot->...Connection opened" << endl;
  return true;
}//end openConnection()

/**************************************************************************************
* Purpose:         Close the serial connection to the serial port.
* Preconditions:   Connection must be open.
* Postconditions:  Connection is closed to port. Returns true if
*                  completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::closeConnection()
{
  if (!SerialPort->Close())
    {
      if (verbose)
	cout << "kRobot->Connection failed to close" << endl;
      return false;
    }

  if (verbose)
    cout << "Connection to khepera closed " << endl;
  return true;

}//end closeConnection()

/**************************************************************************************
* Purpose:         Pauses robot so commands can be carried out
* Preconditions:   Goal counters for wheels are given
* Postconditions:  Robot is paused until wheel counters are close to goal
*                  Returns true if completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::pauseRobot(int leftCounter, int rightCounter)
{
  if (!updateWheelCounters())
    {
      if (verbose)
	cout << "Error in pause robot -- could not update wheel counters" << endl;
      return false;
    }

  //keep pausing until (goal-10 < counter < goal+10)
  while (((leftCounter-10 > wheelCounters[0]) || (wheelCounters[0] > leftCounter+10)) ||
	 ((rightCounter-10 > wheelCounters[1]) || (wheelCounters[1] > rightCounter+10)))
  {
    wait(.001);
    if(verbose)
    {
        cout << "left ctr " << leftCounter << " left wheel " <<  wheelCounters[0] << " right ctr "
             << rightCounter << " right wheel " << wheelCounters[1] << endl;
    }//if verbose
      if (!updateWheelCounters())
    {
      if (verbose)
	cout << "Error in puase robot -- error in while loop" << endl;
      return false;
    }
  }
  return true;
}//end pauseRobot(int,int)

/**************************************************************************************
* Purpose:         Updates class wheel counters with robot's wheel counters
* Preconditions:   Serial communication open.
* Postconditions:  Private wheel counters data is updated. Returns true if
*                  completed sucessfully, false otherwise.
**************************************************************************************/
bool kRobot::updateWheelCounters()
{
  apstring wheelCountReply = "";
  wheelCountReply = send("H\n");
  wheelCountReply = send("H\n");
  if (verbose)
  {
	cout << " response on wheel count in updateWheelCount: " << wheelCountReply << endl;
  }

  if (wheelCountReply[0] != 'h')

    return false;

  int comma = 0;

  //Get wheel counters from robot and assign to private data
  wheelCountReply = wheelCountReply.substr(2,wheelCountReply.length()-1);
  comma = wheelCountReply.find(',');
  wheelCounters[0] = atoi(wheelCountReply.substr(0,comma+1).c_str());
  wheelCountReply = wheelCountReply.substr(comma+1, wheelCountReply.length() - comma+1);
  wheelCounters[1] = atoi(wheelCountReply.c_str());
  if (verbose)
  {
	cout << "wheel counters from updateWheelCount: left " << wheelCounters[0]
             << " right " << wheelCounters[1] << endl;
  }
  return true;
}//end updateWheelCounters()

/**************************************************************************************
* Purpose:        Gives the robot time to complete an action, if needed.
* Preconditions:  Parameter must be initiallized.
* Postconditions: Pauses program. Returns true if
*                 completed sucessfully, false otherwise.
**************************************************************************************/
/**********************NON-MEMBER FUNCTIONS***********************/
bool wait(double time){
  long start= clock();

  time = time*1000000;
  time += start;

  while(clock() < time);

  return true;

}// end wait(double)
/**************************************************************************************
* Purpose:       Converts the integer num passed into an apstring and returns it.
*                Used to create a command string where integer values are passed
* Precondition:  num is an integer
* Postcondition: returns string representation of integer
**************************************************************************************/
/*apstring intToString(int num)
{
  apstring strReply;
  sstream strstTmp;

  strstTmp << num;
  getline(strstTmp, strReply);


  return strReply;
}//end intToString(int)

*/
apstring intToString(int num)
{
  char aux[30];
  int i=29,j=0,div=0,rest=0;
  aux[0]='0';
  aux[1]='\0';

  if(num<0) aux[j++]='-';
 div=abs(num);
 //cout << "intToString:: num=" << num << endl;

  while(div!=0){
    rest=div%10;
    div=div/10;
    switch(rest){
      case 0: aux[i]='0';break;
      case 1: aux[i]='1';break;
      case 2: aux[i]='2';break;
      case 3: aux[i]='3';break;
      case 4: aux[i]='4';break;
      case 5: aux[i]='5';break;
      case 6: aux[i]='6';break;
      case 7: aux[i]='7';break;
      case 8: aux[i]='8';break;
      case 9: aux[i]='9';break;
    }
    //if(div<10) break;
    i--;
  }
  i++;
  while(i<30){
    aux[j++]=aux[i++];
  }
  if (j!=0) aux[j]='\0';
apstring strReply(aux);

//cout << "intToString:: aux=" << aux << "mientras que strReply=" << strReply <<endl;


  return strReply;
}//end intToString(int)
