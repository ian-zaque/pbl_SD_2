#ifndef DISPLAY_H
#define DISPLAY_H


/**
 * Limpa os dados escritos no display
*/
extern void clearDisplay();

/**
 * Realiza as rotinas de inicializacao do display
*/
extern void initDisplay();

/**
 * Move o cursor do display
*/
extern void moveCursor();

/**
 * Escreve um caractere no display de LCD
 * @param c - caractere a ser escrito no display
*/
extern void write_lcd(char c);

/**
 * Escreve um texto no display LCD
 * @param  palavra[] - texto a ser escrito no display
 */
void write_textLCD(char palavra[]){
    clearDisplay(); // limpa o display
    int i = 0;

    while(palavra[i] != '\0'){
	    printf("%c", palavra[i]);
        write_lcd(palavra[i]); // exibe no display cada caractere
        i++;
    }
    printf("\n");
}

#endif