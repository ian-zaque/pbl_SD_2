// COMPILE WITH WIRING PI
//gcc uart.c -o uart.run -lwiringPi
//sudo /uart.run

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
void recDataInput(int port, char *comm, char *addr, char *val);
void evaluateRecData(char *addr, char *comm, char *value);


int main (void) {
        setlocale(LC_ALL,"Portuguese");			// TO ACTIVATE PORTUGUESE CHARACTERS

        int serial_port, i = 0, j = 0;
        char output_command[] ="", input_command[] ="", input_address[] ="", input_value[] ="";
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
        
        system("cls || clear");
        printf("PBL - Interfaces de E/S. \n \n");
        printf("As ações seguirão a seguinte ordem: \n");
        printf("1 - Solicita a situação atual do NodeMCU. \n");
        printf("2 - Solicita o valor da entrada analógica. \n");
        printf("3 - Solicita o valor de uma das entradas digitais. \n");
        printf("4 - Acendimento do led da NodeMCU. \n");
        printf("5 - Desligamento do led da NodeMCU. \n \n");
        sleep(2);
        

        for(i = 0; i <= 4; i++){           // LOOP TO ENGAGE AUTOMATIC ACTIONS
            if(i == 0){                   //this checks the NodeMCU status.
               printf("Ação: Solicita a situação atual do NodeMCU. \n \n");
               sleep(1);

               output_command[0] = '3';
               output_command[1] = '0';
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "30");

               recData(serial_port, input_command, input_address);
               evaluateRecData(input_address, input_command, "00");
            }

            if(i == 1){            //this requests analog input value
               system("cls || clear");
               printf("Ação: Solicita o valor da entrada analógica. \n \n");
               sleep(1);

               output_command[0] = '4';
               output_command[1] = '0';
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "40");

               recDataInput(serial_port, input_command, input_address, input_value);
               evaluateRecData(input_address, input_command, input_value);
            }

            if(i == 2){            //this requests some digital input value
               system("cls || clear");
               printf("Ação: Solicita o valor de uma das entradas digitais. \n \n");
               sleep(1);

               output_command[0] = '5';
               output_command[1] = '0';
               
              // printf("comm out: %s \n ", output_command);

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
               system("cls || clear");
               printf("Ação: Acendimento do LED da NodeMCU. \n \n");
               sleep(1);

               output_command[0] = '6';
               output_command[1] = '0';
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "60");

               recData(serial_port, input_command, input_address);
               write_textLCD("LED On");
            }

            if(i == 4){            //this turn off the led
               i = -1;             // RESET THE LOOP

               system("cls || clear");
               printf("Ação: Desligamento do LED do NodeMCU. \n \n");
               sleep(1);

               output_command[0] = '7';
               output_command[1] = '0';
               
               fflush (stdout);
               serialFlush(serial_port);
               sendData(serial_port, "70");

               recData(serial_port, input_command, input_address);
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
     char buffer[] = "";
     
     buffer[0] = serialGetchar(port);
     buffer[1] = serialGetchar(port);
     
     comm[0] = buffer[0];
     comm[1] = buffer[1];
     addr[0] = buffer[0];
     addr[1] = buffer[1];
}

void recDataInput(int port, char *addr, char *comm, char *val){     
     char bufferComm[] = "";
     char bufferVal[] = "00";
     
     bufferComm[0] = serialGetchar(port);
     bufferComm[1] = serialGetchar(port);
     
     comm[0] = bufferComm[0];
     comm[1] = bufferComm[1];
     addr[0] = bufferComm[0];
     addr[1] = bufferComm[1];
     
     bufferVal[0] = serialGetchar(port);
     bufferVal[1] = serialGetchar(port);
     
     val[0] = bufferVal[0];
     val[1] = bufferVal[1];
}

void evaluateRecData(char *addr, char *comm, char *value){
    char val[] = "";
    val[0] = value[0];
    val[1] = value[1];
    value = "";

    // 1F
    if ( comm[0] == '1' && comm[1] == 'F'){
       write_textLCD("NodeMcu Not Ok");
    }

    // 00
    else if (comm[0] == '0' && comm[1] == '0'){
       write_textLCD("NodeMcu Ok");
    }

    // 01
    else if (comm[0] == '0' && comm[1] == '1'){
       char text[] = "S. Analog.: ";
       strcat(text, val);
       write_textLCD(text);
    }

    //02
    else if (comm[0] == '0' && comm[1] == '2'){
       char text[] = "S. Digital: ";
       strcat(text, addr);

       char text2[] = " ";
       strcat(text2, value);
       strcat(text, text2);
       write_textLCD(text2);
    }

    else{
        write_textLCD("NodeMcu Not Ok");
        printf("Resposta desconhecida: %s. \n", comm);
        printf("Valor recebido: %s. \n \n", value);
    }
}
