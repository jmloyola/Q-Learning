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

import static java.lang.Thread.sleep;


public class ClienteKhepera
{
    //Factor de aprendizaje
    public static final double BETA = 0.9;
    //Tasa de descuento
    public static final double GAMMA = 0.5;
    //Numero de episodios
    public static final int NUMERO_EPISODIOS = 50000;
    
    static final int CANT_ESTADOS = 256;
    
    static final int CANT_ACCIONES = 16;
    
    static final int ACCION_IZQ_M15_DER_M15 = 0;
    static final int ACCION_IZQ_M15_DER_10 = 1;
    static final int ACCION_IZQ_M15_DER_15 = 2;
    static final int ACCION_IZQ_M15_DER_20 = 3;
    
    static final int ACCION_IZQ_10_DER_M15 = 4;
    static final int ACCION_IZQ_10_DER_10 = 5;
    static final int ACCION_IZQ_10_DER_15 = 6;
    static final int ACCION_IZQ_10_DER_20 = 7;
    
    static final int ACCION_IZQ_15_DER_M15 = 8;
    static final int ACCION_IZQ_15_DER_10 = 9;
    static final int ACCION_IZQ_15_DER_15 = 10;
    static final int ACCION_IZQ_15_DER_20 = 11;
    
    static final int ACCION_IZQ_20_DER_M15 = 12;
    static final int ACCION_IZQ_20_DER_10 = 13;
    static final int ACCION_IZQ_20_DER_15 = 14;
    static final int ACCION_IZQ_20_DER_20 = 15;
    
    // No es una de las acciones que realiza khepera
    static final int ACCION_LEER_SENSORES = 24;   
    //Valor de sensor considerado como posible choque
    static final int VALOR_CHOQUE = 700;
            
    double[][] tabla_q = new double [CANT_ESTADOS][CANT_ACCIONES];
    int iteracion = 1;
    double epsilon = 0.9;
    int auxiliar = 0, auxiliar2 = 0;

    
  // call our constructor to start the program
  public static void main(String[] args)
  {
    new ClienteKhepera();
  }
  
      public void ejecutar( ) {
            

            String testServerName = "localhost";
            int port = 6800;
            Socket socket  = null;
            double cotasuperior = Math.exp(NUMERO_EPISODIOS);
            try
            {
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

                    pos_estado = this.getEstado(socket); /// cambiado

                    while( iteracion != NUMERO_EPISODIOS ) {
                        //socket = openSocket(testServerName, port);

                        //Recuperar estado desde robot
                        //pos_estado = this.getEstado(socket); ///
                        
                        accion = seleccionarAccion( pos_estado );
                        
                        socket = openSocket(testServerName, port);
                        //Ejecuta accion 
                        ejecutarAccion( accion, socket);

                        socket = openSocket(testServerName, port);
                        
                        //Nuevo estado luego de ejecutar la accion
                        nuevo_pos_estado =  this.getEstado(socket);
                        
                        //Calcula la recompensa para el nuevo par estado-accion
                        recompensa = getRecompensa();

                        if (recompensa == -1){
                            System.out.println("Recompesa NEGATIVA!!!!!!");
                        }
                        else{
                            System.out.println("Recompesa......"+recompensa+".........");
                        }
                        
                        q_actual = tabla_q[pos_estado][accion];
                        
                        q_max = getMaxQ(nuevo_pos_estado);

                        // Calculate new Value for Q
                        q_nuevo = q_actual + BETA * ( recompensa + GAMMA * q_max - q_actual );
                        
                        tabla_q[pos_estado][accion] = q_nuevo;

                        // Set state to the new state.
                        pos_estado = nuevo_pos_estado;/// cambiado
                        
                        iteracion++;
                        if (iteracion % 2000 == 0){
                            if(epsilon > 0.2){
                                 epsilon -= 0.1;
                            }
                        }
                        //epsilon = epsilon - (epsilon*(1/(Math.exp(NUMERO_EPISODIOS) - Math.exp(iteracion))));
                        System.out.println("iteracion:: "+ iteracion+" ---------   Epsion ::     "+epsilon);
                }
              
              // close the socket, and we're done
              socket.close();
            }
            catch (Exception e)
            {
              e.printStackTrace();
            }


            
    } // end ejecutar 
        
        
    //Inicializar la matriz de valores Q
     public void inicializarValoresQ(double valorInicial){     
	for (int i = 0 ; i <CANT_ESTADOS; i++) {
	    for (int j=0; j < CANT_ACCIONES ; j++) {
		  tabla_q[i][j]=valorInicial;
        }
	}
    }
    
