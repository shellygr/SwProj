#include "queue.h"
#include "network.h" // in an upper folder

// returns cluster's diameter - longest shortest path
int bfs(network* net, vertex* s, int **edges, int **id, int cluster_id,int **realEdges,
	int **longest_shortest, double *avgWithinCluster, int *edgesBetClusters, double *sumBetClusters, int makeChanges,
	int *size) {
    edge 	**lst;
    edge 	*currEdge;
    vertex 	*currVertex;
    int 	from, to;
    int 	i,currVertexId, newVertexId;   
    int 	distFunc[nV];
    int 	colors[nV];
    
    *size = 1; /* 1 for vertex s*/
    
    queue *q = NULL;
    init_queue(q);
    
    /* Init distfunc and colors */
    // init colors to all white - 0
    // I know what vertices already has been assigned a cluster so I'm not afraid of re-runnning BFS on them.
    // I'll just make sure no vertex with a valid cluster_id is assigned a new one.'
    for (i = 0 ; i < nV ; i++) {
    	distfunc[i] = -1;
    	colors[i] = 0;
    }
    
   
    // assumed
    
    for( i = 0 ; i < numCols ; i++ )
    	realEdges[i] = 0;
    
    /*if (!(*colors[s->id])) { /* s is not white so make sure not overriding everything*/*/
    enqueue( init_elem( s->id, NULL ) );
    distfunc[s] = 0;
    currVertexId = s->id;
    
    while ( !is_empty(q) ) {
      	currVertexId = dequeue(q);
      	currVertex = get_vertex( currVertexId, net );
      	lst = currVertex->incoming;
      
		for ( i = 0 ; i < currVertex->out_deg ; i++ ) {
			currEdge = lst[i];
	      		to   = currEdge->to;
			from = currEdge->from;
			if (to == currVertexId) {
		  		newVertexId = from;
			} else {
		  		newVertexId = to;
			}
		
			if ( newVertexId < i )
			  continue;
		      
			elem* elm = init_elem( newVertexId, NULL );
		
			if ( edges[id[i][newVertexId-i-1]] == 1 ) {
				*realEdges[i] = 1;
				if ( (get_vertex(newVertexId, net))->cluster_id != -1 )  // A check for cluster_id existing
					(get_vertex(newVertexId, net))->cluster_id = cluster_id;	
			}
			
			if ( !(*colors[newVertexId]) ) {
				  enqueue(elm);
				  (*size)++;
				  if ( edges[id[i][newVertexId-i-1]] == 1 ) {
					*colors[newVertexId] = 1; // gray/black
					*distfunc[newVertexId] = (*distfunc[currVertex]) + 1;
				  }
			}      
	   	}
	      
    }

    for ( i = 0 ; i < nV ; i++ )
    	if (distfunc[i] > longest_shortest[cluster_id]) /* each element of distfunc is a shortest path dist */
    		longest_shortest[cluster_id] = distfunc[i];
  
}

void bfs_all(network *net,int **id, int **edges,int nV,int **realEdges){
	int colors[nV];
	int longest_shortest[nV]; /* Maintains longest shortest paths for each cluster. Max of nV clusters */
	double avgWithin = 0;
	double sumBetween = 0;
	int counterBetween = 0;
	int size = 0;
	double clustersScores[nV];/*Array of Tuples(not to lose the original ids)*/
	/*
	[6,7,4,5,1,2,3]
	inv_dic=[5,6,7,3,4,1,2]
	v->cluster_id=inv_dic[v->cluster_id];
	*/
	int i,cluster_id=1; /*0 OR 1???*/
	
	/* Init longest shortest path array */
	for (i = 0 ; i < nV ; i++)
		longest_shortest[i] = 0;
	
	/*for(i=0;i<nV;i++){
		colors[i]=0;
	}*/
	
	for(i=0;i<nV;i++){		
		//if(!colors[i]){
			bfs(net,get_vertex(net,i),edges,id,cluster_id++,realEdges,&longest_shortest,&avgWithin,
			&counterBetween,&sumBetween,&size);
		//}
	}
	
	/* Build array of sorting by scores */
	
	
}

