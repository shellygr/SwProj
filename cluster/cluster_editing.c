/*------------------------------------------------------------------------*/
/*  File: cluster.c                                                       */
/*  Version 1.0                                                           */
/*------------------------------------------------------------------------*/
/*  Written for Software Project Class.                                   */
/*  Based uopn qpex1.c                                                    */
/*  Permission is expressly granted to use this example in the            */
/*  course of developing applications that use ILOG products.             */
/*------------------------------------------------------------------------*/

/* cluster.c - Entering and optimizing k-clustering integer linear programming problem */
/* You may and should alter the file */

#include "cluster_editing.h"

/* This routine initializes the cplex enviorement, sets screen as an output for cplex errors and notifications, 
   and sets parameters for cplex. It calls for a mixed integer program solution and frees the environment.
   To Do:
   Declare the parameters for the problem and fill them accordingly. After creating the program thus, copy it into cplex. 
   Define any integer or binary variables as needed, and change their type before the call to CPXmipopt to solve problem. 
   Use CPXwriteprob to output the problem in lp format, in the name of <k>.lp.
   Read solution (both objective function value and variables assignment). 
   Communicate to pass the problem and the solution between the modules in the best way you see. 
*/
int cluster()
{

   /* Declare pointers for the variables and arrays that will contain
      the data which define the LP problem. */

   char     *probname = NULL;  
   
   /* Declare and allocate space for the variables and arrays where we
      will store the optimization results including the status, objective
      value and variable values. */

   CPXENVptr p_env              = NULL;
   CPXLPptr  p_lp               = NULL;
   int       status;
   
   /* Initialize the CPLEX environment */
   p_env = CPXopenCPLEX (&status);

   /* If an error occurs, the status value indicates the reason for
      failure.  A call to CPXgeterrorstring will produce the text of
      the error message. Note that CPXopenCPLEX produces no output,
      so the only way to see the cause of the error is to use
      CPXgeterrorstring. For other CPLEX routines, the errors will
      be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON.  */

   if ( p_env == NULL ) {
   char  errmsg[1024];
      fprintf (stderr, "Error: Could not open CPLEX environment.\n");
      CPXgeterrorstring (p_env, status, errmsg);
      fprintf (stderr, "%s", errmsg);
      goto TERMINATE;
   }

   /* Turn on output to the screen */
   status = CPXsetintparam (p_env, CPX_PARAM_SCRIND, CPX_ON);
   if ( status ) {
      fprintf (stderr, 
		  "Error: Failure to turn on screen indicator, error %d.\n", status);
      goto TERMINATE;
   }

   /* Create the problem. */
   p_lp = CPXcreateprob (p_env, &status, probname);

   /* A returned pointer of NULL may mean that not enough memory
      was available or there was some other problem.  In the case of 
      failure, an error message will have been written to the error 
      channel from inside CPLEX. The setting of
      the parameter CPX_PARAM_SCRIND causes the error message to
      appear on stdout.  */

   if ( p_lp == NULL ) {
      fprintf (stderr, "Error: Failed to create problem.\n");
      goto TERMINATE;
   }

   /* Use CPXcopylp to transfer the ILP part of the problem data into the cplex pointer lp */   
int nV=numOfVertices(network);
int numCols =nV*(nV-1)/2 ,numRows=numCols*(nV-2);
int counter,i,j,k,conNumbering;

int **IDs=malloc(nV*sizeof(int*));
int **offset=malloc(nV*sizeof(int*));
int **conIndices=malloc(nV*sizeof(int*));/*MUST BE FREED LATER*/
if(conIndeces==NULL||offset==NULL||IDs==NULL){
	printf("DRAGONS!!!");
}
for(i=0;i<nV;i++){
	IDs[i]=malloc((nV-i-1)*sizeof(int));
	conIndices[i]=malloc(3*(nV-i-1)*sizeof(int));/*malloc NULL check*/
	offset[i]=malloc(3*(nV-2)*size(int));
}
for(i=0;i<nV;i++)
	for(j=(i+1);j<nV;j++){
		offset[i][j]=0;
		conIndices[i][j]=counter;
		counter+=3*(nV-2);
	}
assert(counter==numRows);


int *ids;
char *ctypes;
int obvsen=CPX_MIN;
double *prices,*lb,*up; /* Objective coeffs*/
double *rhs; /* Target 1-1-1-1-1 */
char *sense; /* "<= "=='L' */
int *matbeg,*matind,*matcnt;
double *matval;

matval=malloc(numCols*3*(nV-2)*sizeof(double));/* was:numRows*sizeof(double)*/
matind=malloc(numCols*3*(nV-2)*sizeof(int));
matcnt=malloc(numCols*sizeof(int));
matbeg=malloc(numCols*sizeof(int));
lb=malloc(numCols*sizeof(dobule));
ub=malloc(numCols*sizeof(dobule));
rhs=malloc(numRows*sizeof(double));
sense=malloc(numRows*sizeof(char));
prices=malloc(numCols*sizeof(double));
get_id_array(nV,&IDs);
get_prices_array(net,&prices);
/*MALLOC CHECKS FOR EVERYTHING!!*/
if(sense==NULL||prices==NULL||rhs==NULL||matbeg==NULL||matind==NULL||matcnt==NULL||matval==NULL||lb==NULL||ub==NULL){

}

for(i=0;i<numCols;i++){
	lb[i]=0;
	ub[i]=2;
}

for(i=0;i<numRows;i++){
	rhs[i]=1;
	sense[i]='L';
}

for(i=0;i<numCols;i++)
	prices[i]=-C/*Initialize to -C(C is given in input), the rest will be generated by a network function, integrate into matbeg*/
}