    //Ejecutar accion en el servidor
     //OJO Esto puede cambiarse dejando dos parametros indicando las velocidades de las ruedas y sacando el switch
    public String ejecutarAccion(int accion, Socket socket){
        String respuesta = "NO MIRAR!!";
        try{
            switch (accion){
                case ACCION_IZQ_M15_DER_M15:{
                    writeToAndReadFromSocket(socket, "2,-15,-15"+"\0");
                    break;
                }
                case ACCION_IZQ_M15_DER_10:{
                    writeToAndReadFromSocket(socket, "2,-15,10"+"\0");
                    break;
                }
                case ACCION_IZQ_M15_DER_15:{
                    writeToAndReadFromSocket(socket, "2,-15,20"+"\0");
                    break;
                }
                case ACCION_IZQ_M15_DER_20:{
                    writeToAndReadFromSocket(socket, "2,-15,30"+"\0");
                    break;
                }
                case ACCION_IZQ_10_DER_M15:{
                    writeToAndReadFromSocket(socket, "2,10,-15"+"\0");
                    break;
                }
                case ACCION_IZQ_10_DER_10:{
                    writeToAndReadFromSocket(socket, "2,10,10"+"\0");
                    break;
                }
                case ACCION_IZQ_10_DER_15:{
                    writeToAndReadFromSocket(socket, "2,10,20"+"\0");
                    break;
                }
                case ACCION_IZQ_10_DER_20:{
                    writeToAndReadFromSocket(socket, "2,10,30"+"\0");
                    break;
                }
                case ACCION_IZQ_15_DER_M15:{
                    writeToAndReadFromSocket(socket, "2,20,-15"+"\0");
                    break;
                }
                case ACCION_IZQ_15_DER_10:{
                    writeToAndReadFromSocket(socket, "2,20,10"+"\0");
                    break;
                }
                case ACCION_IZQ_15_DER_15:{
                    writeToAndReadFromSocket(socket, "2,20,20"+"\0");
                    break;
                }
                case ACCION_IZQ_15_DER_20:{
                    writeToAndReadFromSocket(socket, "2,20,30"+"\0");
                    break;
                }
                case ACCION_IZQ_20_DER_M15:{
                    writeToAndReadFromSocket(socket, "2,30,-15"+"\0");
                    break;
                }
                case ACCION_IZQ_20_DER_10:{
                    writeToAndReadFromSocket(socket, "2,30,10"+"\0");
                    break;
                }
                case ACCION_IZQ_20_DER_15:{
                    writeToAndReadFromSocket(socket, "2,30,20"+"\0");
                    break;
                }
                case ACCION_IZQ_20_DER_20:{
                    writeToAndReadFromSocket(socket, "2,30,30"+"\0");
                    break;
                }
                case ACCION_LEER_SENSORES:{
                    respuesta = writeToAndReadFromSocket(socket, "24"+"\0");
                    break;
                }
                default:{
                    System.out.println("ACCION INVALIDA");
                }
            }
        } catch (Exception ex) {
                Logger.getLogger(ClienteKhepera.class.getName()).log(Level.SEVERE, null, ex);
            }
        return respuesta;
    } 
     
     
    //Recupera la posicion de estado actual
    public int getEstado(Socket socket){
        String valorSensores;
        String[] arr;
        int[] arr_int = new int[8];
        int pos_estado =0;
        auxiliar2 = 0;
        
        //Consultar valor de sensores y control de falta de respuesta
        valorSensores = this.ejecutarAccion(ACCION_LEER_SENSORES, socket);
         System.out.println("Valores Sensores es:: "+valorSensores);
        arr = valorSensores.split(";");
        
        for(int j=0; j<8; j++){
            System.out.println("Arr en "+j+" ES::    "+ arr[j]);
            arr_int[j]= Integer.parseInt(arr[j]);
            auxiliar2 += arr_int[j];
            if(arr_int[j] > VALOR_CHOQUE){
                arr_int[j]=1;
            }
            else{
                arr_int[j]=0;
            }
        }
        //Calcula la posicion de estado en base al valor de los sensores discretizados
        for(int i=0; i<8;i++){
            pos_estado += arr_int[i]*(Math.pow(2.0, i)); 
            
        }

        
        return pos_estado;
    } 
         
