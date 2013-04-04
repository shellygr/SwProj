#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define FLIP(x) (1-(x))

#define LP_FILE "clustering_solution.lp"
#define WHITESPACES " \n\t\v\f\r"
#define MAX_COMMAND_LENGTH	501 // Including \0
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

void send_error(int code);
void send_perror(char *func);

typedef void** dynamic_array;

struct vertex {
	char *name;
	int id;
	dynamic_array adjacency_list;
	int deg;
	//int		in_deg;
	// Assert incoming length = in_deg
	int cluster_id;
};
typedef struct vertex vertex;

struct edge {
	int from;
	int to;
	double weight;
};

typedef struct edge edge;

struct network {
	int num_of_vertices;
	dynamic_array vertices;
};

typedef struct network network;

struct tuple {
	int size;
	double score;
	int place;
	int diameter;
};

typedef struct tuple tuple;

int analyze_network(network *net, double ghost_const, char *out_dir);

/* results_files_writer.c */
int output(network *net, int *edges, double ghost_const,
		double optimized_score, int num_of_clusters, double avg_within, double avg_between, tuple **scores, char *out_dir);

/* tuple.c */
void* init_tuple(int size, double score, int place);
int compare_to(const void *a, const void *b);

/* statistics.c */
int bfs_all(network *net, int **id, int nV, int *realEdges,
		tuple **clusterScores);
void calc_avg(network* net, double *avg_within, double *avgBetween,
		int *edges, int **id);

/*network.c*/
void destroy_net(network *net);
int init_network(network **net);
int add_vertex_to_network(network **net, vertex *vrtx);
vertex* get_vertex(int id, network *net);
void print_edges(network *net);
void get_prices_array(network *net, double *prices);
void get_id_array(int nV, int ***id);

/*edge.c*/
void init_edge(edge **e, int from, int to, double weight);
int search_for_edge_in_incoming_edge_list(edge** edge_list, int list_len,
		edge *e);
int check_edge_exists(vertex *src, vertex *to, edge *e);

/*dynamic_array.c*/
int double_and_add(dynamic_array *arr, int *arr_size, void *element);
dynamic_array* init_array(dynamic_array *arr);
void* get_element(int index, dynamic_array *arr);
int is_power_two(int x);

/*ui.c*/
char* get_command(int *exit_status, FILE *f);
int check_all_whitespace(char *str);
int get_action(char *command);
char *get_params(char *command, int code);

/*vertex.c*/
void destroy_vertex(vertex *vrtx);
vertex* init_vertex(char *name, int id);
int double_vertices(vertex ***v, size_t current_size);
int add_edge_to_vertex(vertex **src_vrtx, vertex **dest_vrtx, edge *e);

/*commands.c*/
int check_one_param(char *param);
int check_and_get_three_params(char *params, char **first_param,
		char **second_param, char **weight);
int check_string_is_non_negative_integer(char *str);
int check_string_is_double(char *str);
int check_not_max_num_of_vertices(network *net);
int is_legal_id(int id, network *net);
int add_vertex(char *name, network *net);
int add_edge(int first_id, int second_id, double weight, network *net);
int dispatch_add_vertex(char *params, network *net);
int dispatch_add_edge(char *params, network *net);
int dispatch_command(int code, char *params, network *net);