get_id_array(nV,&IDs);
get_prices_array(net,&prices);


/*
for(i=0;i<nV;i++){
	for(j=(i+1);j<nV;j++){
		matbeg[IDs[i][j-i-1]]=conIndices[i][j];
		matcnt[IDs[i][j-i-1]]=3*(nV-2);
	}
}
*/

conNumbering=0
for(i=0;i<nV;i++){
	for(j=(i+1);j<nV;j++){
		counter=0;/*why do we need this? the loop 3lines above can be used to init matbeg and matcnt*/
		for(k=j+1;k<nV;k++){
			if(k==i||k==j)/*why do we need this check?(the condition never holds...)*/
				continue;
			matbeg[counter]=conIndices[i][j];
			matcnt[counter]=3*(nV-2);			
			
			/*
			//building the constraint Xij-Xjk+Xik<=1
			matind[conIndices[i][j]+offset[i][j]]=conNumbering;
			matval[conIndices[i][j]+(offset[i][j]++)]=1;
			matind[conIndices[j][k]+offset[k][i]]=conNumbering;
			matval[conIndices[j][k]+(offset[k][i]++)]=-1;
			matind[conIndices[i][k]+offset[i][k]]=conNumbering++;
			matval[conIndices[i][k]+(offset[i][k]++)]=1;
			
			//building the constraint -Xij+Xjk+Xik<=1
			matind[conIndices[i][j]+offset[i][j]]=conNumbering;
			matval[conIndices[i][j]+(offset[i][j]++)]=-1;
			matind[conIndices[j][k]+offset[k][i]]=conNumbering;
			matval[conIndices[j][k]+(offset[k][i]++)]=1;
			matind[conIndices[i][k]+offset[i][k]]=conNumbering++;
			matval[conIndices[i][k]+(offset[i][k]++)]=1;
			
			//building the constraint Xij+Xjk-Xik<=1
			matind[conIndices[i][j]+offset[i][j]]=conNumbering;
			matval[conIndices[i][j]+(offset[i][j]++)]=1;
			matind[conIndices[j][k]+offset[k][i]]=conNumbering;
			matval[conIndices[j][k]+(offset[k][i]++)]=1;
			matind[conIndices[i][k]+offset[i][k]]=conNumbering++;
			matval[conIndices[i][k]+(offset[i][k]++)]=-1;
			*/
			
			/*"case" (i,j,k)---->(i,j,k)*/
			matind[conIndices[i][j]+offset[i][j]]=IDs[i][j-i-1];
			matval[conIndices[i][j]+(offset[i][j]++)]=1;
			matind[conIndices[i][j]+offset[i][j]]=IDs[i][j-i-1];
			matval[conIndices[i][j]+(offset[i][j]++)]=1;
			matind[conIndices[i][j]+offset[i][j]]=IDs[i][j-i-1];
			matval[conIndices[i][j]+(offset[i][j]++)]=-1;

			/*"case" (i,j,k)---->(i,k,j)*/
			matind[conIndices[i][k]+offset[i][k]]=IDs[i][k-i-1];
			matval[conIndices[i][k]+(offset[i][k]++)]=-1;
			matind[conIndices[i][k]+offset[i][k]]=IDs[i][k-i-1];
			matval[conIndices[i][k]+(offset[i][k]++)]=1;
			matind[conIndices[i][k]+offset[i][k]]=IDs[i][k-i-1];
			matval[conIndices[i][k]+(offset[i][k]++)]=1;
			
			/*"case" (i,j,k)---->(k,i,j)*/
			matind[conIndices[k][i]+offset[k][i]]=IDs[k][i-k-1];
			matval[conIndices[k][i]+(offset[k][i]++)]=1;
			matind[conIndices[k][i]+offset[k][i]]=IDs[k][i-k-1];
			matval[conIndices[k][i]+(offset[k][i]++)]=-1;
			matind[conIndices[k][i]+offset[k][i]]=IDs[k][i-k-1];
			matval[conIndices[k][i]+(offset[k][i]++)]=1;
					
		}
	}	
}


