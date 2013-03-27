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

Cluster: cluster_editing.o
	gcc -o Cluster cluster_editing.o $(CFLAGS) $(LDFLAGS)

cluster_editing.o: cluster_editing.h cluster_editing.c
	gcc -c cluster_editing.c $(CFLAGS)