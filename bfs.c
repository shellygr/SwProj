#include "queue.h"
#include "network.h" // in an upper folder

// this file should be called "statistics"

// returns cluster's diameter - longest shortest path
int bfs(network* net, vertex* s, int **edges, int **id, int *cluster_id,int **realEdges,
	int **longest_shortest, tuple ***clusterScores, int* status) {
    edge 	**lst;
    edge 	*currEdge;
    vertex 	*currVertex;
    int 	from, to,size=1,numEdges=0;
    int 	i,currVertexId, newVertexId;   
    int 	distFunc[nV];
    int 	colors[nV];
    int		is_new = FALSE;
    double sum=0;
    
    queue *q = NULL;
    init_queue(q);
    if(q==NULL){
    	*status=2;
    	return;    
    }
    
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
    
    
    if (s->cluster_id != -1)
    	is_new = TRUE;
    
    /*if (!(*colors[s->id])) { /* s is not white so make sure not overriding everything*/*/
    enqueue( init_elem( s->id, NULL ) );
    distfunc[s->id] = 0;
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
			if(elm==NULL){
				*status=2;
				return;
			}
		
			if ( edges[id[i][newVertexId-i-1]] == 1 ) {
				numEdges++;
				sum+=currEdge->weight;
				*realEdges[id[i][newVertexId-i-1]] = 1; // why? we will overlap a lot
				if ( (get_vertex(newVertexId, net))->cluster_id != -1 )  // A check for cluster_id existing
					(get_vertex(newVertexId, net))->cluster_id = cluster_id;	
			}
			
			if ( !(colors[newVertexId]) ) {
				  enqueue(elm);
				  size++;
				  if ( edges[id[i][newVertexId-i-1]] == 1 ) {
					colors[newVertexId] = 1; // gray/black
					distfunc[newVertexId] = (distfunc[currVertex]) + 1;
				  }
			}      
	   	}
	if(is_new){
		*clusterScores[cluster_id]=init_tuple(size,sum.place,averageWithin);
		if(*clusterScores[cluster_id]==NULL){
			*status=2;
			return;
		}
	}
    }

    for ( i = 0 ; i < nV ; i++ )
    	if ((get_vertex (i , net))->cluster_id == cluster_id)
    		if ((distfunc[i] > longest_shortest[cluster_id]) || (distfunc[i] == -1)) /* each element of distfunc is a shortest path dist */
    			longest_shortest[cluster_id] = distfunc[i];
    			
    if (is_new)
    	(*cluster_id)++;
  
}

void bfs_all(network *net,int **id, int **edges, int nV, int **realEdges,tuple ***clusterScores,int *status){
	int colors[nV];
	int longest_shortest[nV]; /* Maintains longest shortest paths for each cluster. Max of nV clusters */
	double avgWithin = 0;
	double sumBetween = 0;
	int counterBetween = 0;
	int size = 0;
	
	
	/*tuple clustersScores[nV]; Array of Tuples(not to lose the original ids)
	
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
			bfs(net,get_vertex(net,i),edges,id,&cluster_id,realEdges,&longest_shortest,&clusterScores,status);
			if(*status==2)
				return;
		//}
	}
	// -1 <~> infinity
	
	/* Build array of sorting by scores */	
}
//avg_between[nV], init it
double calc_avg(network* net, double *avg_between, double **avgWithin, int *edges, int **id) {
	int i, j;
	edge **lst;
	vertex *currVertex;
	int other_vertex_id;
	int curr_vertex_id;
	
	for ( i = 0 ; i < net->num_of_vertices ; i++ ) {
		currVertex = get_vertex( i , net );
		lst = currVertex->incoming;
		for ( j = 0  ; j < currVertex->out_deg ; j++ ) {
			other_vertex_id = lst[j]->to;
			if (other_vertex_id < i) {
				curr_vertex_id = other_vertex_id;
				other_vertex_id = i;
			} else {
				curr_vertex_id = i;
			}
			
			if (edges[id[curr_vertex_id][other_vertex_id]] == 1) { // real edge! not ghost
				if (currVertex->cluster_id == get_vertex(other_vertex_id, net)->cluster_id) { //within
					int cluster_id = currVertex->cluster_id;
					
				} else { // between
				}
			}	
		}
		
		
	}
	
}

