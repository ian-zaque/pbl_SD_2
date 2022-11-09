/* map.s */

@ Constantes importantes para o programa
.equ clrregoffset, 40
.equ setregoffset, 28
.equ map_shared, 1
.equ sys_open, 5
.equ sys_map, 192
.equ nano_sleep, 162
.equ prot_read, 1
.equ prot_write, 2
.equ pagelen, 4096
.equ level, 52
 

/*======================================================
        Funcao de espara
  ======================================================
        Entradas:
                segundos: segundo a aguardar
                milissegundo: milissegundo a aguardar
        Registradores utilizados: r0, r1, r7
                obs:    r0 carrega o valor em s
                        r1 carrega o valor em ms
  ------------------------------------------------------*/
.macro nanoSleep milissegundo
        ldr R0,=timespecsec     @adiciona o valor da variavel second
        ldr R1,=\milissegundo   @paramentro da macro
        mov R7, #nano_sleep
        swi 0
.endm

/*======================================================
        Analisa qual botao foi precionado
  ======================================================
        Entradas:  
                pin: pino a ser acessado
        Registradores utilizados: r0, r2, r3, r8
                obs:    r2 carrega ...
                        r3 carrega o pino
  ------------------------------------------------------*/
.macro GPIOReadRegister pin
        mov r2, r8      @ Endereço dos registradores da GPIO
        add r2, #level  @ offset para acessar o registrador do pin level 0x34 
        ldr r2, [r2]    @ pino5, 19 e 26
        ldr r3, =\pin  
        add r3, #8      @ offset para acessar a terceira word
        ldr r3, [r3]    /* carrega a posiçao do pino -> 
                          ex queremos saber o valor do pino5 =2^5= 32 => 00 000 000 000 000 000 000 000 000 100 000*/
        and r0, r2, r3  @ Filtrando os outros bits => 00 000 000 000 000 000 000 000 000 100 000
.endm

/*======================================================
        Direciona a saida de um dado pino
  ======================================================
        Entradas:
                pin: pino a ser direcionado
        Registradores utilizados: r0, r1, r2, r3, r8
  ------------------------------------------------------*/
.macro GPIODirectionOut pin
        ldr r2, =\pin
        ldr r2, [r2]
        ldr r1, [r8, r2]
        ldr r3, =\pin   @ address of pin table
        add r3, #4      @ load amount to shift from table
        ldr r3, [r3]    @ load value of shift amt
        mov r0, #0b111  @ mask to clear 3 bits
        lsl r0, r3      @ shift into position
        bic r1, r0      @ clear the three bits
        mov r0, #1      @ 1 bit to shift into pos
        lsl r0, r3      @ shift by amount from table
        orr r1, r0      @ set the bit
        str r1, [r8, r2] @ save it to reg to do work
.endm

/*======================================================
        Seta um valor para um determinado pino
  ======================================================
        Entradas:
                pin: pino que recebera a informacao
                value: valor a ser enviado
        Registradores utilizados: r0, r1, r2, r3 r5, r8
  ------------------------------------------------------*/
.macro GPIOValue pin value
        mov r0, #40     @ valor do clear off set
        mov r2, #12     @ valor que ao subtrair o clear off set resulta 28 o set
        mov r1, \value  @ registra o valor 0 ou 1 no registrador
        mul r5, r1, r2  @ Ex r1 recebe o valor 1, ou seja multiplica o 12 do r2 por 1 resultando 12 no r5
        sub r0, r0, r5  @ valor do r5 que é 12 subtraido por 40 do r0 resultando 28 para o r0 ou seja o set do offset
        mov r2, r8      @ Endereço dos registradores da GPIO
        add r2, r2, r0  @ adiciona no r2 o valor do set com o endereço dos regs
        mov r0, #1      @ 1 bit para o shift
        ldr r3, =\pin   @ valor dos endereços dos pinos
        add r3, #8      @ Adiciona offset para shift 
        ldr r3, [r3]    @ carrega o shift da tabela
        lsl r0, r3      @ realiza a mudança
        str r0, [r2]    @ Escreve no registro
.endm 

/*======================================================
        Realiza o mapeamento dos pinos do display
  ======================================================
        Registradores utilizados: r0, r1, r2, r3 r4, r5, 
                                  r7, r8
  ------------------------------------------------------*/
.macro map
        ldr r0, =fileName
	mov r1, #0x1b0
	orr r1, #0x006
	mov r2, r1
	mov r7, #sys_open
	swi 0
	movs r4, r0

	ldr r5, =gpioaddr
	ldr r5, [r5]
	mov r1, #pagelen
	mov r2, #(prot_read + prot_write)
	mov r3, #map_shared
	mov r0, #0
	mov r7, #sys_map
	swi 0
	movs r8, r0
.endm

/*======================================================
        Realiza a entrada de dados na tela (input)
  ======================================================
        Registradores utilizados: r0, r1, r2, r7
                obs: r1 carrega o variavel
  ------------------------------------------------------*/
.macro input variavel, len_variavel
        mov r0, #1          @ atribui 1 ao r0 para escrever na tela
        ldr r1, =\variavel      @ dado para impressao
        mov r2, #\len_variavel  @ tamanho da palavra a ser exidido
        mov r7, #3          @ chamada de sistema para escrita
        swi 0
.endm

/*======================================================
        Realiza a impressao na tela (print)
  ======================================================
        Entradas:
                variavel -> variavel a ser exibido na tela
                len_variavel -> tamanho do variavel
        Registradores utilizados: r0, r1, r2, r7
  ------------------------------------------------------*/
.macro print variavel, len_variavel
        mov r0, #1          @ atribui 1 ao r0 para escrever na tela
        ldr r1, =\variavel      @ dado para impressao
        mov r2, #\len_variavel  @ tamanho da palavra a ser exidido
        mov r7, #4          @ chamada de sistema para escrita
        swi 0 
.endm
