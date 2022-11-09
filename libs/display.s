/* display_lcd.s */
.include "map.s"


.global clearDisplay
.global moveCursor
.global initDisplay
.global write_lcd


/*======================================================
        Chama a saida dos pinos do display
  ======================================================
        Utiliza a macro GPIODirectionOut presente no
        map.s para chamar a saida de cada pino do display
  ------------------------------------------------------*/
.macro  setOut
        GPIODirectionOut pinE
        GPIODirectionOut pinRS
        GPIODirectionOut pinDB7
        GPIODirectionOut pinDB6
        GPIODirectionOut pinDB5
        GPIODirectionOut pinDB4
        .ltorg
.endm

/*======================================================
        Ativa ou desativa os pinos do display LCD
        Recebe o nivel logico de cada uma das entradas 
        dos pinos com excecao do enable
  ======================================================
        Macros utilizadas: 
                GPIOValue: presente no map.s, utilizada 
                para setar um determinado dado no display.
        Entradas: 
                RS, DB7, DB6, DB5, DB4
  ------------------------------------------------------*/
.macro setDisplay RS, DB7, DB6, DB5, DB4
        GPIOValue pinE, #0
        GPIOValue pinRS, #\RS
        GPIOValue pinE, #1
        GPIOValue pinDB7, #\DB7
        GPIOValue pinDB6, #\DB6
        GPIOValue pinDB5, #\DB5
        GPIOValue pinDB4, #\DB4
        GPIOValue pinE, #0
.endm

/*======================================================
        Inicializa o display seguindo as orientacoes do 
        datashit
  ======================================================
        Macros utilizadas:  
                setDisplay: usada para setar um
                determinado dado no display
                nanoSleep: presente no map.s, utilizada
                para aguardar um determinado tempo
  ------------------------------------------------------*/
.macro init
        setDisplay 0, 0, 0, 1, 1  
        nanoSleep timespecnano5

        setDisplay 0, 0, 0, 1, 1 
        nanoSleep timespecnano150  

        setDisplay  0, 0, 0, 1, 1


        setDisplay 0, 0, 0, 1, 0
        nanoSleep timespecnano150  

        .ltorg 

        setDisplay 0, 0, 0, 1, 0 

        setDisplay 0, 0, 0, 0, 0
        nanoSleep timespecnano150 
        setDisplay 0, 0, 0, 0, 0 

        setDisplay 0, 1, 0, 0, 0  
        nanoSleep timespecnano150

        setDisplay 0, 0, 0, 0, 0  

        setDisplay 0, 0, 0, 0, 1  
        nanoSleep timespecnano150 

        setDisplay 0, 0, 0, 0, 0

        setDisplay 0, 0, 1, 1, 0 
        nanoSleep timespecnano150

        .ltorg
.endm

/*======================================================
        Realiza o deslocamento do cursor
  ======================================================
        Macros utilizadas:  
                setDisplay: usada para setar um
                determinado dado no display
                nanoSleep: presente no map.s, utilizada
                para aguardar um determinado tempo
  ------------------------------------------------------*/
.macro entryModSetMacro
        setDisplay 0, 0, 0, 0, 0
        setDisplay 0, 1, 1, 1, 0
        nanoSleep timespecnano150

        setDisplay 0, 0, 0, 0, 0
        setDisplay 0, 0, 1, 1, 0
        nanoSleep timespecnano150
        .ltorg
.endm

/*======================================================
        Limpa o display
  ======================================================
        Macros utilizadas:  
                GPIOsetDisplay: usada para setar um
                determinado dado no display
                nanoSleep: presente no map.s, utilizada
                para aguardar um determinado tempo
  ------------------------------------------------------*/
clearDisplay:
        setDisplay 0, 0, 0, 0, 0
        setDisplay 0, 0, 0, 0, 1
        nanoSleep timespecnano150 
bx lr

/*======================================================
        Realiza o deslocamento do cursor
  ======================================================
        Macros utilizadas:  
                setDisplay: usada para setar um
                determinado dado no display
                nanoSleep: presente no map.s, utilizada
                para aguardar um determinado tempo
  ------------------------------------------------------*/
