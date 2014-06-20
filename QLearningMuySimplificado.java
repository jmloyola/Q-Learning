/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package qlearningsimple;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.SocketTimeoutException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Walter
 */
public class QLearningSimple {
    
    //Factor de aprendizaje
    public static final double BETA = 0.9;
    //Tasa de descuento
    public static final double GAMMA = 0.5;
    //Numero de episodios
    public static final int NUMERO_EPISODIOS = 40000;
    
    static final int CANT_ESTADOS = 3; // hacia adelante/ hacia atras
    static final int CANT_ACCIONES = 2; // ir atras / ir adelante    
    static final int ACCION_ADELANTE = 0;
    static final int ACCION_ATRAS = 1;
    
    // No es una de las acciones que realiza khepera
    static final int ACCION_LEER_SENSORES = 9;  
    
    //Valor de sensor considerado como posible choque
    static final int VALOR_CHOQUE = 500;
    static final int VALOR_APROXIMACION = 300;
            
    double[][] tabla_q = new double [CANT_ESTADOS][CANT_ACCIONES];
    
    double epsilon = 0.9;    
    
    int sensor_Front = 0;
    
    float auxiliar = 0, auxiliar2 = 0;    
    float pesos[] = new float[]{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        new QLearningSimple().ejecutar();
    }
    
    public void ejecutar(){
        int iteracion = 1;
        String testServerName = "localhost";
        int port = 6800;
        Socket socket = null;
        try {
            // open a socket
            double q_actual;
            double q_max;
            double q_nuevo;
            //Posicion de estado en la tabla Q
            int pos_estado;
            int nuevo_pos_estado;
            int accion;
            double recompensa;

            inicializarValoresQ(0.0);
            
            socket = openSocket(testServerName, port);
            
            //Recuperar estado desde robot
            pos_estado = this.getEstado(socket);

            while (iteracion != NUMERO_EPISODIOS) {
                accion = seleccionarAccion(pos_estado);

                socket = openSocket(testServerName, port);
                //Ejecuta accion 
                ejecutarAccion(accion, socket);

                socket = openSocket(testServerName, port);

                //Nuevo estado luego de ejecutar la accion
                nuevo_pos_estado = this.getEstado(socket);

                //Calcula la recompensa para el nuevo par estado-accion
                recompensa = getRecompensa(pos_estado, nuevo_pos_estado);


                q_actual = tabla_q[pos_estado][accion];

                q_max = getMaxQ(nuevo_pos_estado);

                // Calculate new Value for Q
                q_nuevo = q_actual + BETA * (recompensa + GAMMA * q_max - q_actual);

                tabla_q[pos_estado][accion] = q_nuevo;
                System.out.println(q_nuevo + " -- " + pos_estado + " -- " + accion);
                
                // Set state to the new state.
                pos_estado = nuevo_pos_estado;
                iteracion++;
                if(iteracion % 2000 == 0){
                    if(epsilon > 0.2){
                         epsilon -= 0.1;
                    }
                }
            }

            // close the socket, and we're done
            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }



    } // end ejecutar 
    
    
    //Inicializar la matriz de valores Q
    public void inicializarValoresQ(double valorInicial) {
        for (int i = 0; i < CANT_ESTADOS; i++) {
            for (int j = 0; j < CANT_ACCIONES; j++) {
                tabla_q[i][j] = valorInicial;
                System.out.println("valorInicial: " + valorInicial + " [" + i + "," + j + "]");
            }
        }
    }

    //Ejecutar accion en el servidor
    //OJO Esto puede cambiarse dejando dos parametros indicando las velocidades de las ruedas y sacando el switch
    public String ejecutarAccion(int accion, Socket socket) {
        String respuesta = "NO MIRAR!!";
        try {
            switch (accion) {
                case ACCION_ADELANTE: {
                    writeToAndReadFromSocket(socket, "2,20,20" + "\0");
                    break;
                }
                case ACCION_ATRAS: {
                    writeToAndReadFromSocket(socket, "2,-20,-20" + "\0");
                    break;
                }
                case ACCION_LEER_SENSORES: {
                    respuesta = writeToAndReadFromSocket(socket, "24" + "\0");
                    break;
                }
                default: {
                    System.out.println("ACCION INVALIDA");
                }
            }
        } catch (Exception ex) {
            Logger.getLogger(QLearningSimple.class.getName()).log(Level.SEVERE, null, ex);
        }
        return respuesta;
    }

