.equ pagelen, 4096
.equ setregoffset, 28
.equ clrregoffset, 40
.equ prot_read, 1
.equ prot_write, 2
.equ map_shared, 1
.equ sys_open, 5
.equ sys_map, 192
.equ nano_sleep, 162
.equ level, 52

.global _start


.macro nanoSleep time
        LDR R0,=\time
        LDR R1,=\time
        MOV R7, #nano_sleep
        SVC 0
.endm

.macro sleep
        LDR R0,=timespecsec
        LDR R1,=timespecsec
        MOV R7, #nano_sleep
        SVC 0
.endm

.macro GPIODirectionOut pin
        LDR R2, =\pin
        LDR R2, [R2]
        LDR R1, [R8, R2]
        LDR R3, =\pin @ address of pin table
        ADD R3, #4 @ load amount to shift from table
        LDR R3, [R3] @ load value of shift amt
        MOV R0, #0b111 @ mask to clear 3 bits
        LSL R0, R3 @ shift into position
        BIC R1, R0 @ clear the three bits
        MOV R0, #1 @ 1 bit to shift into pos
        LSL R0, R3 @ shift by amount from table
        ORR R1, R0 @ set the bit
        STR R1, [R8, R2] @ save it to reg to do work
.endm

.macro GPIOReadRegister pin
        MOV R2, R8
        ADD R2, #level
        LDR R2, [R2]
        LDR R3, =\pin
        ADD R3, #8
        LDR R3, [R3]
        AND R0, R2, R3
.endm

.macro GPIOTurnOn pin
        MOV R2, R8 @ address of gpio regs
        ADD R2, #setregoffset @ off to set reg
        MOV R0, #1 @ 1 bit to shift into pos
        LDR R3, =\pin @ base of pin info table
        ADD R3, #8 @ add offset for shift amt
        LDR R3, [R3] @ load shift from table
        LSL R0, R3 @ do the shift
        STR R0, [R2] @ write to the register
.endm


.macro GPIOTurnOff pin
        MOV R2, R8 @ address of gpio regs
        ADD R2, #clrregoffset @ off set of clr reg
        MOV R0, #1 @ 1 bit to shift into pos
        LDR R3, =\pin @ base of pin info table
        ADD R3, #8
        LDR R3, [R3]
        LSL R0, R3
        STR R0, [R2]
.endm


.macro setOut
        GPIODirectionOut pinE
        GPIODirectionOut pinRS
        GPIODirectionOut pinDB7
        GPIODirectionOut pinDB6
        GPIODirectionOut pinDB5
        GPIODirectionOut pinDB4
.endm

.macro enable
        GPIOTurnOff pinE
        nanoSleep time1ms
        GPIOTurnOn pinE
        nanoSleep time1ms
        GPIOTurnOff pinE
        .ltorg
.endm


.macro functionSet
        GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOff pinDB4
        enable
.endm


.macro display
        GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable

        GPIOTurnOff pinRS
        GPIOTurnOn pinDB7
        GPIOTurnOn pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
.endm


.macro displayOff
        GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable

        GPIOTurnOff pinRS
        GPIOTurnOn pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable	
.endm


.macro displayClear
        GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable

        GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOn pinDB4
        enable
.endm


.macro entrySetMode
        GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable

        GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOn pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOff pinDB4
        enable
.endm


.macro write_0
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable
.endm

.macro write_1
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOn pinDB4
        enable
.endm

.macro write_2
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOff pinDB4
        enable
.endm

.macro write_3
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
.endm

.macro write_4
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOn pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable
.endm

.macro write_5
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOn pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOn pinDB4
        enable
.endm

.macro write_6
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOn pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOff pinDB4
        enable
.endm

.macro write_7
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOn pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
.endm

.macro write_8
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOn pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable
.endm

.macro write_9
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOn pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOn pinDB4
        enable
.endm

.macro cursorBack
	GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOn pinDB4
        enable

        GPIOTurnOff pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable
.endm

.macro writeNUll
	GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOn pinDB5
        GPIOTurnOn pinDB4
        enable
        
        GPIOTurnOn pinRS
        GPIOTurnOff pinDB7
        GPIOTurnOff pinDB6
        GPIOTurnOff pinDB5
        GPIOTurnOff pinDB4
        enable
.endm

