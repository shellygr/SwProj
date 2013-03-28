#include "common.h"
#include "structs.h"
#include "queue.h" // TODO don't forget to separate tuple! which is the only one needed, especially since no need to include statistics folder in makefile
#include "io.h"

int results_writer(network *net, int *edges, double ghost_const,
		double optimized_score, int num_of_clusters, double avg_within, double avg_between,
		tuple **cluster_scores, char *out_dir);
int output(network *net, int *edges, double ghost_const,
		double optimized_score, int num_of_clusters, double avg_within, double avg_between, tuple **scores, char *out_dir);
void print_edges_for_solution(FILE *file, network *net);

// check fprintf!
void print_network(FILE *file, network *net) {
	vertex **v = (vertex **) net->vertices;

	fprintf(file, "%d vertices:\n", net->num_of_vertices);
	for (int i = 0; i < net->num_of_vertices; i++) {
		fprintf(file, "%s %d\n", ((vertex*) v[i])->name, v[i]->cluster_id);
	}
	print_edges_for_solution(file, net);
}

//TODO  that's all edges I think!!
void print_edges_for_solution(FILE *file, network *net) {
	vertex **v = (vertex **) net->vertices;
	int edges_count = 0;
	int i;

	// Search for edges:
	for (i = 0; i < net->num_of_vertices; i++) {
		edges_count += v[i]->deg;
	}

	if (edges_count > 0) { // include count of edges
		fprintf(file, "%d edges:\n", edges_count);
		for (int i = 0; i < net->num_of_vertices; i++) {
			edge **arr_edges = (edge **) v[i]->adjacency_list;
			for (int j = 0; j < v[i]->deg; j++) {
				int other_vertex_id;
				if (arr_edges[j]->from == i) {
					other_vertex_id = arr_edges[j]->to;
				} else {
					other_vertex_id = arr_edges[j]->from;
				}
				if (i < other_vertex_id) {
					fprintf(file, "%s-%s %1.3lf\n", v[i]->name,
							v[other_vertex_id]->name, arr_edges[j]->weight);
				}
			}
		}
	}
}

int print_newline(FILE *file) {
	if (fprintf(file, "\n") < 0) {
		send_perror("fprintf");
		return 2;
	}
	return 0;
}

// make sure network has vertices, or don't create files.
int results_writer(network *net, int *edges, double ghost_const,
		double optimized_score, int num_of_clusters, double avg_within, double avg_between,
		tuple **cluster_scores, char *out_dir) {
	FILE *results;
	int i;
	char *filename = get_full_path(out_dir, RESULTS_FILE);

	if (filename == NULL) {
		return 2;
	}

	results = fopen(filename, "w");

	if (fprintf(results, "%s\n", RESULTS_FILE_HEADER) < 0) {
		send_perror("fprintf");
		return 2;
	}

	if (fprintf(results, "score: %1.3lf\n", optimized_score) < 0) {
		send_perror("fprintf");
		return 2;
	}

	if (fprintf(results, "number of clusters: %d\n", num_of_clusters)
			< 0) {
		send_perror("fprintf");
		return 2;
	}

	if (fprintf(results, "cost for ghost: %1.3lf\n", ghost_const) < 0) {
		send_perror("fprintf");
		return 2;
	}

	if (print_newline(results) == 2) {
		return 2;
	}

	if (fprintf(results, "The clustered network\n") < 0) {
		send_perror("fprintf");
		return 2;
	}

	// TODO include edges!
	print_network(results, net);

	if (print_newline(results) == 2) {
		return 2;
	}

	if (fprintf(results, "Clustering statistics for the %d clusters:\n", num_of_clusters) < 0) {
		send_perror("fprintf");
		return 2;
	}

	if (fprintf(results, "Average weight of an edge within clusters: %1.3lf\n", avg_within) < 0) {
		send_perror("fprintf");
		return 2;
	}

	if (fprintf(results, "Average weight of an edge between clusters: %1.3lf\n", avg_between) < 0) {
			send_perror("fprintf");
			return 2;
	}

	for (i = 0; i < num_of_clusters; i++) {
		double score = cluster_scores[i]->score;
		int diameter = cluster_scores[i]->diameter;
		if (fprintf(results, "Cluster %d score: %1.3f diameter: %d\n", i, score, diameter) < 0) {
			send_perror("fprintf");
			return 2;
		}
	}


	if (fclose(results) < 0) {
		send_perror("fclose");
		return 2;
	}

	return 0;

}

char* get_full_path(char *out_dir, char *filename) {
	char *full_path = (char *) malloc(strlen(out_dir) + strlen(filename) + 1);

	if (full_path == NULL) {
		send_perror("malloc");
		return NULL;
	}

	if (sprintf(full_path, "%s%s", out_dir, filename) < 0) {
		send_perror("sprintf");
		return NULL;
	}

	return full_path;
}

int output(network *net, int *edges, double ghost_const,
		double optimized_score, int num_of_clusters, double avg_within, double avg_between, tuple **scores, char *out_dir) {

	if (net->num_of_vertices == 0) {
		printf("The network has no vertices. Exiting.\n");
		return 1;
	} else {
		if (results_writer(net, edges, ghost_const,
		optimized_score, num_of_clusters, avg_within, avg_between, scores, out_dir) == 2) {
			return 2;
		}

		// TODO why ** for edges? no need to change it after all!
		if (cluster_sol_writer(net, &edges, out_dir, num_of_clusters) == 2) {
			return 2;
		}

		if (best_solution_writer(net, &edges, out_dir) == 2) {
			return 2;
		}
	}


	return 0;

}
