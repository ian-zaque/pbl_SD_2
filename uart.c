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
	char dat;
	int menu_choice = 0;
	
	if ((serial_port = serialOpen ("/dev/ttyS0", BAUD_RATE)) < 0) {
    		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    		return 1 ;
  	}

  	if (wiringPiSetup () == -1)
	{
    		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    		return 1;
  	}
  	
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
        printf("Escolha uma das opcoes abaixo. \n\n");
        printf("1) Receber dados. \n");
        printf("2) Requisitar Sensor. \n");
        printf("3) Sair. \n");
        printf("Opcao: ");
        
        scanf("%d \n", &menu_choice);
        switch(menu_choice){
               case 1:
                    //topo = criar_celula(topo, &qntd_jogadores); //Chamada de função que cria os nós e cadastra os jogadores.
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
			        
              default:
                      printf("Opcao invalida!");
        }
    
    }
    while(menu_choice != 3);
    
    return 0;
}