CPXcopylp(env,lp,numCols,numRows,objsen,obj,rhs,sense,matbeg,matcnt,matind,matval,lb,ub,NULL);
ctypes=malloc(numCols*sizeof(char));
ids=malloc(numCols*size(int));/*Generated by Network.c*/
for(i=0;i<numCols;i++){
	ctypes=CPX_BINARY;
}
CPXchgctype(env,lp,numCols,ids,ctypes);


   /* Optimize the problem. */
   status = CPXmipopt (p_env, p_lp);
   if ( status ) {
      fprintf (stderr, "Error: Failed to optimize problem.\n");
      goto TERMINATE;
   }

   /* Write a copy of the problem to a file. 
      Please put into probname the following string: Output Directory + "clustering_solution.lp" to create clustering_solution.lp in your output directory */
   status = CPXwriteprob (p_env, p_lp, probname, NULL);
   if ( status ) {
      fprintf (stderr, "Error: Failed to write LP to disk.\n");
      goto TERMINATE;
   }
   
   
TERMINATE:

   /* Free up the problem as allocated by CPXcreateprob, if necessary */
   if ( p_lp != NULL ) {
      status = CPXfreeprob (p_env, &p_lp);
      if ( status ) {
         fprintf (stderr, "Error: CPXfreeprob failed, error code %d.\n", status);
      }
   }

   /* Free up the CPLEX environment, if necessary */
   if ( p_env != NULL ) {
      status = CPXcloseCPLEX (&p_env);

      /* Note that CPXcloseCPLEX produces no output,
         so the only way to see the cause of the error is to use
         CPXgeterrorstring.  For other CPLEX routines, the errors will
         be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON. */

      if ( status ) {
         char  errmsg[1024];
         fprintf (stderr, "Could not close CPLEX environment.\n");
         CPXgeterrorstring (p_env, status, errmsg);
         fprintf (stderr, "%s", errmsg);
      }
   }
     
   /* Free up the problem data arrays, if necessary. */
     
   return (status);
}  


/* This simple routine frees up the pointer *ptr, and sets *ptr to NULL */
void free_and_null (char **ptr)
{
   if ( *ptr != NULL ) {
      free (*ptr);
      *ptr = NULL;
   }
} 


