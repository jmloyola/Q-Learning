/****************************************************************
*
*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*  X  Bonaventure Robotics Laboratory   X
*  X                                    X
*  X  serial   Version 1.4              X
*  X  serial.cpp                        X
*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
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
* This version should write to a linux serial port
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
#include "serial.h"


//Public Functions

//Purpose:         Constructor.
//Preconditions:   None.
//PostConditions:  Sets Verbose to verbose status passed, message size,
//                   and default timeout and retry.
Serial::Serial(bool verboseStatus)
{
  VERBOSE = verboseStatus;
  MESSAGE_SIZE = 200;
  DEFAULT_TIMEOUT = 5;
  DEFAULT_RETRY = 3;
}//end Serial()

/**************************************************************************/

//Purpose:         Display all of the debugging messages on the screen,
//Preconditions:   None.
//Postconditions:  VERBOSE set to true.
void Serial::VerboseOn()
{
  VERBOSE = true;
}//end VerboseOn()

/**************************************************************************/

//Purpose:         Stops the displaying of all of the debugging messages on the screen.
//Preconditions:   None.
//Postconditions:  VERBOSE set to false.
void Serial::VerboseOff()
{
  VERBOSE = false;
}//end VerboseOff()

/**************************************************************************/

//Purpose:         Open connection to Khepera on port passed
//                 as a parameter e.g., "ttya".
//Preconditions:   Port name is defined
//Postconditions:  Connection open to given port. Returns true if successful,
//                 false otherwise.
bool Serial::Open(apstring portname)
{
  if (VERBOSE)
    cout << "Serial::Entering Serial Open" << endl;

  int i;
  char buffer[64];
  //if (VERBOSE)
	//cout << "Serial::buffer=";
  // convert apstring to a c string
  for (i=0; i<portname.length(); i++){
    //if (VERBOSE)
	//cout << portname[i];
    buffer[i] = portname[i];
  }
  buffer[i] = '\0';

  SERIAL_ID = open(buffer,O_RDWR|O_EXCL);
  //cerr << "Serial ID: " << SERIAL_ID << endl;
  if (SERIAL_ID == -1)
    {
      if (VERBOSE)
	cout << "Serial::Serial Port Failed to Open --> Serial ID" << endl;
      return false;
    }
  if (!Configure())
    return false;

  if (!Drain())
    return false;
  if (Talk("G,0,0\n") == "")
    {
      if (VERBOSE)
	cout << "Serial::Serial Port Failed to Open --> unable to talk" << endl;
      return false;
    }

  if (VERBOSE)
    cout << "Serial::Serial Port Opened" << endl;

  return true;
}//end Open(apstring)

/**************************************************************************/

//Purpose:         Send command to Khepera and return response.
//Preconditions:   Commands are in the form of a capital letter,
//                 followed by any parameters needed, ending with
//                 a carriage return and line feed.
//Postconditions:  Responses are in the form of a lowercase letter
//                 corresponding to the capital letter command, then
//                 any data it needs to transmit and a carriage return.
apstring Serial::Talk(apstring send, int size)
{
  if (VERBOSE)
    cout << "Serial::Entering Serial Talk" << endl;

  apstring receive = "";
  char cSend[send.length()+1];
  int i;

  for (i=0; i<send.length(); i++)
    cSend[i] = send[i];

  cSend[i] = '\0';
if (VERBOSE)
cout << "El Comando enviado al puerto es "<< cSend << endl;

  int tries = 0;

  do {
    //if write succeeds, read response
    if (write(SERIAL_ID,cSend,send.length()) == send.length())
      {
	  receive = Readline(size);
	  if (VERBOSE){
cout << "Serial::receive="<< receive << endl;
	    cout << "Serial::Talk done" << endl;
}

	  return receive;
      }
    tries ++;
  } while (tries < DEFAULT_RETRY);

  //if reach this point, talk failed

  if (VERBOSE)
    cout << "Serial::Talk Timeout" << endl;

  //perror("Serial::Readline()");
  return receive;
}//end Talk(apstring)

