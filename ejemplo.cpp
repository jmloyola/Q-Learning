#include "apstring.h"
#include "kRobot.h"
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


#define SOCKET_PORT 8000


    /**************************************************************************************
     * bool moveBackward();
     * Purpose:        Makes robot move backwards indefinitely at speeds in private fields
     * Preconditions:  Serial communications open.
     * Postconditions: Robot moves backwards indefinitely. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

    r.setWheelSpeed(leftSpeed, rightSpeed);
    r.moveBackward();

    /**************************************************************************************
     * bool moveBackwardDistance(int distance);
     * Purpose:        Makes robot move backward n millimeters.
     *                 Range is 0 < distance <= 670000 mm.
     * Preconditions:  Serial communications open; distance in millimeters.
     * Postconditions: Robot moves backwards n millimeters. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

    r.moveBackwardDistance(leftSpeed);

    /**************************************************************************************
     * bool stop();
     * Purpose:        Stops the movement of the robot.
     * Preconditions:  Serial communications open; robot in motion.
     * Postconditions: Robot stops. Returns true if completed sucessfully, false otherwise.
     **************************************************************************************

    r.stop();

    /**************************************************************************************
     * bool turnLeft(int degree);
     * Purpose:        Makes robot turn left degrees passed as parameter.
     * Preconditions:  0<=degrees<=360; Serial communications open.
     * Postconditions: Robot turns right n degrees. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************
    r.turnLeft(leftSpeed);
    wait(0.2);

    /**************************************************************************************
     * bool turnRight(int degree);
     * Purpose:        Makes robot turn right degrees passed as parameter.
     * Preconditions:  0<=degrees<=360; Serial communications open.
     * Postconditions: Robot turns right n degrees. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

    r.turnRight(leftSpeed);
    wait(0.2);

    /**************************************************************************************
     * int getLeftWheelCounter();
     * Purpose:        Reads left wheel counter.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns counter (+ forward, - reverse)
     **************************************************************************************

    r.getLeftWheelCounter();

    /**************************************************************************************
     * int getRightWheelCounter();
     * Purpose:        Reads right wheel counter.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns counter (+ forward, - reverse)
     **************************************************************************************

    r.getRightWheelCounter();

    /**************************************************************************************
     * int getLeftWheelSpeed();
     * Purpose:        Reads the speed and direction (+/-) of left wheel/motor in mm/sec.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns speed and direction of left wheel
     **************************************************************************************

    r.getLeftWheelSpeed();

    /**************************************************************************************
     * int getRightWheelSpeed();
     * Purpose:        Reads the speed and direction (+/-) of right wheel/motor in mm/sec.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns speed and direction of right wheel
     **************************************************************************************

    r.getRightWheelSpeed();

    /**************************************************************************************
     * bool setWheelSpeed(int leftSpeed, int rightSpeed);
     * Purpose:        Sets the speed and direction (+/-) of wheel/motor in mm/sec.
     *                 Range is 0 < speed <= 1000 mm/sec.
     * Preconditions:  Serial communications open.
     * Postconditions: Private fields are updated. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

     r.setWheelSpeed(leftSpeed,rightSpeed));

    /**************************************************************************************
     * int getLeftWheelAcceleration();
     * Purpose:        Reads the acceleration of left wheel/motor in mm/sec^2.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns acceleration of left wheel
     **************************************************************************************

    r.getLeftWheelAcceleration();

    /**************************************************************************************
     * int getRightWheelAcceleration();
     * Purpose:        Reads the acceleration of right wheel/motor in mm/sec^2.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns acceleration of right wheel
     **************************************************************************************

    r.getRightWheelAcceleration();

    /**************************************************************************************
     * bool setWheelAcceleration(int leftAcc, int rightAcc);
     * Purpose:        Sets the acceleration and direction (+/-) of wheel/motor in mm/sec^2.
     *                 Range is 0 < acceleration <= 396875 mm/sec^2.
     * Preconditions:  Serial communications open.
     * Postconditions: Private fields are updated. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

    r.setWheelAcceleration(leftSpeed,rightSpeed);

    /**************************************************************************************
     * bool readLightSensors();
     * Purpose:        Reads the values of each of eight light sensors.
     * Preconditions:  Serial communication open.
     * Postconditions: Stores values of light sensors in an array, indexed
     *                 0..7, as labeled on Khepera diagram. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

    r.readLightSensors();

    /**************************************************************************************
     * bool writeLightSensors();
     * Purpose:        Displays the values of each of eight light sensors.
     * Preconditions:  Sensors updated; serial communication open.
     * Postconditions: Values of light sensors displayed, indexed
     *                 0..7 as labeled on Khepera diagram. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

    r.writeLightSensors();

    /**************************************************************************************
     * int getLightSensor(int s);
     * Purpose:        Displays the value of requested sensor as numbered
     *                on Khepera diagram.
     * Preconditions:  0<=s<8; Sensors updated; serial communication open.
     * Postconditions: Value returned, 0<=val<1024, larger magnitidue is less light
     **************************************************************************************

    r.getLightSensor(leftSpeed);

    /**************************************************************************************
     * bool readProxSensors();
     * Purpose:        Reads the values of each of eight proximity sensors.
     * Preconditions:  Serial communication open.
     * Postconditions: Stores values of proximity sensors in an array, indexed
     *                 0..7 as labeled on Khepera diagram. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

    r.readProxSensors();

    /**************************************************************************************
     * bool writeProxSensors();
     * Purpose:        Displays the values of the proximity sensors to the screen.
     * Preconditions:  Array contains values.
     * Postconditions: Values of proximity sensors displayed, indexed
     *                 0..7 as labeled on Khepera diagram. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************

    r.writeProxSensors();

    /**************************************************************************************
     * int getProxSensor(int s);
     * Purpose:        Displays the value of requested sensor as numbered
     *                 on Khepera diagram.
     * Preconditions:  0<=s<8; Sensors updated; serial communication open.
     * Postconditions: Value returned, 0<=val<1024, larger magnitidue is closer.
     **************************************************************************************

    r.getProxSensor(leftSpeed);*/


int main()
{

//set serial port for linux
    kRobot r("/dev/ttyUSB0",true);

    r.reset();
    //r.setWheelSpeed(10,10);
//r.moveForward();
/**************************************************************************************
 * bool moveForwardDistance(int distance);
 * Purpose:        Makes robot move forward n millimeters.
 *                 Range is 0 < distance <= 670000 mm.
 * Preconditions:  Serial communications open; distance in millimeters.
 * Postconditions: Robot moves forward n millimeters. Returns true if
 *                 completed sucessfully, false otherwise.
 **************************************************************************************/

    r.moveForwardDistance(10000);


  return 0;
}//main



