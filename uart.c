//gcc test.c -o test.run -lwiringPi

//sudo /test.run


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 9600

int startNodeMcuConnection(int *port);
int valueAnalogicInput(int *port);

int main (void) {
	int serial_port;
	char serial_input[2], serial_output[2];        //[0] => Command; [1] => Address
	int menu_choice = 99, status_nodemcu = 0;
  	
 	//while(1){
	//	if(serialDataAvail (serial_port) ) {
			//printf("%c", serial_port);
	//		dat = serialGetchar (serial_port);
	//		printf ("%c", dat) ;
	//		fflush (stdout) ;
	//		serialPutchar(serial_port, dat);
	//	}
	//}
	do{
        printf("Escolha uma das opcoes abaixo: \n\n");
        printf("1) Status NodeMCU. \n");                //Cod 0x03
        printf("2) Valor da entrada analógica. \n");    //Cod 0x04
        printf("3) Valor de entrada digital. \n");      //Cod 0x05
        printf("4) Acender LED NodeMCU. \n");           //Cod 0x06
        printf("0) Sair. \n");
        printf("Opcao: ");
        scanf("%d \n", &menu_choice);
        
        switch(menu_choice){
               case 1:
                    status_nodemcu = startNodeMcuConnection(&serial_port);
                    system("cls");
			        break;
			        
               case 2:
                    //topo = criar_celula(topo, &qntd_jogadores); //Chamada de função que cria os nós e cadastra os jogadores.
                    system("cls");
			        break;
			        
               case 3:
                    //topo = criar_celula(topo, &qntd_jogadores); //Chamada de função que cria os nós e cadastra os jogadores.
                    system("cls");
			        break;
			        
               case 4:
                    //topo = criar_celula(topo, &qntd_jogadores); //Chamada de função que cria os nós e cadastra os jogadores.
                    system("cls");
			        break;
			        
              case 0:
                    printf("Programa encerrado!");
                    exit;
			        break;
			        
              default:
                      printf("Opcao invalida!");
        }
    
    }
    while(menu_choice != 0);
    
    return 0;
}



int startNodeMcuConnection(int *port){
    
    //write 0x03 twice; this checks the NodeMCU status
    serial_output[0] = 0x03;
    serial_output[1] = 0x03;
    serialPutchar(port, serial_output[0]);
    serialPutchar(port, serial_output[1]);
    
    for(int i = 0; i < 2; i++){
        serial_input[i] = serialGetchar(port);
    }
    
    if(serial_input[0] == 0x00  && serial_input[1] == 0x00){
       printf("NodeMCU funcionando normalmente!");
       return 0x00;  
    }
    
    else if(serial_input[0] == 0x1F  && serial_input[1] == 0x1F){
       printf("NodeMCU com problema!");
       return 0x1F;
    }
  	
    else{
        printf("Problema desconhecido detectado!");
        return 0x1F;
    }   
}
