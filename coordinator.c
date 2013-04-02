/* Coordinates the cluster module and the results module */
#include "common.h"
#include "tuple.h"
#include "statistics.h"

int nV;

int analyze_network(network *net, double ghost_const, char *out_dir) {

	int 	numCols =	nV * (nV - 1) / 2; /* Shorthand for the number of edges = variables we have */
	char	*out_file;
	int		num_of_clusters;
	double	avg_within, avg_between;
	tuple	**cluster_scores;
	int 	realEdges[numCols]; // only edges that existed before, no ghosts
	double	optimized_score;

	nV		=	net->num_of_vertices; /* Shorthand for the number of vertices in the network */

	if (init_cluster_scores(&cluster_scores) == 2) {
		return 2;
	}

	if (init_out_file(&out_file, out_dir) == 2) {
		free_and_null(cluster_scores);
		return 2;
	}

	update_globals(nV, numCols); // curious if that works!

	if (solver(net, ghost_const, out_file, &avg_within, &avg_between, &num_of_clusters, cluster_scores, &realEdges, &optimized_score) == 2) {
		free_and_null(cluster_scores);
		free_and_null(out_file);
		return 2;
	}

	if(output(net, realEdges, ghost_const,
			optimized_score, num_of_clusters, avg_within, avg_between, *out_dir) == 2) {
		free_and_null(cluster_scores);
		free_and_null(out_file);
		return 2;
	}

	free_and_null(cluster_scores);
	free_and_null(out_file);
	return 0;
}


int init_cluster_scores(tuple ***cluster_scores) {
	*cluster_scores = (tuple**) malloc(nV * sizeof(tuple*));
	if (*cluster_scores == NULL) {
		send_perror("malloc");
		return 2;
	}
	return 0;
}

int init_out_file(char **out_file, char *out_dir) {
	*out_file = (char *) malloc(strlen(out_dir) + strlen(LP_FILE) + 1);
	if (*out_file == NULL) {
		send_perror("malloc");
		return 2;
	}
	if (sprintf(*out_file, "%s%s", out_dir, LP_FILE) < 0) {
		send_perror("sprintf");
		return 2;
	}
	return 0;
}
