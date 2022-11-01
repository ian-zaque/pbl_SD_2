//gcc test.c -o test.run -lwiringPi

//sudo /test.run


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 9600

int main (void) {
        int serial_port;
        char dat, output_address, output_command, input_address, input_command;

        if ((serial_port = serialOpen ("/dev/ttyS0", BAUD_RATE)) < 0) {
                fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
                return 1 ;
        }

        if (wiringPiSetup () == -1)
        {
                fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
                return 1;
        }
        
        printf("Port: %d \n", serial_port);
        dat = serialGetchar (serial_port);
        printf ("Dat: %d \n", dat);
        fflush (stdout) ;
        serialPutchar(serial_port, 0x00);
        dat = serialGetchar (serial_port);
        printf ("Dat: %d \n", dat) ;
        
        output_command = 0x03;
        output_address = 0x03;
        
        //if(serialDataAvail (serial_port) > 0){
            serialPutchar(serial_port, output_command);
            serialPutchar(serial_port, output_address);
        
            input_command = serialGetchar(serial_port);
            input_address = serialGetchar(serial_port);
        
            printf("out: %d , %d \n", output_address, output_command, output_address, output_command);
            printf("in: %d , %d, %X , %X \n", input_address, input_command, input_address, input_command);
        
            if(input_command == 0x00  && input_address == 0x00){
               printf("NodeMCU funcionando normalmente!");
               //return 0x00;
            }
        
            else if(input_command == 0x1F  && input_address == 0x1F){
               printf("NodeMCU com problema!");
               //return 0x1F;
            }
        
            else{
                printf("Problema desconhecido detectado!");
                //return 0x1F;
            }
        //}
        //else{
           //printf("Problema desconhecido detectado!!  %d \n \n",serial_port);
           //return 0x1F;
       //}
        
        return 0;
}
