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

 	//while(1){
	//	if(serialDataAvail (serial_port) ) {
			//printf("%c", serial_port);
	///		dat = serialGetchar (serial_port);
	//		printf ("%c", dat) ;
	//		fflush (stdout) ;
	//		serialPutchar(serial_port, dat);
	//	}
	//}
	
    
    if ((serial_port = serialOpen ("/dev/ttyS0", BAUD_RATE)) < 0) {
  		fprintf (stderr, "Conexao serial inviavel. Dispositivo serial inapto: %s\n", strerror (errno)) ;
  		return 0x1F;
  	}

  	if (wiringPiSetup () == -1){
 		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
 		return 0x1F;
  	}
  	
  	if(serialDataAvail (serial_port) ) {
        printf("NodeMCU esta ok. Conexao serial estabelecida!");
        return 0x00;
    }
    	
	/*do{
        printf("Escolha uma das opcoes abaixo: \n\n");
        printf("1) Status NodeMCU. \n");                //Cod 0x03
        printf("2) Valor da entrada analogica. \n");    //Cod 0x04
        printf("3) Valor de entrada digital. \n");      //Cod 0x05
        printf("4) Acender LED NodeMCU. \n");           //Cod 0x06
        printf("Opcao: ");
        //scanf("%d \n", &menu_choice);

        switch(menu_choice){   
               case 1:
                    serial_output[0] = 0x03; serial_output[1] = 0x03;
                    printf("\n out 0: %X , %d \n", serial_output[0], serial_output[0]);
                    printf("out 1: %X , %d \n", serial_output[1], serial_output[1]);
                    printf("port: %d \n", serial_port);
                    status_nodemcu = statusNodeMcuConnection(serial_port, serial_input, serial_output);
                    printf("status: %d , %X \n \n", status_nodemcu, status_nodemcu);
                    //system("cls");
			        break;

               case 2:
                    //topo = criar_celula(topo, &qntd_jogadores); //Chamada de função que cria os nós e cadastra os jogadores.
                    //system("cls");
			        break;

               case 3:
                    //topo = criar_celula(topo, &qntd_jogadores); //Chamada de função que cria os nós e cadastra os jogadores.
                    //system("cls");
			        break;

               case 4:
                    //topo = criar_celula(topo, &qntd_jogadores); //Chamada de função que cria os nós e cadastra os jogadores.
                    //system("cls");
			        break;

              default:
                      printf("Opcao invalida!");
        }

    }
    while(menu_choice != 0);
    */
    serialFlush (serial_port);
    serial_output[0] = 0x03;
    serial_output[1] = 0x03;
    printf("\n out 0: %X , %d \n", serial_output[0], serial_output[0]);
    printf("out 1: %X , %d \n", serial_output[1], serial_output[1]);
    printf("port: %d \n", serial_port);
    status_nodemcu = statusNodeMcuConnection(serial_port, serial_input, serial_output);
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
