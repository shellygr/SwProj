#include "queue.h"
#include "network.h" // in an upper folder

// returns cluster's diameter - longest shortest path
int bfs(network* net, vertex* s, int **edges, int **id, int cluster_id,int **realEdges,
	int **colors,int **distfunc,double avgWithinCluster,int *edgesBetClusters,double *sumBetClusters,int makeChanges) {
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

	if(edges[id[i][newVertexId-i-1]]==1){
		*realEdges[i]=1;
		(get_vertex(newVertexId, net))->cluster_id = cluster_id;
	
	}
	if(!colors[newVertexId]){
	  enqueue(elm);
	  if(edges[id[i][newVertexId-i-1]] == 1){
		*colors[newVertexId] = 1; // gray/black	  	
	  }
	}      
      
      }
    
      
    }
  
}

void bfs_all(network *net,int **id, int **edges,int nV,int **realEdges){
	int colors[nV];
	int **distfunc;
	double sumBetween;
	int counterBetween;
	double clustersScores[nV];/*Array of Tuples(not to lose the original ids)*/
	/*
	[6,7,4,5,1,2,3]
	inv_dic=[5,6,7,3,4,1,2]
	v->cluster_id=inv_dic[v->cluster_id];
	*/
	int i,cluster_id=1; /*0 OR 1???*/
	
	for(i=0;i<nv;i++){
		colors[i]=0;
	}
	
	for(i=0;i<nV;i++){		
		if(!colors[i]){
			bfs(net,get_vertex(net,i),edges,id,cluster_id++,realEdges,&colors);
		}
	}
	
}

