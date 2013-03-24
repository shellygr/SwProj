#include "queue.h"
#include "common.h" // in an upper folder
// this file should be called "statistics"

void bfs(network* net, vertex* s, int **edges, int **id, int *cluster_id,
		int **longest_shortest, tuple ***clusterScores, int* status) {

	int nV = net->num_of_vertices;
	int numCols = nV * (nV - 1) / 2;

	edge **lst;
	edge *currEdge;
	vertex *currVertex;
	int from, to, size = 1, numEdges = 0;
	int i, currVertexId, newVertexId;
	int distFunc[nV];
	int colors[nV];
	int is_new = FALSE;
	double sum = 0;

	queue *q = NULL;
	init_queue(q);
	if (q == NULL) {
		*status = 2;
		return;
	}

	/* Init distfunc and colors */
	// init colors to all white - 0
	// I know what vertices already has been assigned a cluster so I'm not afraid of re-runnning BFS on them.
	// I'll just make sure no vertex with a valid cluster_id is assigned a new one.'
	for (i = 0; i < nV; i++) {
		distFunc[i] = -1;
		colors[i] = 0;
	}

	if (s->cluster_id != -1) {
		is_new = TRUE;
	}

	/*if (!(*colors[s->id])) { /* s is not white so make sure not overriding everything*/
	enqueue(init_elem(s->id, NULL));
	distFunc[s->id] = 0;
	currVertexId = s->id;

	while (!is_empty(q)) {
		currVertexId = dequeue(q);
		currVertex = get_vertex(currVertexId, net);
		lst = currVertex->adjacency_list;

		for (i = 0; i < currVertex->deg; i++) {
			currEdge = lst[i];
			to = currEdge->to;
			from = currEdge->from;
			if (to == currVertexId) {
				newVertexId = from;
			} else {
				newVertexId = to;
			}

			if (newVertexId < currVertexId) // why? not correct BFS.
				continue;

			elem* elm = init_elem(newVertexId, NULL);
			if (elm == NULL) {
				*status = 2;
				return;
			}

			if (edges[id[currVertexId][newVertexId - currVertexId - 1]] == 1) {
				numEdges++;
				sum += currEdge->weight;
				//*realEdges[id[i][newVertexId - i - 1]] = 1; // why? we will overlap a lot
				if ((get_vertex(newVertexId, net))->cluster_id != -1) // A check for cluster_id existing
					(get_vertex(newVertexId, net))->cluster_id = cluster_id;
			}

			if (!(colors[newVertexId])) {
				enqueue(elm);
				size++;
				if (edges[id[currVertexId][newVertexId - currVertexId - 1]]
						== 1) {
					colors[newVertexId] = 1; // gray/black
					distFunc[newVertexId] = (distFunc[currVertex]) + 1;
				}
			}
		}

		if (is_new) {
			*clusterScores[cluster_id-1] = init_tuple(size, sum, cluster_id);
			if (*clusterScores[cluster_id-1] == NULL) {
				*status = 2;
				return;
			}
		}
	}

	for (i = 0; i < nV; i++)
		if ((get_vertex(i, net))->cluster_id == cluster_id)
			if ((distFunc[i] > longest_shortest[cluster_id])
					|| (distFunc[i] == -1)) /* each element of distfunc is a shortest path dist */
				longest_shortest[cluster_id-1] = distFunc[i];

	if (is_new)
		(*cluster_id)++;

}

// Runs bfs on all vertices to get a correct diameter for each cluster. Also helps to calculate the score and size according to which we sort the clusters.
// Returns the number of clusters
int bfs_all(network *net, int **id, int nV, int *realEdges,
		tuple ***clusterScores, int *status) {

	int longest_shortest[nV]; /* Maintains longest shortest paths for each cluster. Max of nV clusters */
	int size = 0;

	int inv_dic[nV];
	/*tuple clustersScores[nV]; Array of Tuples(not to lose the original ids)

	 [6,7,4,5,1,2,3]
	 inv_dic=[5,6,7,3,4,1,2]
	 v->cluster_id=inv_dic[v->cluster_id];
	 */

	int i, cluster_id = 1; /*0 OR 1???*/

	/* Init longest shortest path array */
	for (i = 0; i < nV; i++)
		longest_shortest[i] = 0;

	for (i = 0; i < nV; i++) {
		bfs(net, get_vertex(net, i), realEdges, id, &cluster_id,
				&longest_shortest, &clusterScores, status);
		if (*status == 2)
			return -1; // should free and clean
	}

	for (i = 0; i < cluster_id; i++) {
		clusterScores[i]->diameter = longest_shortest[i];
	}

	// sort.
	qsort(*clusterScores, cluster_id, sizeof(tuple), compare_to);

	// build inverse dictionary
	for (i = 0; i < cluster_id; i++) {
		inv_dic[clusterScores[i]->place - 1] = i + 1;
	}

	// assign correct cluster_id-s
	for (i = 0; i < nV; i++) {
		vertex *v = get_vertex(i, net);
		v->cluster_id = inv_dic[v->cluster_id - 1];
	}

	return cluster_id;
	// -1 <~> infinity

	/* Build array of sorting by scores */
}

//avg_between[nV], init it
// calcs both within for each cluster separately and overall within, and between.
void calc_avg(network* net, tuple ***clusterScores, double *avg_within, double *avgBetween,
		int **edges, int **id) {
	int i, j;
	int nV = net->num_of_vertices;
	int count_edges[nV]; /* for within clusters */
	int count_between = 0;
	int count_within = 0;
	edge **lst;
	vertex *currVertex;
	int other_vertex_id;
	int curr_vertex_id;

	*avg_within = 0;
	*avgBetween = 0;
	for (i = 0; i < nV; i++) {
		count_edges[i] = 0;
	}

	for (i = 0; i < nV; i++) {
		currVertex = get_vertex(i, net);
		lst = currVertex->adjacency_list;
		for (j = 0; j < currVertex->deg; j++) {
			other_vertex_id = lst[j]->to;

			if (other_vertex_id > i) { // count only when (i,j) is an edge where j > i.
				if (edges[id[curr_vertex_id][other_vertex_id]] == 1) { // real edge! not ghost
					double curr_weight = lst[j]->weight;
					if (currVertex->cluster_id
							== get_vertex(other_vertex_id, net)->cluster_id) { //within
						int cluster_id = currVertex->cluster_id;

						*avg_within = ((*avg_within * (count_within++))
								+ curr_weight) / count_within;

						double old_avg =
								*(*clusterScores[cluster_id])->averageWithin;
						*(*clusterScores[cluster_id])->averageWithin =
								(((old_avg * (count_edges[cluster_id]++))
										+ curr_weight) / count_edges[cluster_id]);
					} else { // between
						*avgBetween = ((*avgBetween * (count_between++))
								+ curr_weight) / count_between;
					}
				}
			}
		}

	}

}

