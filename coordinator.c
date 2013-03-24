/* Coordinates the cluster module and the results module */

int analyze_network(network *net, double ghost_const, char *out_dir) {

	int 	nV; /* Shorthand for the number of vertices in the network */
	int 	numCols; /* Shorthand for the number of edges = variables we have */

	tuple	**cluster_scores;
	char	*out_file;

	nV		=	net->num_of_vertices;
	numCols =	nV * (nV - 1) / 2;

	if (init_cluster_scores(&cluster_scores) == 2) {
		return 2;
	}

	if (init_out_file(&out_file, out_dir) == 2) {
		free_and_null(cluster_scores);
		return 2;
	}


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
}
