//gcc test.c -o test.run -lwiringPi

//sudo /test.run


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<locale.h>
#include <unistd.h>                //BIB TO SLEEP FUNCTION
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 9600

void sendData(int port, char addr, char comm);
void recData(int port, char *addr, char *comm);

int main (void) {
        setlocale(LC_ALL,"Portuguese");			//Comando para utilizar caracteres a lingua portuguesa.
    
        int serial_port, i = 0;
        unsigned char output_address, output_command, input_address, input_command;

        if ((serial_port = serialOpen ("/dev/ttyS0", BAUD_RATE)) < 0) {
                fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
                return 1 ;
        }

        if (wiringPiSetup () == -1)
        {
                fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
                return 1;
        }

        system("cls || clear");
        printf("PBL - Interfaces de E/S. \n \n");
        printf("As ações seguirão a seguinte ordem: \n");
        printf("1 - Solicita a situa atual do NodeMCU. \n");
        printf("2 - Solicita o valor da entrada analógica. \n");
        printf("3 - Solicita o valor de uma das entradas digitais. \n");
        printf("4 - Acendimento do led da NodeMCU. \n");
        printf("5 - Desligamento do led da NodeMCU. \n \n");
        sleep(4);

        for(i = 0; i <= 4; i++){
            if(i == 0){                   //write 0x03 twice; this checks the NodeMCU status.
               system("cls || clear");
               printf("Ação: Solicita a situação atual do NodeMCU. \n \n");
               sleep(4);

               output_command = 0x03;
               output_address = 0x03;
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address);
               printf("comm out: %X , %d \n ", output_command);
               printf("addr in 0: %X , %d \n ", input_address);
               printf("comm in 0: %X , %d \n ", input_command);

               recData(serial_port, &input_address, &input_command);
               printf("addr in 0: %X , %d \n ", input_address);
               printf("comm in 0: %X , %d \n \n", input_command);
            }

            if(i == 1){            //write 0x04 twice; this requests analog input value
               system("cls || clear");
               printf("Ação: Solicita o valor da entrada analógica. \n \n");
               sleep(4);

               output_command = 0x04;
               output_address = 0x04;
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address);
               printf("comm out: %X , %d \n ", output_command);
               printf("addr in 1: %X , %d \n ", input_address);
               printf("comm in 1: %X , %d \n ", input_command);

               recData(serial_port, &input_address, &input_command);
               printf("addr in 1: %X , %d \n ", input_address);
               printf("comm in 1: %X , %d \n \n", input_command);
            }

            if(i == 2){            //this requests some digital input value
               system("cls || clear");
               printf("Ação: Solicita o valor de uma das entradas digitais. \n \n");
               sleep(4);

               output_command = 0x05;
               output_address = 0x04;   // IMPLEMENT HERE HOW TO CHOOSE WHAT INPUT WILL BE READ
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address);
               printf("comm out: %X , %d \n ", output_command);
               printf("addr in 2: %X , %d \n ", input_address);
               printf("comm in 2: %X , %d \n ", input_command);

               recData(serial_port, &input_address, &input_command);
               printf("addr in 2: %X , %d \n ", input_address);
               printf("comm in 2: %X , %d \n \n", input_command);
            }

            if(i == 3){            //this turn on the led
               system("cls || clear");
               printf("Ação: Acendimento do led da NodeMCU. \n \n");
               sleep(4);

               output_command = 0x06;
               output_address = 0x04;   // THIS VALUE MUST BE THE BUILTIN LED PIN
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address);
               printf("comm out: %X , %d \n ", output_command);
               printf("addr in 3: %X , %d \n ", input_address);
               printf("comm in 3: %X , %d \n ", input_command);

               recData(serial_port, &input_address, &input_command);
               printf("addr in 3: %X , %d \n ", input_address);
               printf("comm in 3: %X , %d \n \n", input_command);
            }

            if(i == 4){            //this turn off the led
               i = -1;       // RESET THE LOOP

               system("cls || clear");
               printf("Ação: Desligamento do led da NodeMCU. \n \n");
               sleep(4);

               output_command = 0x07;
               output_address = 0x04;   // THIS VALUE MUST BE THE BUILTIN LED PIN
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address);       
               printf("comm out: %X , %d \n ", output_command);   
               printf("addr in 4: %X , %d \n ", input_address); 
               printf("comm in 4: %X , %d \n ", input_command); 

               recData(serial_port, &input_address, &input_command);
               printf("addr in 4: %X , %d \n ", input_address);
               printf("comm in 4: %X , %d \n \n", input_command);
            }

        }
            
        return 0;
}

void sendData(int port, char addr, char comm){
     serialPutchar(port, addr);
     serialPutchar(port, comm);
}

void recData(int port, char *addr, char *comm){
     *addr = serialGetchar(port);
     *comm = serialGetchar(port);
}

void evaluateData(char addr, char comm){
    // printf("NOT IMPLEMENTED");
    if (comm == 0xFE){
       printf("É HEXA UHUUUUUL!!");
    }

    if (comm == 0xFE){
       printf("É HEXA UHUUUUUL!!");
    }
}
