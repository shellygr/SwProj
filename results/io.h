#define RESULTS_FILE "results"
#define CLUSTER_SOL "clustering_solution.xgmml"
#define BEST_CLUSTERS "best_clusters.xgmml"

#define MAX_LINE_LENGTH 1024
#define RESULTS_FILE_HEADER "Cluster Editing Optimal Solution"

char* get_full_path(char *out_dir, char *filename);
// TODO these are definetly used
int cluster_sol_writer(network *net, int **edges, char *out_dir, int num_of_clusters);
int best_solution_writer(network *net, int **edges, char *out_dir);