    private int seleccionarAccion( int estado ) {
          
        int pos_accion = 0;
        double max_val=tabla_q[estado][0];

        for (int i=1; i< CANT_ACCIONES; i++) {
            if(tabla_q[estado][i] > max_val) {
                max_val=tabla_q[estado][i];
                pos_accion = i;
            }
        }
        
        //Explore
        if ( Math.random() < epsilon ) {
            /*
            if(Math.random() < 0.5){
                pos_accion = (pos_accion + 6 )% CANT_ACCIONES;
            }
            else{
                pos_accion = (pos_accion + 10 )% CANT_ACCIONES;
            } 
            */
            pos_accion = (pos_accion + (int)(Math.random() * 15))  %   CANT_ACCIONES;           
        }

        return pos_accion;
    }
    
    public double getRecompensa(){

        System.out.println("auxiliar 1 : "+ auxiliar + "-----auxiliar2  "+ auxiliar2);

        if(auxiliar  > auxiliar2 + 500){
            auxiliar = auxiliar2;
            System.out.println("Entro sleeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeep");
            try{
                sleep(1000);
            }catch(Exception ex){

            }
            return 1;
        }
        else{
            if((Math.abs(auxiliar - auxiliar2) < 250)|| (auxiliar == 0)){
                auxiliar = auxiliar2;
                return 0;
            }

            auxiliar = auxiliar2;
            return -1;
        }


        // if (pos_estado == 0){ 
        //     if (nuevo_pos_estado == 0){
        //         if((accion== ACCION_IZQ_M15_DER_10)||
        //             (accion== ACCION_IZQ_M15_DER_15)||
        //             (accion== ACCION_IZQ_M15_DER_20)||
        //             (accion== ACCION_IZQ_10_DER_M15)||
        //             (accion== ACCION_IZQ_15_DER_M15)||
        //             (accion== ACCION_IZQ_20_DER_M15))
        //             return -1;
        //         else 
        //             return 0;
        //     }
        //     else{
        //         return -1;
        //     }
        // }
        // else{
        //     if (nuevo_pos_estado == 0){
        //         try{
        //             sleep(50000);
        //         }catch(Exception ex){

        //         }
        //         return 1;
        //     }
        //     else{
        //         return -1;
        //     }
        // }
     //   }
    }
    
     private double getMaxQ(int estado){         
        double max_val=tabla_q[estado][0];

        for (int i=1; i< CANT_ACCIONES; i++) {
            if(tabla_q[estado][i] > max_val) {
                max_val=tabla_q[estado][i];
            }
        }
        return max_val;
     }

  
  
  public ClienteKhepera()
  {
    ejecutar();
  }
  
  private String writeToAndReadFromSocket(Socket socket, String writeTo) throws Exception
  {
    try 
    {
      // write text to the socket
      BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
      bufferedWriter.write(writeTo);
      bufferedWriter.flush();
      
      // read text from the socket
      BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      StringBuilder sb = new StringBuilder();
      String str;
      while ((str = bufferedReader.readLine()) != null)
      {
        System.out.println("STR::"+ str);
        sb.append(str);
      }
      
      // close the reader, and return the results as a String
      bufferedReader.close();
      return sb.toString();
    } 
    catch (IOException e) 
    {
      e.printStackTrace();
      throw e;
    }
  }
  
  /**
   * Open a socket connection to the given server on the given port.
   * This method currently sets the socket timeout value to 10 seconds.
   * (A second version of this method could allow the user to specify this timeout.)
   */
  private Socket openSocket(String server, int port) throws Exception
  {
    Socket socket;
    
    // create a socket with a timeout
    try
    {
      InetAddress inteAddress = InetAddress.getByName(server);
      SocketAddress socketAddress = new InetSocketAddress(inteAddress, port);
  
      // create a socket
      socket = new Socket();
  
      // this method will block no more than timeout ms.
      int timeoutInMs = 10*1000;   // 10 seconds

      socket.connect(socketAddress, timeoutInMs);
      
      return socket;
    } 
    catch (SocketTimeoutException ste) 
    {
      System.err.println("Timed out waiting for the socket.");
      ste.printStackTrace();
      throw ste;
    }
  }
  
}
