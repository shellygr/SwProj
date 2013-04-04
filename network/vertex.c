#include "common.h"

// Vertex

void destroy_vertex(vertex *vrtx) {

	if (vrtx->deg > 0) {
		int j;
		for (j = 0; j < vrtx->deg; j++)
			free(vrtx->adjacency_list[j]);
	}

	free(vrtx->adjacency_list);
	free(vrtx->name);
	free(vrtx);
}

vertex* init_vertex(char *name, int id) {
	vertex *vrtx = (vertex*) malloc(sizeof(vertex));
	if (vrtx == NULL) {
		send_perror("malloc");
		return NULL;
	}
	vrtx->id = id;
	if (init_array(&vrtx->adjacency_list) == NULL) {
		destroy_vertex(vrtx);
		return NULL;
	}
	if ((vrtx->name = malloc(MAX_COMMAND_LENGTH - 11)) == NULL) { // "add_vertex " is 11 chars
		destroy_vertex(vrtx);
		send_perror("malloc");
		return NULL;
	}
	strncpy(vrtx->name, name, strlen(name));
	vrtx->name[strlen(name)] = '\0';
	vrtx->deg = 0;
	vrtx->cluster_id = -1;

	return vrtx;
}

int double_vertices(vertex ***v, size_t current_size) {
	vertex **tmp;

	if ((tmp = realloc(*v, 2 * current_size * sizeof(vertex*))) == NULL) {
		send_perror("realloc");
		return 2;
	} else {
		*v = tmp;
	}
	return 0;
}

int add_edge_to_vertex(vertex **src_vrtx, vertex **dest_vrtx, edge *e) {
	//edge **in = (edge **)((*dest_vrtx)->incoming);
	int status;

	if (check_edge_exists(*src_vrtx, *dest_vrtx, e) == 1) {
		send_error(9);
		free(e);
		return 1;
	}

	status = double_and_add(&((*dest_vrtx)->adjacency_list), &((*dest_vrtx)->deg), e)
					|| double_and_add(&((*src_vrtx)->adjacency_list), &((*src_vrtx)->deg), e);
//
//	(*src_vrtx)->deg++;
//	(*dest_vrtx)->deg++;

	return status;
}

