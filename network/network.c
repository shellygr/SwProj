#include "common.h"
#include "structs.h"

/*	Initializes the pointer to a network struct to a valid initialized network with 0 vertices */
int init_network(network **net) {
	/* Allocate memory for the network struct */
	if ((*net = (network*) malloc(sizeof(network))) == NULL)
		return 2;

	/* Initialize the vertices array, and sets it as the network's vertices array */
	if (((*net)->vertices = *init_array(&(*net)->vertices)) == NULL) {
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

	for (i = 0; i < net->num_of_vertices; i++) {
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
	i = double_and_add(&((*net)->vertices), &((*net)->num_of_vertices), vrtx);
	return i;

}

void get_prices_array(network *net, double *prices) {
	int i, k;
	vertex currVertex;
	int counter = 0;
	vertex *vertices = (vertex*) (net->vertices);
	int from, to;
	edge **currNeighList;

	for (i = 0; i < net->num_of_vertices; i++) {
		currVertex = vertices[i];
		currNeighList = (edge **)currVertex.adjacency_list;
		for (k = 0; k < currVertex.deg; k++) {
			from = currNeighList[k]->from;
			to = currNeighList[k]->to;

			if ((from == i) && (to > i)) {
				prices[counter + to - i] = currNeighList[k]->weight;
			} else {
				if ((to == i) && (from > i)) {
					prices[counter + from - i] = currNeighList[k]->weight;
				}
			}
		}

		counter += (net->num_of_vertices - i - 1);
	}
}

void get_id_array(int nV, int ***id) {
	int i, j;
	int counter = 0;
	for (i = 0; i < nV; i++) {
		for (j = 0; j < nV - i - 1; j++) {
			*id[i][j] = counter++;
		}
	}
}

/*
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
