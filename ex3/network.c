#include "common.h"

/*	Initializes the pointer to a network struct to a valid initialized network with 0 vertices */
int init_network(network **net) {
	/* Allocate memory for the network struct */
	if ((*net = (network*)malloc(sizeof(network))) == NULL)
		return 2;
	
	/* Initialize the vertices array, and sets it as the network's vertices array */
	if (((*net)->vertices = *init_array(&(*net)->vertices)) == NULL){
		free(net);
		return 2;
	}
	
	(*net)->num_of_vertices = 0;
	return 0;
}

/*	De-allocates memory for the network */
void destroy_net(network *net) {
	// Destroy linked list of vertices one-by-one, each of them
	// needs to destroy a linked list of edges, one-by-one...
	int i;
	
	for (i = 0 ; i < net->num_of_vertices ; i++) {
		destroy_vertex((net->vertices[i]));	
	}
	free(net->vertices);
	free(net);
}

/*  Gets the vertex his ID is id from the vertices array */
vertex* get_vertex(int id, network *net) {
	return (net->vertices[id]);
}

int add_vertex_to_network(network **net, vertex *vrtx) {
	int i;
	//printf("vrtx name is %s\n",vrtx->name);
	// Will double memory for vertices if necessary, should be O(1)
	// therefore realloc + add after tail
	i= double_and_add(&((*net)->vertices), &((*net)->num_of_vertices), vrtx);
	return i;
	
}

void print_edges(network *net) {
	vertex **v = (vertex **)net->vertices;
	int has_edges = FALSE;

	// Search for edges:
	for (int i = 0 ; i < net->num_of_vertices ; i++) {
		if (v[i]->in_deg > 0) {
			has_edges = TRUE;
			break;
		}
	}

	if (has_edges) {
		printf("edges:\n");
		for (int i = 0 ; i < net->num_of_vertices ; i++) {
			edge **arr_edges = (edge **)v[i]->incoming;
			for (int j = 0 ; j < v[i]->in_deg ; j++) {
				printf("%s %s\n", v[arr_edges[j]->from]->name, v[arr_edges[j]->to]->name);
			}

		}
	}
}

void get_prices_array(int nV,network *net,double *prices){
	int i,j,k;
	vertex currVertex;
	int counter=0;
	vertex *vertices=(vertex*)(net->vertices);
	vertex from,to;
	edge *currNeighList;
	for(int i=0;i<net->num_of_vetrtices;i++){
		currVertex=vertices[i];
		currNeighList=currVertex.incoming
		for(k=0;k<currVertex.in_deg;k++){
			from=currNeightList[k]->from;to=currNeightList[k]->to;
			if(from.id==i&&to.id>i){
				prices[counter+to.id-i]=currNeighList[k]->weight;
			}else
			if(to.id==i&&from.id>i){
				prices[counter+from.id-i]=currNeighList[k]->weight;
			}
		}
		counter+=(nV-i-1);
	}
}

void get_id_array(int nV,network *net,int *id){
	int i,j;
	int counter=0;
	for(i=0;i<;i++){
		for(j=(i+1);j<nV;j++){
			if[counter++]=counter;
		}
	}
}

/*
void print(network *net, int with_pr) {
	vertex **v = (vertex **)net->vertices;

	if (net->num_of_vertices > 0) {
		
		printf("%d vertices:\n", net->num_of_vertices);
		for (int i = 0 ; i < net->num_of_vertices ; i++) {
			if (with_pr)
				printf("%d: %s %5.3f\n", i, ((vertex*)v[i])->name, v[i]->PR);
			else
				printf("%d: %s\n", i, ((vertex*)v[i])->name);
		}
		print_edges(net);
	} else
		printf("\n");
}

double page_rank_for_vertex(network *net, vertex *vrtx) {
	double pr = 0.0;
	double sum = 0.0;

	for (int i = 0 ; i < vrtx->in_deg ; i++) {
		int id = ((edge *)(vrtx->incoming[i]))->from;
		sum+= ((vertex *)(net->vertices[id]))->PR / ((vertex *)(net->vertices[id]))->out_deg;
				
	}
	
	pr = (1-DAMP) + DAMP*sum;

	return pr;
}

void page_rank(network *net, int to_recalc) {
	double max_change;
	int iterations = 0;
	int to_stop = FLIP(to_recalc);
	double old_pr,new_pr,change=0.0;

	while (!to_stop) {
		max_change = 0;
		for (int i = 0 ; i < net->num_of_vertices ; i++) {
			vertex *current = (net->vertices[i]);
			old_pr = current->PR;
			new_pr = page_rank_for_vertex(net, current);
			change = fabs((new_pr-old_pr)/old_pr);
			if (change > max_change)
				max_change = change;
			current->PR = new_pr;
		}

		iterations++;

		to_stop = (iterations > MIN_ITERATIONS) || (max_change < MAX_CHANGE_PERCENTAGE);

	}
	
	print(net, TRUE);
}
*/
