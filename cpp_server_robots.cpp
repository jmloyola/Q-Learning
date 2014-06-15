#include "apstring.h"
#include "kRobot.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> /* definition of struct sockaddr_in */
#include <netdb.h>      /* definition of gethostbyname */
#include <arpa/inet.h>  /* definition of inet_ntoa */

#define SOCKET_PORT 6800

//set serial port for linux


int send_to_khepera(kRobot *r, char *command){
  int len=strlen(command),i=2,acum=0,j,leftSpeed,rightSpeed,ret;
  char tmp[15];

  if ((command[1]==',')||(command[1]=='\0')){
    acum=(int)(command[0]-48);
    i=1;
  }
  else acum=(int)((command[0]-48)*10)+(command[1]-48);


  switch(acum){
  case 1: {
    /**************************************************************************************
     * bool reset();
     * Purpose:        Resets the wheel counters to zero. Sets speed and acceleration
     *                 to default settings.
     * Preconditions:  Serial communications open.
     * Postconditions: Counters set to 0; Speed and acceleration are se to default.
     *                 Returns true if completed sucessfully, false otherwise.
     **************************************************************************************/
    if(r->reset()) ret=1;
    else ret=0;

    break;
  }
  case 2: {
    /**************************************************************************************
     * bool moveForward();
     * Purpose:        Makes robot move forward indefinitely at speeds in private fields
     * Preconditions:  Serial communications open.
     * Postconditions: Robot moves forward indefinitely. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    i++;//Apunto al primer parámetro
    for(j=0;command[i]!=',';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);
    i++;//Apunto al segundo parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    rightSpeed=atoi(tmp);
    r->setWheelSpeed(leftSpeed*2, rightSpeed*2);
    if(r->moveForward()) ret=1;
    else ret=0;

    break;
  }
  case 3: {
    /**************************************************************************************
     * bool moveForwardDistance(int distance);
     * Purpose:        Makes robot move forward n millimeters.
     *                 Range is 0 < distance <= 670000 mm.
     * Preconditions:  Serial communications open; distance in millimeters.
     * Postconditions: Robot moves forward n millimeters. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    i++;//Apunto al parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);//Uso leftSpeed para guardar la distancia
    if(r->moveForwardDistance(leftSpeed)) ret=1;
    else ret=0;

    break;
  }
  case 4: {
    /**************************************************************************************
     * bool moveBackward();
     * Purpose:        Makes robot move backwards indefinitely at speeds in private fields
     * Preconditions:  Serial communications open.
     * Postconditions: Robot moves backwards indefinitely. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    i++;//Apunto al primer parámetro
    for(j=0;command[i]!=',';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);
    i++;//Apunto al segundo parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    rightSpeed=atoi(tmp);
    r->setWheelSpeed(leftSpeed, rightSpeed);
    if(r->moveBackward()) ret=1;
    else ret=0;

    break;
  }
  case 5: {
    /**************************************************************************************
     * bool moveBackwardDistance(int distance);
     * Purpose:        Makes robot move backward n millimeters.
     *                 Range is 0 < distance <= 670000 mm.
     * Preconditions:  Serial communications open; distance in millimeters.
     * Postconditions: Robot moves backwards n millimeters. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    i++;//Apunto al parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);//Uso leftSpeed para guardar la distancia
    if(r->moveBackwardDistance(leftSpeed)) ret=1;
    else ret=0;

    break;
  }
  case 6: {
    /**************************************************************************************
     * bool stop();
     * Purpose:        Stops the movement of the robot.
     * Preconditions:  Serial communications open; robot in motion.
     * Postconditions: Robot stops. Returns true if completed sucessfully, false otherwise.
     **************************************************************************************/

    if(r->stop()) ret=1;
    else ret=0;

    break;
  }
  case 7: {
    /**************************************************************************************
     * bool turnLeft(int degree);
     * Purpose:        Makes robot turn left degrees passed as parameter->
     * Preconditions:  0<=degrees<=360; Serial communications open.
     * Postconditions: Robot turns right n degrees. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    i++;//Apunto al parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);//Uso leftSpeed para guardar los grados
    if(r->turnLeft(leftSpeed)) ret=1;
    else ret=0;
    wait(0.2);

    break;
  }
  case 8: {
    /**************************************************************************************
     * bool turnRight(int degree);
     * Purpose:        Makes robot turn right degrees passed as parameter->
     * Preconditions:  0<=degrees<=360; Serial communications open.
     * Postconditions: Robot turns right n degrees. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    i++;//Apunto al parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);//Uso leftSpeed para guardar los grados
    if(r->turnRight(leftSpeed)) ret=1;
    else ret=0;
    wait(0.2);

    break;
  }
  case 9: {
    /**************************************************************************************
     * int getLeftWheelCounter();
     * Purpose:        Reads left wheel counter->
     * Preconditions:  Serial communications open.
     * Postconditions: Returns counter (+ forward, - reverse)
     **************************************************************************************/

    ret=r->getLeftWheelCounter();

    break;
  }
  case 10: {
    /**************************************************************************************
     * int getRightWheelCounter();
     * Purpose:        Reads right wheel counter->
     * Preconditions:  Serial communications open.
     * Postconditions: Returns counter (+ forward, - reverse)
     **************************************************************************************/

    ret=r->getRightWheelCounter();

    break;
  }
  case 11: {
    /**************************************************************************************
     * int getLeftWheelSpeed();
     * Purpose:        Reads the speed and direction (+/-) of left wheel/motor in mm/sec.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns speed and direction of left wheel
     **************************************************************************************/

    ret=r->getLeftWheelSpeed();

    break;
  }
  case 12: {
    /**************************************************************************************
     * int getRightWheelSpeed();
     * Purpose:        Reads the speed and direction (+/-) of right wheel/motor in mm/sec.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns speed and direction of right wheel
     **************************************************************************************/

    ret=r->getRightWheelSpeed();

    break;
  }
  case 13: {
    /**************************************************************************************
     * bool setWheelSpeed(int leftSpeed, int rightSpeed);
     * Purpose:        Sets the speed and direction (+/-) of wheel/motor in mm/sec.
     *                 Range is 0 < speed <= 1000 mm/sec.
     * Preconditions:  Serial communications open.
     * Postconditions: Private fields are updated. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    i++;//Apunto al primer parámetro
    for(j=0;command[i]!=',';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);
    i++;//Apunto al segundo parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    rightSpeed=atoi(tmp);
    if(r->setWheelSpeed(leftSpeed, rightSpeed)) ret=1;
    else ret=0;

    break;
  }
  case 14: {
    /**************************************************************************************
     * int getLeftWheelAcceleration();
     * Purpose:        Reads the acceleration of left wheel/motor in mm/sec^2.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns acceleration of left wheel
     **************************************************************************************/

    ret=r->getLeftWheelAcceleration();

    break;
  }
  case 15: {
    /**************************************************************************************
     * int getRightWheelAcceleration();
     * Purpose:        Reads the acceleration of right wheel/motor in mm/sec^2.
     * Preconditions:  Serial communications open.
     * Postconditions: Returns acceleration of right wheel
     **************************************************************************************/

    ret=r->getRightWheelAcceleration();

    break;
  }
  case 16: {
    /**************************************************************************************
     * bool setWheelAcceleration(int leftAcc, int rightAcc);
     * Purpose:        Sets the acceleration and direction (+/-) of wheel/motor in mm/sec^2.
     *                 Range is 0 < acceleration <= 396875 mm/sec^2.
     * Preconditions:  Serial communications open.
     * Postconditions: Private fields are updated. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    i++;//Apunto al primer parámetro
    for(j=0;command[i]!=',';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);//Uso leftSpeed para guardar la aceleración izquierda
    i++;//Apunto al segundo parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    rightSpeed=atoi(tmp);//Uso rightSpeed para guardar la aceleración derecha
    if(r->setWheelAcceleration(leftSpeed, rightSpeed)) ret=1;
    else ret=0;

    break;
  }
  case 20: {
    /**************************************************************************************
     * bool readProxSensors();
     * Purpose:        Reads the values of each of eight proximity sensors.
     * Preconditions:  Serial communication open.
     * Postconditions: Stores values of proximity sensors in an array, indexed
     *                 0..7 as labeled on Khepera diagram. Returns true if
     *                 completed sucessfully, false otherwise.
     **************************************************************************************/

    if(r->readProxSensors()) ret=1;
    else ret=0;

    break;
  }
  case 22: {
    /**************************************************************************************
     * int getProxSensor(int s);
     * Purpose:        Displays the value of requested sensor as numbered
     *                 on Khepera diagram.
     * Preconditions:  0<=s<8; Sensors updated; serial communication open.
     * Postconditions: Value returned, 0<=val<1024, larger magnitidue is closer->
     **************************************************************************************/

    i++;//Apunto al parámetro
    for(j=0;command[i]!='\0';j++,i++) tmp[j]=command[i];
    tmp[j]='\0';
    leftSpeed=atoi(tmp);//Uso leftSpeed para guardar el número de sensor
    ret = r->getProxSensor(leftSpeed);
    break;
  }

  default: ret = 1;
}

return ret;

}//Fin de int send_to_khepera(kRobot r, char *command)

