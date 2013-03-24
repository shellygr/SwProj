C_FLAGS = -ansi -Wall -pedantic-errors -std=c99

all: Ex3

clean: 
	rm *.o Ex3

Ex3: main.o
		 gcc $(C_FLAGES) -o Ex3 main.o network.o vertex.o edge.o dynamic_array.o ui.o commands.o errors.o

main.o:	common.h
		 gcc $(C_FLAGS) -c main.c
	
errors.o:
		 gcc $(C_FLAGS) -c errors.c

ui.o: common.h
		 gcc $(C_FLAGS) -c ui.c
	
commands.o: common.h
		 gcc $(C_FLAGS) -c commands.c
	
network.o: common.h
		 gcc $(C_FLAGS) -c network.c

vertex.o: common.h
		 gcc $(C_FLAGS) -c vertex.c
	
edge.o: common.h
		 gcc $(C_FLAGS) -c edge.c

dynamic_array.o: common.h
		 gcc $(C_FLAGS) -c dynamic_array.c
