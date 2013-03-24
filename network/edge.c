#include "common.h"
#include "structs.h"

// Edge

void init_edge(edge **e, int from, int to, double weight) {
	if ((*e = malloc(sizeof(edge))) == NULL)
		send_perror("malloc");

	(*e)->from = from;
	(*e)->to = to;
	return;
}

/*
 1 if edge found in edge_list
 0 otherwise
 */
int search_for_edge_in_incoming_edge_list(edge** edge_list, int list_len,
		edge *e) {
	int i;

	for (i = 0; i < list_len; ++i) {
		//printf("list len %d\tedge_list[i]->from = %d edge_list[i]->to = %d",list_len,edge_list[i]->from ,edge_list[i]->to);
		if (((edge_list[i]->from == e->from) && (edge_list[i]->to == e->to))
				|| ((edge_list[i]->from == e->to)
						&& (edge_list[i]->to == e->from))) {
			return 1;
		}
	}

	return 0;

}

/*
 Check if the edge "e" exists, O(min(src.deg,to.deg))
 */
int check_edge_exists(vertex *src, vertex *to, edge *e) {
	vertex *tmp;
	if (src->deg > to->deg) {
		tmp = src;
		src = to;
		to = tmp;
	}
	return search_for_edge_in_incoming_edge_list((edge**) src->adjacency_list,
			src->adjacency_list, e);
}