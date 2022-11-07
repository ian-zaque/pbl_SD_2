// COMPILE WITH WIRING PI
//gcc uart.c -o uart.run -lwiringPi
//sudo /uart.run

/* REQUESTS
0x03 - Solicita a situação atual do NodeMCU
0x04 - Solicita o valor da entrada analógica
0x05 - Solicita o valor de uma das entradas digitais
0x06 - Acendimento do led da NodeMCU
0x07 - Desligamento do led da NodeMCU
*/

/* RESPONSES
0x1F - NodeMCU com problema
0x00 - NodeMCU funcionando normalmente
0x01 - Medida da entrada analógica
0x02 - Estado da entrada digital
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>                //BIB TO SLEEP FUNCTION
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 9600

void sendData(int port, char addr, char comm);
void sendDataDigitalInput(int port, char addr, int comm);
void recData(int port, char *addr, char *comm);
void recDataInput(int port, char *addr, char *comm, char *val);
void evaluateRecData(char addr, char comm, int value);


int main (void) {
        setlocale(LC_ALL,"Portuguese");			// TO ACTIVATE PORTUGUESE CHARACTERS

        int serial_port, i = 0, digital_input_comm = 50;
        unsigned char output_address, output_command, input_address, input_command, input_value;

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
               printf("Ação: Solicita a situação atual do NodeMCU. \n \n");
               sleep(4);

               output_command = 0x03;
               output_address = 0x03;
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address, output_address);
               printf("comm out: %X , %d \n ", output_command, output_command);
               printf("addr in 0: %X , %d \n ", input_address, input_address);
               printf("comm in 0: %X , %d \n ", input_command, input_command);

               recData(serial_port, &input_address, &input_command);
               printf("addr in 0: %X , %d \n ", input_address, input_address);
               printf("comm in 0: %X , %d \n \n", input_command, input_command);
               evaluateRecData(input_address, input_command, 0);
            }

            if(i == 1){            //write 0x04 twice; this requests analog input value
               //system("cls || clear");
               printf("Ação: Solicita o valor da entrada analógica. \n \n");
               sleep(4);

               output_command = 0x04;
               output_address = 0x04;
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address, output_address);
               printf("comm out: %X , %d \n ", output_command, output_command);
               printf("addr in 1: %X , %d \n ", input_address, input_address);
               printf("comm in 1: %X , %d \n ", input_command, input_command);

               recDataInput(serial_port, &input_address, &input_command, &input_value);
               printf("addr in 1: %X , %d \n ", input_address, input_address);
               printf("comm in 1: %X , %d \n \n", input_command, input_command);
               evaluateRecData(input_address, input_command, input_value);
            }

            if(i == 2){            //this requests some digital input value
               //system("cls || clear");
               printf("Ação: Solicita o valor de uma das entradas digitais. \n \n");
               sleep(4);

               output_command = 0x05;
               output_address = 0x05;   // IMPLEMENT HERE HOW TO CHOOSE WHAT INPUT WILL BE READ

               for (digital_input_comm = 50; digital_input_comm <= 52; digital_input_comm++){   //LOOP TO COMMUTE DIGITAL SENSORS
                   printf("Entrada digital: %d. \n \n", digital_input_comm);
                   sendDataDigitalInput(serial_port, output_address, digital_input_comm);

                   printf("addr out: %X , %d \n ", output_address, output_address);
                   printf("comm out: %X , %d \n ", digital_input_comm, digital_input_comm);
                   printf("addr in 2: %X , %d \n ", input_address, input_address);
                   printf("comm in 2: %X , %d \n ", input_command, input_command);

                   recDataInput(serial_port, &input_address, &input_command, &input_value);
                   printf("addr in 2: %X , %d \n ", input_address, input_address);
                   printf("comm in 2: %X , %d \n \n", input_command, input_command);
                   printf("val in 2: %X , %d \n \n", input_value, input_value);
                   evaluateRecData(input_address, input_command, input_value);
               }
            }

            if(i == 3){            //this turn on the led
               //system("cls || clear");
               printf("Ação: Acendimento do led da NodeMCU. \n \n");
               sleep(4);

               output_command = 0x06;
               output_address = 0x04;   // THIS VALUE MUST BE THE BUILTIN LED PIN
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address, output_address);
               printf("comm out: %X , %d \n ", output_command, output_command);
               printf("addr in 3: %X , %d \n ", input_address, input_address);
               printf("comm in 3: %X , %d \n ", input_command, input_command);

               recData(serial_port, &input_address, &input_command);
               printf("addr in 3: %X , %d \n ", input_address, input_address);
               printf("comm in 3: %X , %d \n \n", input_command, input_command);
            }

            if(i == 4){            //this turn off the led
               i = -1;             // RESET THE LOOP

               //system("cls || clear");
               printf("Ação: Desligamento do led da NodeMCU. \n \n");
               sleep(4);

               output_command = 0x07;
               output_address = 0x04;   // THIS VALUE MUST BE THE BUILTIN LED PIN
               sendData(serial_port, output_address, output_command);

               printf("addr out: %X , %d \n ", output_address, output_address);       
               printf("comm out: %X , %d \n ", output_command, output_command);   
               printf("addr in 4: %X , %d \n ", input_address, input_address); 
               printf("comm in 4: %X , %d \n ", input_command, input_command); 

               recData(serial_port, &input_address, &input_command);
               printf("addr in 4: %X , %d \n ", input_address, input_address);
               printf("comm in 4: %X , %d \n \n", input_command, input_command);
            }

        }

        return 0;
}

void sendData(int port, char addr, char comm){
     serialPutchar(port, addr);
     serialPutchar(port, comm);
}

void sendDataDigitalInput(int port, char addr, int comm){
     serialPutchar(port, addr);
     serialPutchar(port, comm);
}

void recData(int port, char *addr, char *comm){
     *addr = serialGetchar(port);
     *comm = serialGetchar(port);
}

void recDataInput(int port, char *addr, char *comm, char *val){
     *addr = serialGetchar(port);
     *comm = serialGetchar(port);
     *val  = serialGetchar(port);
}

void evaluateRecData(char addr, char comm, int value){

    if (addr == 0x03 && comm == 0x1F){
       printf("NodeMCU com problema!! \n \n");
       // escreve no display
    }

    if(addr != 0x03 && comm == 0x1F){
       printf("NodeMCU com problema, sensor desconhecido!! \n \n");
       // escreve no display
    }

    if (comm == 0x01){
       printf("Medida da entrada analógica: %d. \n \n", value);
       // escreve no display
    }

    if (comm == 0x02){
        printf("Medida da entrada digital: %d. \n \n", value);
       // escreve no display
    }

    else{
        printf("Resposta desconhecida: %X. \n", comm);
        printf("Valor recebido: %d. \n \n", value);
       // escreve no display
    }
}