moveCursor:
        setDisplay 0, 0, 0, 0, 0
        setDisplay 0, 1, 1, 1, 0
        nanoSleep timespecnano150

        setDisplay 0, 0, 0, 0, 0
        setDisplay 0, 0, 1, 1, 0
        nanoSleep timespecnano150
        .ltorg
bx lr

/*======================================================
        Inicializa o display e faz o mapeamento dos pinos
  ======================================================
        Macros utilizadas:  
                moveCursor
                init
                setOut

                map: Realiza o mapeamento
  ------------------------------------------------------*/
initDisplay:
        map
        setOut
        init
        entryModSetMacro
bx lr

/*======================================================
        Realiza a exibicao de uma cadeia de caracteres 
        no display
  ======================================================
        Macros utilizadas:  
                GPIOValue: usada para setar cada bit que
                representa o caractere
                nanoSleep: presente no map.s, utilizada
                para aguardar um determinado tempo
  ------------------------------------------------------*/
write_lcd:
        mov r9, r0             @ passa o valor do texto para r10
        mov r6, #7
        mov r10, #256
        loop_bit:                @ percorre todos os 8 bits do bit para sabe o nivel logico
                lsr r10, #1      @ desloca o bit para a direita  ex: 100000000 -> 010000000
                and r4, r10, r9  @ faz um and entre r10 e r9 para saber se o bit esta ativo ou nao
                cmp r4, #0
                beq switch 	 @ se for igual a 0 valor nao sera alterado          
                mov r4, #1
                switch:
                        @ se for 0 seta no pino DB4
                        cmp r6, #0
                        beq case4
                        cmp r6, #4
                        beq case4
                        @ se for 1 seta no pino DB5
                        cmp r6, #1
                        beq case3
                        cmp r6, #5
                        beq case3
                        @ se for 2 seta no pino DB6
                        cmp r6, #2
                        beq case2
                        cmp r6, #6
                        beq case2
                        @ se for 3 seta no pino DB7
                        cmp r6, #3
                        beq case1
                        cmp r6, #7
                        beq case1

                        case1:
                                GPIOValue pinE, #0 @ atribui 0 ao enable
                               	GPIOValue pinRS, #1
                                GPIOValue pinE, #1
                                GPIOValue pinDB7, r4
                                b retornar @ pula os outros casos
                        case2:
                                GPIOValue pinDB6, r4
                                b retornar  @ pula os outros casos
                        case3:
                                GPIOValue pinDB5, r4
                                b retornar @ pula os outros casos
                        case4:
                                GPIOValue pinDB4, r4
                                GPIOValue pinE, #0
                                @enable
                retornar:
                        sub r6, #1       @ subtrai +1 a r0
                        cmp r10, #1      @ compara o valor de r0 para saber se ja percorreu o ultimo bit
        bne loop_bit
	nanoSleep timespecnano5
bx lr

@ variaveis utilizadas no codigo
.data
	timespecsec: .word 0 @definição do nano sleep 0s permitindo os milissegundos
	timespecnano5: .word 5000000 @valor em milisegundos para lcd
	timespecnano150: .word 150000 @valor em milisegundos para LCD

	fileName: .asciz "/dev/mem"
	gpioaddr: .word 0x20200 @carrega o endereco os onde registradores do controlador GPIO são mapeados na memória

        @ pino do LED
        pin6:   .word 0
                .word 18
                .word 6

        @ pinos dos botoes
        pin19:  .word 4
                .word 27
                .word 524288
        pin26:  .word 8
                .word 18
                .word 67108864 

        @ pinos do display LCD
        pinRS:	@ Pino RS - GPIO25
                .word 8
                .word 15
                .word 25
	pinE:	@ Pino Enable - GPIO1
		.word 0
		.word 3
		.word 1
	pinDB4: @ Pino DB4 - GPIO12
		.word 4
		.word 6
		.word 12
	pinDB5: @ Pino DB5 - GPIO16
		.word 4
		.word 18
		.word 16
	pinDB6: @ Pino DB6 - GPIO20
		.word 8
		.word 0
		.word 20
	pinDB7: @ Pino DB7 - GPIO21
		.word 8
		.word 3
		.word 21