int main()
{
   kRobot r("/dev/ttyUSB0",true);

  /*Declaraciones de variables*/
  int des_socket, conex_val, nuevo_socket, long_server, long_cliente, pid_servidor;
  struct sockaddr_in dir_serv, dir_cli;
  char buffer[300],num[20];
  int i,len,answer;

  /*Se crea el socket*/
  des_socket = socket(AF_INET, SOCK_STREAM, 0);

  /*Verifica si fue creado*/
  if(des_socket < 0)
    {
      printf("SERVIDOR: NO SE PUEDE ABRIR SOCK_STREAM\n");
      return 1;
    }

  /*En la estructura dir_serv inicializa con cero tantos bytes como indique el segundo argumento*/
  bzero((char *) &dir_serv,sizeof(dir_serv));

  dir_serv.sin_family = AF_INET;
  dir_serv.sin_port = htons((unsigned short)SOCKET_PORT);
  dir_serv.sin_addr.s_addr = htonl(INADDR_ANY);

  /*Asignamos un nombre al nuevo socket*/
  conex_val = bind(des_socket, (struct sockaddr *) &dir_serv, sizeof(dir_serv));

  /*Con este verificamos que la llamada al sistema bind haya asignado al socket una direccion valida
    de manera que el servidor tenga una direccion de retorno donde pueda enviarle las respuestas*/
  if (conex_val < 0)
    {
      printf("SERVIDOR: NO SE PUDO REALIZAR EL ENLACE CON EL PUERTO, DIRECCION DE RETORNO INVALIDA \n");
      return 1;
    }
  /*Con esta llamada al sistema el servidor indica que esta dispuesto a recibir conexion.
    Especifica que a lo sumo la cola de conexiones pendientes puede crecer hasta diez.*/
  listen(des_socket, 10);


  while (true){
    printf("SERVIDOR ESPERANDO CONEXION ...?? \n");
    long_cliente = sizeof(dir_cli);

    /*Con accept s#include "apstring.h"e toma el primer pedido de conexion de la cola y se crea un nuevo socket con las
      las mismas propiedades que des_socket */
    nuevo_socket = accept(des_socket, (struct sockaddr *) &dir_cli, (socklen_t *)&long_cliente);
    if(nuevo_socket < 0)
      {
  printf("SERVIDOR: ERROR AL TRATAR DE RECUPERAR EL PEDIDO DE CONEXION DE LA COLA...\n");
  return 2;
      }


    /*Inicializa buffer*/
    int y;
    // for(y=0;y<300;y++){
    //     buffer[y]='\0';
    // }
    buffer[0]='\0';

    /*Recibe el mensaje del nuevo socket creado con la llamada al sistema accept*/
    recv(nuevo_socket, buffer, 100, 0);

//    printf("SERVER: recibo en buffer %s\n",buffer);

    /*Manda el comando al khepera y recibe la respuesta*/
     if(!strcmp(buffer,"24")){
        answer=send_to_khepera(&r, "20");
        buffer[0]='\0';
        char parametro[5];
        char stri[1];
        for(i=0;i<8;i++){
            sprintf(stri,"%d",i);
            strcpy(parametro, "22,");
            strcat(parametro, stri);
            answer=send_to_khepera(&r, parametro);
            num[0]='\0';
            sprintf(num,"%d",answer);
            strcat(buffer,num);
            strcat(buffer,";");
           
        }
    }else{
        answer=send_to_khepera(&r, buffer);
        /*Inicializa buffer*/
        buffer[0]='\0';
        num[0]='\0';
        sprintf(num,"%d",answer);
        strcat(buffer,num);
    }
    //r.cerrar();
    send(nuevo_socket, buffer, strlen(buffer)+1, 0);
    close(nuevo_socket);

  }

  close(des_socket);

  return 0;
}//main

