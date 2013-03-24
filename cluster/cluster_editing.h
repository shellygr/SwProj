#ifndef CLUSTER_EDITING_H
#define CLUSTER_EDITING_H

/* You may and should alter the file */

/* Bring in the CPLEX function declarations and the C library 
   header file stdio.h with include of cplex.h. */
#include <ilcplex/cplex.h>
#include <stdlib.h>

/* Bring in the declarations for the string functions */
#include <string.h>
#include "queue.h"

#define LP_FILE "clustering_solution.lp"
#define IS_VALUE_1(X) ((1 - X) < 0.00001)


/* Include declaration for functions */
void free_and_null (char **ptr);
int  cluster();

#endif
