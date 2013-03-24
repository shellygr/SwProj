#include "common.h"
#include "structs.h"
#include "cluster_editing.h"

int 	nV; /* Shorthand for the number of vertices in the network */
int 	numCols; /* Shorthand for the number of edges = variables we have */

void update_globals(int nV_update, int numCols_update) {
	nV = nV_update;
	numCols = numCols_update;
}

int solver(network *net, double ghost_const, char *out_file, double *avg_within, double *avgBetween, int *num_of_clusters, tuple **cluster_scores, int **realEdges, double *optimized) {
	int		status;
	int 	results[numCols]; // edges indicator - includes ghosts
	int 	**IDs;

	if (init_ids(&IDs) == 2) {
		return 2;
	}

	cluster(net, &results, IDs, &status, ghost_const, out_file, optimized);

	if (status == 2) { // is it 2?
		return 2;
	}

	if (get_statistics(net, avg_within, avgBetween, num_of_clusters, cluster_scores, IDs, realEdges, results) == 2) {
		free_array(&IDs);
		return 2;
	}

	free_array(&IDs);

	return 0;
}

int get_statistics(network *net, double *avg_within, double *avgBetween, int *num_of_clusters, tuple **cluster_scores, int **IDs, int **realEdges, int *results) {
	int i;

	// Get the real edges
	for (i = 0; i < numCols; i++)
		realEdges[i] = 0;
	get_real_edges(net, results, realEdges);

	*num_of_clusters = bfs_all(net, IDs, nV, *realEdges, cluster_scores);

	if (*num_of_clusters == -1) {
		return 2;
	}

	// Set averages.
	*avgBetween = 0;
	*avg_within = 0;
	calc_avg(net, cluster_scores, avg_within, avgBetween, *realEdges, IDs);

	return 0;
}

int init_ids(int ***IDs) {
	int i;

	*IDs = (int**) malloc(nV * sizeof(int*));
	if (*IDs == NULL) {
		send_perror("malloc");
		return 2;
	}

	for (i = 0; i < nV; i++) {
		*IDs[i] = (int*) malloc((nV - i - 1) * sizeof(int));
		if (*IDs[i] == NULL) {
			for (; i >= 0; i--) {
				free(*IDs[i]);
			}
			send_perror("malloc");
			return 2;
		}
	}

	get_id_array(nV, IDs);

	return 0;
}

void get_real_edges(network *net, int *cluster_results, int **realEdges, int **IDs) {
	int i = 0;

	for (i = 0; i < net->num_of_vertices; i++) {
		vertex *v = get_vertex(i, net);
		edge **lst = v->adjacency_list;
		int j;
		for (j = 0; j < v->deg; j++) {
			edge *curr_edge = lst[j];
			printf(
					"asserting that curr_edge->from = i; curr_edge->from = %d, i = %d\n",
					curr_edge->from, i);
			if (curr_edge->to > curr_edge->from) {
				int other_vertex_id = curr_edge->to;
				if (cluster_results[IDs[i][other_vertex_id - i - 1]] == 1) {
					*realEdges[IDs[i][other_vertex_id - i - 1]] = 1
				}
			}
		}

	}

}


/* This simple routine frees up the pointer *ptr, and sets *ptr to NULL */
void free_and_null(char **ptr) {
	if (*ptr != NULL) {
		free(*ptr);
		*ptr = NULL;
	}
}

void free_array(void ***ptr, int size) {
	if (*ptr != NULL) {
		int i;
		for (i = 0; i < size; i++) {
			if (*ptr[i] != NULL) {
				free(*ptr[i]);
				*ptr[i] = NULL;
			}
		}
		*ptr = NULL;
	}
}

