lib = display

uart: uart.c
	gcc uart.c -o uart.run -lwiringPi $(lib).o

$(lib).o: $(lib).s
	as -g -o $(lib).o $(lib).s
