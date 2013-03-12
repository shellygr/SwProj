#include "queue.h"
#include "network.h" // in an upper folder

// returns cluster's diameter - longest shortest path
int bfs(network* net, vertex* s, int **edges, int **id, int cluster_id) {
    edge **lst;
    edge *currEdge;
    vertex *currVertex;
    int from, to;
    int i,currVertexId, newVertexId;
    queue *q = NULL;
    init_queue(q);
    int colors[network->num_of_vertices];
    
    // init colors to all white - 0
    // assumed
    
    
      
    enqueue(s);
    currVertexId=s->id;
    
    while(!is_empty(q)){
      currVertexId=dequeue(q);
      currVertex=get_vertex(currVertexId, net);
      lst=currVertex->incoming;
      
      for(i=0; i<currVertex->out_deg; i++){
	currEdge = lst[i];
      	to   = currEdge->to;
	from = currEdge->from;
	if(to==currVertexId) {
	  newVertexId = from;
	}else{
	  newVertexId = to;
	}
	if(newVertexId<i)
	  continue;
      
	elem* elm = init_elem(newVertexId, NULL);
	if(!colors[newVertexId]){
	  enqueue(elm);
	  if(edges[id[i][newVertexId]] == 1)
	  colors[newVertexId] = 1; // gray/black
	}
	(get_vertex(newVertexId, net))->cluster_id = cluster_id;
      
      
      }
    
      
    }
  
}

void bfs_all(network *net, double **edge)