/**************************************************************************/

//Purpose:         Close the serial connection.
//Preconditions:   Connection must be open.
//Postconditions:  Connection is closed to port. Returns true if successful,
//                 false otherwise.
bool Serial::Close()
{
  if (VERBOSE)
    cout << "Serial::Entering Serial Close  "<<SERIAL_ID<< endl;
  close(SERIAL_ID);


  if (VERBOSE)
    cout << "Serial::Serial Line Closed" << endl;

  return true;
}//end Close()

/**************************************************************************/


/*********Private Functions**************/

//Purpose:        Configures the serial port to read and
//                write at 9600 baud, and does a bunch of other
//                stuff too.  Taken from Olivier's khep_serial.c.
//Preconditions:  None.
//Postconditions: Returns true is port is configured correctly,
//                false otherwise.
bool Serial::Configure()
{
  if (VERBOSE)
    cout << "Serial::Entering Serial Configure..."<< endl;

  struct termios term;
  static int speedkey;

  if (tcgetattr(SERIAL_ID, &term) != 0) {
    cerr << "Serial::Configure(), getting attributes" << endl;
    return false;
  }

  //speedkey = B9600;
  speedkey = B57600;
  cfsetispeed(&term, speedkey);
  cfsetospeed(&term, speedkey);
  clear(term.c_iflag,
	IGNBRK|BRKINT|IGNPAR|ISTRIP|ICRNL|INLCR|IXON|IXOFF);
  set(term.c_iflag, 0);
  clear(term.c_oflag,OPOST);
  set(term.c_oflag, 0);
  clear(term.c_cflag,CSIZE|PARENB);
  set(term.c_cflag,CS8|CSTOPB);

  clear(term.c_lflag, ISIG|ICANON|XCASE|ECHO|IEXTEN);
  set(term.c_lflag, 0);
  term.c_cc[VMIN] = 0;
  term.c_cc[VTIME] = DEFAULT_TIMEOUT;

  if(tcsetattr(SERIAL_ID, TCSANOW, &term) != 0) {
    cerr << "Serial::Configure(), setting attributes" << endl;
    return false;
  }

  if (VERBOSE)
    cout << "Serial::Configured" << endl;

  return true;
}//end Configure()

/**************************************************************************/

//Purpose:        Reads a line from the robot.
//Preconditions:  None.
//Postconditions: Returns the size of the line it wrote.
apstring  Serial::Readline(int size)
{
  if (VERBOSE)
    cout << "Serial::Entering Serial Readline..." << endl;

  char buffer[size+1];

  int nrd,rsize=0;
  char c;
 if (VERBOSE)
    cout << "Serial::buffer=";
  do {
    nrd = read(SERIAL_ID, &c, 1);
    if (nrd < 0)
      break;
    buffer[rsize] = c;
	if (VERBOSE)
           cout << c;
    rsize += nrd;
  } while (nrd ==1 && rsize < (size-1) && c != '\n');
if (VERBOSE)
cout << endl;

  buffer[rsize] = '\0';

  if (nrd<0)
    {
      if (VERBOSE)
	cout << "ERROR IN Serial::Readline()" << endl;
      return "";
      //perror("Serial::Readline()");
    }
  else
    {
      if (VERBOSE)
	cout << "Serial::Read line successfully" << endl;
      //apstring response(buffer);
      //return response;
      return buffer;
    }

}//end ReadLine()

/**************************************************************************/

//Purpose:        Drains the input buffer.
//Preconditions:  None.
//Postconditions: Serial line drained. Returns true if successful,
//                 false otherwise.
bool Serial::Drain()
{
  if(VERBOSE)
    cout << "Serial::Entering Serial Drain..." << endl;

  char answer[MESSAGE_SIZE];
  apstring temp;
  int rsize;

  do {
    temp = Readline();
  } while (temp.length()>0);

  if (VERBOSE)
    cout << "Serial::Serial line Drained" << endl;

  return true;
}
