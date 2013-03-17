#include "queue.h"
#include "network.h" // in an upper folder

// returns cluster's diameter - longest shortest path
int bfs(network* net, vertex* s, int **edges, int **id, int cluster_id,int **realEdges,int **colors) {
    edge **lst;
    edge *currEdge;
    vertex *currVertex;
    int from, to;
    int i,currVertexId, newVertexId;   
    
    queue *q = NULL;
    init_queue(q);
        
    // init colors to all white - 0
    // assumed
    
    for(i=0;i<numCols;i++)
    	realEdges[i]=0;
      
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
	if(edges[id[i][newVertexId]]==1)
		realEdges[i]=1;
	
	if(!colors[newVertexId]){
	  enqueue(elm);
	  if(edges[id[i][newVertexId]] == 1){
		colors[newVertexId] = 1; // gray/black	  	
	  }
	}
	(get_vertex(newVertexId, net))->cluster_id = cluster_id;
      
      
      }
    
      
    }
  
}

void bfs_all(network *net, int **edge,int numCols){
	int colors[network->num_of_vertices];
	int realEdges[numCols];
	
	
	
}

