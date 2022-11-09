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

//BIBS TO MANIPULATE STRING, PORTUGUESE CHARACTERS, SLEEP, I/O
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <errno.h>

// BIBS TO SERIAL COMMUNICATION
#include <wiringPi.h>
#include <wiringSerial.h>

//BIB TO DISPLAY
#include "display.h"

#define BAUD_RATE 9600

void sendData(int port, char *comm);
void sendDataDigitalInput(int port, char addr, int comm);
void recData(int port, char *comm, char *addr);
void recDataInput(int port, char *addr, char *comm, char *val);
void evaluateRecData(char *addr, char *comm, char *value);


int main (void) {
        setlocale(LC_ALL,"Portuguese");			// TO ACTIVATE PORTUGUESE CHARACTERS

        int serial_port, i = 0, j = 0;
        char *output_command, *input_command, *input_address, *input_value;
        char sensors[10] = {"0123456789"};

        if ((serial_port = serialOpen ("/dev/ttyS0", BAUD_RATE)) < 0) {
                fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
                return 1;
        }

        if (wiringPiSetup () == -1)
        {
                fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno));
                return 1;
        }
        
        initDisplay();  // inicializa o display lcd
        char intro[] = "PBL 2 - E/S";
        write_textLCD(intro);
        
        //system("cls || clear");
        printf("PBL - Interfaces de E/S. \n \n");
        printf("As a��es seguir�o a seguinte ordem: \n");
        printf("1 - Solicita a situa��o atual do NodeMCU. \n");
        printf("2 - Solicita o valor da entrada anal�gica. \n");
        printf("3 - Solicita o valor de uma das entradas digitais. \n");
        printf("4 - Acendimento do led da NodeMCU. \n");
        printf("5 - Desligamento do led da NodeMCU. \n \n");
        sleep(2);
        

        for(i = 0; i <= 4; i++){           // LOOP TO ENGAGE AUTOMATIC ACTIONS
            if(i == 0){                   //this checks the NodeMCU status.
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
               evaluateRecData(input_address, input_command, 0);
            }

            if(i == 1){            //this requests analog input value
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
               evaluateRecData(input_address, input_command, input_value);
            }

            if(i == 2){            //this requests some digital input value
               //system("cls || clear");
               printf("A��o: Solicita o valor de uma das entradas digitais. \n \n");
               sleep(1);

               output_command = "5";
               printf("comm out: %s \n ", output_command);

               /*for (j = 0; j <= 9; j++){   //LOOP TO COMMUTE DIGITAL SENSORS
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
                   //evaluateRecData(&input_address, &input_command, &input_value);
               } */
            }

            if(i == 3){            //this turn on the led
               //system("cls || clear");
               printf("A��o: Acendimento do LED da NodeMCU. \n \n");
               sleep(1);

               output_command = "60";
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "60");

               printf("comm out: %s \n ", "60");

               recData(serial_port, input_command, input_address);
               printf("comm in 0: %s \n \n", input_command);
               printf("comm in 0: %s \n \n", input_address);
               write_textLCD("LED On");
            }

            if(i == 4){            //this turn off the led
               i = -1;             // RESET THE LOOP

               //system("cls || clear");
               printf("A��o: Desligamento do LED do NodeMCU. \n \n");
               sleep(1);

               output_command = "70";
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "70");

               printf("comm out: %s \n ", "70");

               recData(serial_port, input_command, input_address);
               printf("comm in 0: %s \n \n", input_command);
               printf("comm in 0: %s \n \n", input_address);
               write_textLCD("LED Off");
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
     //printf("GETCHAR 0: %s \n \n", buffer);
     buffer[1] = serialGetchar(port);
     //printf("GETCHAR 1: %s \n \n", buffer);
     comm = buffer;
     //printf("GETCHAR 2: %s \n \n", comm);
}

void recDataInput(int port, char *addr, char *comm, char *val){
     // memset(comm, 0, strlen(comm));
     //memset(comm, 0, strlen(addr));
     
     char bufferComm[2];
     char bufferVal[2];
     
     //sleep(2);
     bufferComm[0] = serialGetchar(port);
     //printf("GETCHAR 0: %s \n \n", bufferComm);
     bufferComm[1] = serialGetchar(port);
     //printf("GETCHAR 1: %s \n \n", bufferComm);
     comm = bufferComm;
     //printf("GETCHAR 2: %s \n \n", comm);
     
     bufferVal[0] = serialGetchar(port);
     //printf("GETCHAR 2: %s \n \n", bufferVal);
     bufferVal[1] = serialGetchar(port);
     //printf("GETCHAR 3: %s \n \n", bufferVal);
     val = bufferVal;
     
     //printf("GETCHAR 4: %s \n \n", val);
}

void evaluateRecData(char *addr, char *comm, char *value){

    if ( strcmp(comm, "1F") == 0){
       write_textLCD("NodeMcu Not Ok");
       printf("NodeMcu com problema!! \n \n");
    }

    else if (strcmp(comm, "00") == 0){
       write_textLCD("NodeMcu Ok");
       printf("NodeMcu Ok!! \n \n");
    }

    else if (strcmp(comm, "01") == 0){
       char text[] = "S. Analogico: ";
       strcat(text, value);
       write_textLCD(text);
       printf("Sensor analogico: %s. \n \n", value);
    }

    else if (strcmp(comm, "02") == 0){
       char text[] = "S. Digital: ";
       strcat(text, addr);

       char text2[] = " ";
       strcat(text2, value);
       strcat(text, text2);
       write_textLCD(text2);
       printf("Sensor digital %s: %s. \n \n", addr[1], value);
    }

    else{
        write_textLCD("NodeMcu Not Ok");
        printf("Resposta desconhecida: %s. \n", comm);
        printf("Valor recebido: %s. \n \n", value);
    }
}
