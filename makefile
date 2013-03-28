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

Cluster: main.o commands.o errors.o ui.o cluster_editing.o queue.o statistics.o tuple.o xml_writer.o results_files_writer.o network.o edge.o vertex.o dynamic_array.o common.h structs.h
	gcc -o Cluster main.o commands.o errors.o ui.o cluster_editing.o queue.o statistics.o tuple.o xml_writer.o results_files_writer.o network.o edge.o vertex.o dynamic_array.o common.h structs.h

main.o: common.h structs.h 
	gcc -c main.c $(CFLAGS) $(LDFLAGS)
	
commands.o: common.h structs.h 
	gcc -I . -c operation/commands.c $(CFLAGS) $(LDFLAGS)
	
errors.o: common.h structs.h 
	gcc -I . -c operation/errors.c $(CFLAGS) $(LDFLAGS)
	
ui.o: common.h structs.h 
	gcc -I . -c operation/ui.c $(CFLAGS) $(LDFLAGS)

cluster_editing.o: common.h structs.h cluster/cluster_editing.h statistics/queue.h  
	gcc -I . -I ./statistics -c cluster/cluster_editing.c $(CFLAGS) $(LDFLAGS)
	
queue.o: statistics/queue.h common.h 
	gcc -c -I . statistics/queue.c $(CFLAGS) $(LDFLAGS)

statistics.o: statistics/queue.h common.h structs.h
	gcc -c -I . statistics/statistics.c $(CFLAGS) $(LDFLAGS)
	
tuple.o: statistics/queue.h common.h
	gcc -c -I . statistics/tuple.c $(CFLAGS) $(LDFLAGS)
	
xml_writer.o: xml.h io.h common.h structs.h
	gcc -c -I . results/xml_writer.c $(CFLAGS) $(LDFLAGS)
	
results_files_writer.o: xml.h io.h common.h structs.h queue.h
	gcc -c -I . -I ./statistics results/results_files_writer.c $(CFLAGS) $(LDFLAGS)
	
network.o: structs.h
		 gcc -I . -c network/network.c $(CFLAGS) $(LDFLAGS) 

vertex.o: structs.h
		 gcc -I . -c network/vertex.c $(CFLAGS) $(LDFLAGS)
	
edge.o: structs.h
		 gcc -I . -c network/edge.c $(CFLAGS) $(LDFLAGS)

dynamic_array.o: structs.h
		 gcc -I . -c network/dynamic_array.c $(CFLAGS) $(LDFLAGS)