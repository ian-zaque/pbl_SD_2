// COMPILE WITH WIRING PI
//gcc uart.c -o uart.run -lwiringPi
//sudo /uart.run

/* REQUESTS
0x03 - Solicita a situa��o atual do NodeMCU
0x04 - Solicita o valor da entrada anal�gica
0x05 - Solicita o valor de uma das entradas digitais
0x06 - Acendimento do led da NodeMCU
0x07 - Desligamento do led da NodeMCU
*/

/* RESPONSES
0x1F - NodeMCU com problema
0x00 - NodeMCU funcionando normalmente
0x01 - Medida da entrada anal�gica
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

void sendData(int port, char *comm);
void sendDataDigitalInput(int port, char addr, int comm);
void recData(int port, char *comm, char *addr);
void recDataInput(int port, char *addr, char *comm, char *val);
void evaluateRecData(char addr, char comm, int value);


int main (void) {
        setlocale(LC_ALL,"Portuguese");			// TO ACTIVATE PORTUGUESE CHARACTERS

        int serial_port, i = 0, j = 0;
        char *output_command, *input_command, *input_address, *input_value;
        char sensors[9] = {"012345678"};

        if ((serial_port = serialOpen ("/dev/ttyS0", BAUD_RATE)) < 0) {
                fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
                return 1;
        }

        if (wiringPiSetup () == -1)
        {
                fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno));
                return 1;
        }
        
        //system("cls || clear");
        printf("PBL - Interfaces de E/S. \n \n");
        printf("As a��es seguir�o a seguinte ordem: \n");
        printf("1 - Solicita a situa atual do NodeMCU. \n");
        printf("2 - Solicita o valor da entrada anal�gica. \n");
        printf("3 - Solicita o valor de uma das entradas digitais. \n");
        printf("4 - Acendimento do led da NodeMCU. \n");
        printf("5 - Desligamento do led da NodeMCU. \n \n");
        sleep(4);
        
        /*printf("A��o: Solicita a situa��o atual do NodeMCU. \n \n");
        sleep(1);

       output_command = "30";
       
       fflush (stdout);
       serialFlush(serial_port);
       sendData(serial_port, "30");

       printf("comm out: %s \n ", output_command);

       recData(serial_port, input_command, input_address);
       printf("comm in 0: %s \n \n", input_command);
       printf("comm in 0: %s \n \n", input_address); */

        for(i = 0; i <= 4; i++){
            if(i == 0){                   //write 0x03 twice; this checks the NodeMCU status.
               printf("A��o: Solicita a situa��o atual do NodeMCU. \n \n");
               sleep(1);

               output_command = "30";
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "30");

               printf("comm out: %s \n ", output_command);

               recData(serial_port, input_command, input_address);
               printf("comm in 0: %s \n \n", input_command);
               printf("comm in 0: %s \n \n", input_address);
               //evaluateRecData(input_address, input_command, 0);
            }

            if(i == 1){            //write 0x04 twice; this requests analog input value
               //system("cls || clear");
               printf("A��o: Solicita o valor da entrada anal�gica. \n \n");
               sleep(1);

               output_command = "40";
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "40");

               printf("comm out: %s \n ", output_command);

               recDataInput(serial_port, input_command, input_address, input_value);
               printf("comm in 0: %s \n \n", input_command);
               printf("comm in 0: %s \n \n", input_address);
               //evaluateRecData(input_address, input_command, input_value);
            }

            if(i == 2){            //this requests some digital input value
               //system("cls || clear");
               printf("A��o: Solicita o valor de uma das entradas digitais. \n \n");
               sleep(1);

               output_command = "5";
               printf("comm out: %s \n ", output_command);

               for (j = 0; j <= 8; j++){   //LOOP TO COMMUTE DIGITAL SENSORS
                   printf("Entrada digital: %d. \n \n", j);
                   //printf("jjjj %s , %c \n ", &sensors[j], sensors[j]);
                   strcat(output_command, &sensors[j]);
                   //printf("comm out: %s \n ", output_command);
                   
                   fflush (stdout);
                   serialFlush(serial_port);
                   sendData(serial_port, output_command);

                   printf("comm out: %s \n ", output_command);
    
                   recDataInput(serial_port, input_command, input_address, input_value);
                   printf("comm in 0: %s \n \n", input_command);
                   printf("comm in 0: %s \n \n", input_address);
                   //evaluateRecData(input_address, input_command, input_value);
               }
            }

            if(i == 3){            //this turn on the led
               //system("cls || clear");
               printf("A��o: Acendimento do led da NodeMCU. \n \n");
               sleep(1);

               output_command = "60";  // THIS VALUE MUST BE THE BUILTIN LED PIN
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "60");

               printf("comm out: %s \n ", "60");

               recData(serial_port, input_command, input_address);
               printf("comm in 0: %s \n \n", input_command);
               printf("comm in 0: %s \n \n", input_address);
            }

            if(i == 4){            //this turn off the led
               i = -1;             // RESET THE LOOP

               //system("cls || clear");
               printf("A��o: Desligamento do led da NodeMCU. \n \n");
               sleep(1);

               output_command = "70";   // THIS VALUE MUST BE THE BUILTIN LED PIN
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "70");

               printf("comm out: %s \n ", "70");

               recData(serial_port, input_command, input_address);
               printf("comm in 0: %s \n \n", input_command);
               printf("comm in 0: %s \n \n", input_address);
            }

        }

        return 0;
}

void sendData(int port, char *comm){
     serialPuts(port, comm);
}

void sendDataDigitalInput(int port, char addr, int comm){
     serialPutchar(port, comm);
     serialPutchar(port, addr);
}

void recData(int port, char *comm, char *addr){
    // memset(comm, 0, strlen(comm));
     //memset(comm, 0, strlen(addr));
     
     char buffer[2];
     
     //sleep(2);
     buffer[0] = serialGetchar(port);
     printf("GETCHAR 0: %s \n \n", buffer);
     buffer[1] = serialGetchar(port);
     printf("GETCHAR 1: %s \n \n", buffer);
     comm = buffer;
     printf("GETCHAR 2: %s \n \n", comm);
}

void recDataInput(int port, char *addr, char *comm, char *val){
     // memset(comm, 0, strlen(comm));
     //memset(comm, 0, strlen(addr));
     
     char bufferComm[2];
     char bufferVal[2];
     
     //sleep(2);
     bufferComm[0] = serialGetchar(port);
     printf("GETCHAR 0: %s \n \n", bufferComm);
     bufferComm[1] = serialGetchar(port);
     printf("GETCHAR 1: %s \n \n", bufferComm);
     comm = bufferComm;
     printf("GETCHAR 2: %s \n \n", comm);
     
     bufferVal[0] = serialGetchar(port);
     printf("GETCHAR 2: %s \n \n", bufferVal);
     bufferVal[1] = serialGetchar(port);
     printf("GETCHAR 3: %s \n \n", bufferVal);
     val = bufferVal;
     
     printf("GETCHAR 3: %s \n \n", val);
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
       printf("Medida da entrada anal�gica: %d. \n \n", value);
       // escreve no display
    }

    if (comm == 0x02){
        printf("Medida da entrada digital: %d. \n \n", value);
       // escreve no display
    }

    else{
        printf("Resposta desconhecida: %X , %c. \n", comm, comm);
        printf("Valor recebido: %d , %c. \n \n", value);
       // escreve no display
    }
}
