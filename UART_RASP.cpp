//gcc test.c -o test.run -lwiringPi

//sudo /test.run


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 9600

int main (void) {
        int serial_port ;
        char dat;

        if ((serial_port = serialOpen ("/dev/ttyS0", BAUD_RATE)) < 0) {
                fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
                return 1 ;
        }

        if (wiringPiSetup () == -1)
        {
                fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
                return 1;
        }
        while(1){
                if(serialDataAvail (serial_port) ) {
                        printf("Port: %c \n", serial_port);
                        dat = serialGetchar (serial_port);
                        printf ("Dat: %c \n", dat) ;
                        fflush (stdout) ;
                        serialPutchar(serial_port, dat);
                }
        }

}
