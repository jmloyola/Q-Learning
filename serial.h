/**************************************************************** 
* 
*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
*  X  Bonaventure Robotics Labratory    X 
*  X                                    X 
*  X  serial   Version 1.4              X 
*  X  serial.cpp                        X 
*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
* 
*  Copyright (C) 2001, 2002, 2004 Robert M. Harlan 
* 
*   This program is free software; you can redistribute it and/or modify 
*   it under the terms of the GNU General Public License as published by 
*   the Free Software Foundation; version 2 of the License, 
* 
*   This program is distributed in the hope that it will be useful, 
*   but WITHOUT ANY WARRANTY; without even the implied warranty of 
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
*  This version may be the one that writes to a linux serial port 
* 
*    Rev 1.1  Shelley McClarigan 
* 
*         This is a serial port class to handle communication with 
*         a khepera robot and the host computer.  The class can open 
*         and close the connection to the robot through a specified 
*         serial port.  It also includes a function that sends the 
*         commands and receives responses from the robot. 
* 
*         Modified from khep_serial.c written by M. von Holzen and  
*         L. Tettoni and released by Olivier Michel as part of the 
*         code we orignially got with the khepera. 
* 
*    Rev 1.2   Dr. Robert Harlan 
* 
*         Added default arguments to readline() and Talk() to permit 
*         return of lines > 64 as needed by vision turret 
*         Allows buffer lengths to be set by user. (4/22/01) 
* 
*    Rev 1.3  Brian Zimmel, Mike Neel, Josh Goodberry 
* 
*         Added error checking to notify user if a problem has 
*         occured. (5/24/01) 
* 
*    Rev 1.4  Robert Harlan 
*         Parameter added to Serial constructor so that verbose status 
*         set by kRobot (default is false) is passed to Serial object (4/15/04) 
* 
*  Date Created: 2/00 
*  Last Update: 4/15/04 
*****************************************************************/ 
#ifndef serial_H 
#define serial_H 
 
#include <iostream> 
#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <termio.h> 
#include "apstring.h" 
 
using namespace std; 
 
#define clear(var, mask)      var &= (~(mask)) 
#define set(var,mask)         var |= (mask) 
 
class Serial { 
 
  public: 
 
     //Purpose:         Constructor. 
     //Preconditions:   None. 
     //PostConditions:  Sets Verbose to bool value, message size,  
     //                   and default timeout and retry. 
     Serial(bool); 
 
     //Purpose:         Display all of the debugging messages on the screen. 
     //Preconditions:   None. 
     //Postconditions:  VERBOSE set to true. 
     void VerboseOn(); 
 
     //Purpose:         Stops the displaying of the debugging messages on the screen. 
     //Preconditions:   None. 
     //Postconditions:  VERBOSE set to false. 
     void VerboseOff(); 
 
     //Purpose:         Open connection to Khepera on port passed 
     //                 as a parameter e.g., "ttya".  
     //Preconditions:   Port name is defined 
     //Postconditions:  Connection open to given port. Returns true if successful, 
     //                 false otherwise. 
     bool Open(apstring portname); 
 
     //Purpose:         Send command to Khepera and return response. 
     //Preconditions:   Commands are in the form of a capital letter, 
     //                   followed by any parameters needed, ending with 
     //                   a carriage return and line feed. 
     //Postconditions:  Responses are in the form of a lowercase letter  
     //                 corresponding to the capital letter command, then 
     //                 any data it needs to transmit and a carriage return. 
     apstring Talk(apstring send, int size = 64); 
 
     //Purpose:         Close the serial connection. 
     //Preconditions:   Connection must be open. 
     //Postconditions:  Connection is closed to port. Returns true if successful, 
     //                 false otherwise. 
     bool Close(); 
 
 
  private: 
     bool VERBOSE; 
     int MESSAGE_SIZE; 
     int DEFAULT_TIMEOUT; 
     int DEFAULT_RETRY; 
     int SERIAL_ID; 
     apstring Readline(int size = 64); 
     bool Configure(); 
     bool Drain(); 
 
 
}; 
 
#endif 
 
 
 
 
 
