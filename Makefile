display: display.o
	ld -o display display.o

display.o: display.s
	as -o display.o display.s
