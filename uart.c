//gcc test.c -o test.run -lwiringPi

//sudo /test.run


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 9600

char statusNodeMcuConnection(int port, char *input, char *output);
int valueAnalogicInput(int port);

int main (void) {
	int serial_port;
	char serial_input[2], serial_output[2], dat;        //[0] => Command; [1] => Address
	int menu_choice = 1, status_nodemcu = 0;	
    
    if ((serial_port = serialOpen ("/dev/ttyS0", BAUD_RATE)) < 0) {
  		fprintf (stderr, "Conexao serial inviavel. Dispositivo serial inapto: %s\n", strerror (errno)) ;
  		return 0x1F;
  	}

  	if (wiringPiSetup () == -1){
 		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
 		return 0x1F;
  	}
  	
  	/*while(1){
    	if(serialDataAvail (serial_port) ) {
            printf("Port: %c \n", serial_port);
            dat = serialGetchar (serial_port);
            printf ("Dat: %c \n", dat) ;
            fflush (stdout) ;
            serialPutchar(serial_port, dat);
        }
	} */
	
	printf("Port: %d \n", serial_port);
    dat = serialGetchar (serial_port);
    printf ("Dat: %c \n", dat) ;
    fflush (stdout) ;
    serialPutchar(serial_port, dat);
	
    serial_output[0] = 0x03;
    serial_output[1] = 0x03;
    printf("\n out 0: %X , %d \n", serial_output[0], serial_output[0]);
    printf("out 1: %X , %d \n", serial_output[1], serial_output[1]);
    printf("port: %d \n", serial_port);
    
    int val = 0;
    //write 0x03 twice; this checks the NodeMCU status   
    if(serialDataAvail (serial_port)){
        serialPutchar(serial_port, serial_output[0]);
        serialPutchar(serial_port, serial_output[1]);
    
        for(int i = 0; i < 2; i++){
            serial_input[i] = serialGetchar(serial_port);
        }
    
        if(serial_input[0] == 0x00  && serial_input[1] == 0x00){
           printf("NodeMCU funcionando normalmente!");
           //return 0x00;
        }
    
        else if(serial_input[0] == 0x1F  && serial_input[1] == 0x1F){
           printf("NodeMCU com problema!");
           //return 0x1F;
        }
    
        else{
            printf("Problema desconhecido detectado!");
            //return 0x1F;
        }
    }
    else{
       printf("%d   Problema desconhecido detectado!!  %d \n \n",val, serial_port);
       //return 0x1F;
    }
    
    //status_nodemcu = statusNodeMcuConnection(serial_port, serial_input, serial_output);
    printf("status: %d , %X \n \n", status_nodemcu, status_nodemcu);

    return 0;
}



char statusNodeMcuConnection(int port, char *input, char *output){
     int val = 0;
    //write 0x03 twice; this checks the NodeMCU status   
    if(serialDataAvail (port)){
        serialPutchar(port, output[0]);
        serialPutchar(port, output[1]);
    
        for(int i = 0; i < 2; i++){
            input[i] = serialGetchar(port);
        }
    
        if(input[0] == 0x00  && input[1] == 0x00){
           printf("NodeMCU funcionando normalmente!");
           return 0x00;
        }
    
        else if(input[0] == 0x1F  && input[1] == 0x1F){
           printf("NodeMCU com problema!");
           return 0x1F;
        }
    
        else{
            printf("Problema desconhecido detectado!");
            return 0x1F;
        }
    }
    else{
       printf("%d   Problema desconhecido detectado!!  %d \n \n",val, port);
       return 0x1F;
    }
}
