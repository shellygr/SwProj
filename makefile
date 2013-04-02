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
	rm *.o Cluster

Cluster: common.h structs.h main.o errors.o commands.o ui.o coordinator.o network.o
	gcc -I bin -o Cluster main.o errors.o commands.o ui.o coordinator.o network.o vertex.o edge.o cluster_editing.o statistics.o queue.o tuple.o $(LDFLAGS)

main.o: common.h structs.h errors.o
	gcc -c main.c $(CFLAGS)
	
commands.o: common.h structs.h network.o vertex.o edge.o
	gcc -I . -c operation/commands.c 
	
errors.o: common.h structs.h 
	gcc -I . -c operation/errors.c 
	
ui.o: common.h structs.h 
	gcc -I . -c operation/ui.c

coordinator.o: cluster_editing.h common.h statistics.h cluster.o statistics.o
	gcc -c -I ./statistics coordinator.c

cluster.o: cluster_editing.h tuple.h common.h structs.h cluster_editing.o tuple.o
	gcc -I . -I ./statistics -c cluster/cluster.c $(CFLAGS) $(LDFLAGS)

cluster_editing.o: common.h structs.h cluster/cluster_editing.h statistics/statistics.h  
	gcc -I . -I ./statistics -c cluster/cluster_editing.c  $(CFLAGS) $(LDFLAGS)
	
queue.o: statistics/statistics.h common.h 
	gcc -c -I . statistics/queue.c

statistics.o: tuple.h statistics/statistics.h common.h structs.h queue.o
	gcc -c -I . statistics/statistics.c 
	
tuple.o: tuple.h common.h
	gcc -c -I . tuple.c
	
xml_writer.o: xml.h io.h common.h structs.h
	gcc -c -I . results/xml_writer.c
	
results_files_writer.o: xml.h io.h common.h structs.h statistics.h
	gcc -c -I . -I ./statistics results/results_files_writer.c
	
network.o: structs.h dynamic_array.o vertex.o
		 gcc -I . -c network/network.c

vertex.o: structs.h dynamic_array.o edge.o
		 gcc -I . -c network/vertex.c
	
edge.o: structs.h
		 gcc -I . -c network/edge.c

dynamic_array.o: structs.h
		 gcc -I . -c network/dynamic_array.c