_start:
	@ opening the file
	LDR R0, = fileName
	MOV R1, #0x1b0
	ORR R1, #0x006
	MOV R2, R1
	MOV R7, #sys_open
	SVC 0
	MOVS R4, R0

	@ preparing the mapping
	LDR R5, =gpioaddr
	LDR R5, [R5]
	MOV R1, #pagelen
	MOV R2, #(prot_read + prot_write)
	MOV R3, #map_shared
	MOV R0, #0
	MOV R7, #sys_map
	SVC 0
	MOVS R8, R0

	setOut
	displayClear
	functionSet
	display
	entrySetMode
	
    before:
        GPIOReadRegister pinB19
        CMP r0, r3              @Se B19 
        BNE timer
        B before

    timer:
        writeNUll

        @initialValues				@Seta #99 em R11 R12
        MOV R11, #0b1001	@DEZENA
        MOV R12, #0b1001	@UNIDADE

        loop:
            checkDezena:
                CMP R11, #0b1001	@Se Dezena for 9
                BEQ dezena9
                CMP R11, #0b1000	@Se Dezena for 8
                BEQ dezena8
                CMP R11, #0b0111	@Se Dezena for 7
                BEQ dezena7
                CMP R11, #0b0110	@Se Dezena for 6
                BEQ dezena6
                CMP R11, #0b0101	@Se Dezena for 5
                BEQ dezena5
                CMP R11, #0b0100	@Se Dezena for 4
                BEQ dezena4
                CMP R11, #0b0011	@Se Dezena for 3
                BEQ dezena3
                CMP R11, #0b0010	@Se Dezena for 2
                BEQ dezena2
                CMP R11, #0b0001	@Se Dezena for 1
                BEQ dezena1
                CMP R11, #0b0000	@Se Dezena for 0
                BNE subtrDezena
                BEQ dezena0
        
            checkUnidade:
                CMP R12, #0b1001	@Se UNIDADE for 9
                BEQ unidade9
                CMP R12, #0b1000	@Se UNIDADE for 8
                BEQ unidade8
                CMP R12, #0b0111	@Se UNIDADE for 7
                BEQ unidade7
                CMP R12, #0b0110	@Se UNIDADE for 6
                BEQ unidade6
                CMP R12, #0b0101	@Se UNIDADE for 5
                BEQ unidade5
                CMP R12, #0b0100	@Se UNIDADE for 4
                BEQ unidade4
                CMP R12, #0b0011	@Se UNIDADE for 3
                BEQ unidade3
                CMP R12, #0b0010	@Se UNIDADE for 2
                BEQ unidade2
                CMP R12, #0b0001	@Se UNIDADE for 1
                BEQ unidade1
                CMP R12, #0b0000	@Se UNIDADE for 0
                BEQ unidade0
                BNE loop
        
        subtrDezena:
            SUB R11, #0b0001
            MOV R12, #0b1001
            B checkUnidade
        
        dezena9:
            displayClear
            write_9
            B checkUnidade
        dezena8:
            displayClear
            write_8
            B checkUnidade
        dezena7:
            displayClear
            write_7
            B checkUnidade
        dezena6:
            displayClear
            write_6
            B checkUnidade
        dezena5:
            displayClear
            write_5
            B checkUnidade
        dezena4:
            displayClear
            write_4
            B checkUnidade
        dezena3:
            displayClear
            write_3
            B checkUnidade
        dezena2:
            displayClear
            write_2
            B checkUnidade
        dezena1:
            displayClear
            write_1
            B checkUnidade
        dezena0:
            displayClear
            write_0
            B checkUnidade
        
        
        unidade9:
            write_9
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade8:
            write_8
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade7:
            write_7
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade6:
            write_6
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade5:
            write_5
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade4:
            write_4
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade3:
            write_3
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade2:
            write_2
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade1:
            write_1
            sleep
            SUB R12, #0b0001
            B checkDezena
        unidade0:
            CMP R11, #0b0000
            BEQ resetNumber
            write_0
            sleep
            MOV R12, #0b1001
            SUB R11, #0b0001
            B checkDezena

        resetNumber:
            MOV R11, #0b1001	@DEZENA
            MOV R12, #0b1001	@UNIDADE
            B before

_end:
        MOV R7,#1
        SWI 0


.data

time1ms:
        .word 0
        .word 070000000

timespecsec:
	.word 0
	.word 300000000

fileName: .asciz "/dev/mem"
gpioaddr: .word 0x20200         @GPIO Base Address

pinRS:	@ LCD Display RS pin - GPIO25
	.word 8 @ offset to select register
	.word 15 @ bit offset in select register
	.word 25 @ bit offset in set & clear register

pinE:	@ LCD Display E pin - GPIO1
	.word 0 @ offset to select register
	.word 3 @ bit offset in select register
	.word 1 @ bit offset in set & clr register

pinDB4:	@ LCD Display DB4 pin - GPIO12
	.word 4 @ offset to select register
	.word 6 @ bit offset in select register
	.word 12 @ bit offset in set & clr register

pinDB5:	@ LCD Display DB5 pin - GPIO16
	.word 4 @ offset to select register
	.word 18 @ bit offset in select register
	.word 16 @ bit offset in set & clr register

pinDB6:	@ LCD Display DB6 pin - GPIO20
	.word 8 @ offset to select register
	.word 0 @ bit offset in select register
	.word 20 @ bit offset in set & clr register

pinDB7:	@ LCD Display DB7 pin - GPIO21
	.word 8 @ offset to select register
	.word 3 @ bit offset in select register
	.word 21 @ bit offset in set & clr register

pinB19: @ LCD Display B19 button pin - GPIO19 - Start/Pause
    .word 4
    .word 27
    .word 524288

pinB26:  @ LCD Display B26 button pin - GPIO26 - Reset
    .word 8
    .word 18
    .word 67108864