    //Recupera la posicion de estado actual
    public int getEstado(Socket socket) {
        String valorSensores;
        String[] arr;
        int[] arr_int = new int[8];
        int pos_estado = 0;

        //Consultar valor de sensores y control de falta de respuesta
        valorSensores = this.ejecutarAccion(ACCION_LEER_SENSORES, socket);
        System.out.println("Valores Sensores es:: " + valorSensores);
        arr = valorSensores.split(";");

        for (int j = 0; j < 8; j++) {
            System.out.println("Arr en " + j + "ES::" + arr[j]);
            arr_int[j] = Integer.parseInt(arr[j]);
        }

        sensor_Front = arr_int[2] + arr_int[3];
        if((arr_int[2] + arr_int[3]) / 2 < VALOR_CHOQUE){
            if((arr_int[6] + arr_int[7])/2 >  VALOR_CHOQUE )
                pos_estado = 0;// NO choca nada
            else
                pos_estado = 1;// Choca atras
        }else{
            pos_estado = 2;//choca adelante
        }
        
        return pos_estado;
    }

    private int seleccionarAccion(int estado) {

        int pos_accion = 0;
        double max_val = tabla_q[estado][0];

        for (int i = 1; i < CANT_ACCIONES; i++) {
            if (tabla_q[estado][i] > max_val) {
                max_val = tabla_q[estado][i];
                pos_accion = i;
            }
        }
        return pos_accion;
    }

    public double getRecompensa(int pos_estado, int nuevo_pos_estado) {
        if(pos_estado == 0 && nuevo_pos_estado == 0){
            return 0;
        }
        else{
            if(pos_estado == 0 && nuevo_pos_estado != 0)
                return -1;
            else{
               if(pos_estado != 0 && nuevo_pos_estado == 0)
                   return 1;
               else
                   return -1;
            }                
        }
    }

    private double getMaxQ(int estado) {
        double max_val = tabla_q[estado][0];
        for (int i = 1; i < CANT_ACCIONES; i++) {
            if (tabla_q[estado][i] > max_val) {
                max_val = tabla_q[estado][i];
            }
        }
        return max_val;
    }

    private String writeToAndReadFromSocket(Socket socket, String writeTo) throws Exception {
        try {
            // write text to the socket
            BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            bufferedWriter.write(writeTo);
            bufferedWriter.flush();

            // read text from the socket
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            StringBuilder sb = new StringBuilder();
            String str;
            while ((str = bufferedReader.readLine()) != null) {
                System.out.println("STR::" + str);
                sb.append(str);
            }

            // close the reader, and return the results as a String
            bufferedReader.close();
            return sb.toString();
        } catch (IOException e) {
            e.printStackTrace();
            throw e;
        }
    }

    /**
     * Open a socket connection to the given server on the given port. This
     * method currently sets the socket timeout value to 10 seconds. (A second
     * version of this method could allow the user to specify this timeout.)
     */
    private Socket openSocket(String server, int port) throws Exception {
        Socket socket;

        // create a socket with a timeout
        try {
            InetAddress inteAddress = InetAddress.getByName(server);
            SocketAddress socketAddress = new InetSocketAddress(inteAddress, port);

            // create a socket
            socket = new Socket();

            // this method will block no more than timeout ms.
            int timeoutInMs = 10 * 1000;   // 10 seconds
            socket.connect(socketAddress, timeoutInMs);

            return socket;
        } catch (SocketTimeoutException ste) {
            System.err.println("Timed out waiting for the socket.");
            ste.printStackTrace();
            throw ste;
        }
    }
    
    
}