VPATH = operation:results:cluster:statistics:..

CPLEX=cplex125
CPARCH=x86-64_sles10_4.1

CFLAGS+= -ansi -pedantic-errors -Wall -g
CFLAGS+= -I/usr/local/stow/${CPLEX}/lib/${CPLEX}/include
CFLAGS+= -I/usr/include/libxml2
CFLAGS+= -std=c99

LDFLAGS+= -L/usr/local/stow/${CPLEX}/lib/${CPLEX}/lib/${CPARCH}/static_pic -lilocplex -lcplex 
LDFLAGS+= -lm -lpthread -lgraph
LDFLAGS+= -lxml2

all: Cluster

clean:
	rm cluster_editing.o Cluster

Cluster: main.o commands.o errors.o ui.o cluster/cluster_editing.o statistics/queue.o statistics/statistics.o statistics/tuple.o results/xml_writer.o results/results_file_writer.o
	gcc -o Cluster main.o $(CFLAGS) $(LDFLAGS)

main.o: main.c common.h structs.h 
	gcc -c main.c $(C_FLAGS)
	
commands.o: commands.c common.h structs.h 
	gcc -I . -c operation/commands.c $(C_FLAGS)
	
errors.o: operation/errors.c common.h structs.h 
	gcc -I . -c operation/errors.c $(C_FLAGS)
	
ui.o: operation/ui.c common.h structs.h 
	gcc -I . -c operation/ui.c $(CFLAGS)
	
cluster_editing.o: cluster/cluster_editing.h cluster/cluster_editing.c
	gcc -c cluster/cluster_editing.c $(CFLAGS)
	
queue.o: statistics/queue.c statistics/queue.h
	gcc -c statistics/queue.c $(CFLAGS)

statistics.o: statistics/statistics.c statistics/queue.h
	gcc -c statistics/statistics.c $(CFLAGS)
	
tuple.o: statistics/tuple.c statistics/queue.h
	gcc -c statistics/tuple.c $(CFLAGS)
	
xml_writer.o: results/xml_writer.c xml.h io.h
	gcc -c results/xml_writer.c $(CFLAGS)
	
results_file_writer.o: results/results_file_writer.c xml.h io.h
	gcc -c results/results_file_writer.c $(CFLAGS)