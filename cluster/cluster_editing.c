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

#include "common.h"
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
int cluster(network *net, int **result, int **IDs, int *status, double C,
		char *out_file, double *objval) {

	int nV = net->num_of_vertices;
	int numCols = nV * (nV - 1) / 2;
	int numRows = numCols * (nV - 2);
	int counter, i, j, k, conNumbering;

	int **offset;
	int **conIndices;

	/* Declare pointers for the variables and arrays that will contain
	 the data which define the LP problem. */

	char *probname = NULL;

	/* Declare and allocate space for the variables and arrays where we
	 will store the optimization results including the status, objective
	 value and variable values. */

	CPXENVptr p_env = NULL;
	CPXLPptr p_lp = NULL;

	/* CPlex data structures */
	int ids[numCols];
	char ctypes[numCols];
	int objsen = CPX_MAX;
	double prices[numCols], lb[numCols], ub[numCols]; /* Objective coeffs*/
	double rhs[numRows]; /* Target 1-1-1-1-1 */
	char sense[numRows]; /* "<= "=='L' */
	int matbeg[numCols], matind[numCols * 3 * (nV - 2)], matcnt[numCols];
	double matval[numCols * 3 * (nV - 2)];
	double x[numCols];
	int *results = *result;
	/*int results[numCols];*/
	//double objval;
	int solstat;

	/* Initialize the CPLEX environment */
	p_env = CPXopenCPLEX(status);

	/* If an error occurs, the status value indicates the reason for
	 failure.  A call to CPXgeterrorstring will produce the text of
	 the error message. Note that CPXopenCPLEX produces no output,
	 so the only way to see the cause of the error is to use
	 CPXgeterrorstring. For other CPLEX routines, the errors will
	 be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON.  */

	if (p_env == NULL) {
		char errmsg[1024];
		fprintf(stderr, "Error: Could not open CPLEX environment.\n");
		CPXgeterrorstring(p_env, *status, errmsg);
		fprintf(stderr, "%s", errmsg);
		goto TERMINATE;
	}

	/* Turn on output to the screen */
	*status = CPXsetintparam(p_env, CPX_PARAM_SCRIND, CPX_ON);
	if (*status) {
		fprintf(stderr,
				"Error: Failure to turn on screen indicator, error %d.\n",
				*status);
		goto TERMINATE;
	}

	/* Create the problem. */
	p_lp = CPXcreateprob(p_env, status, probname);

	/* A returned pointer of NULL may mean that not enough memory
	 was available or there was some other problem.  In the case of
	 failure, an error message will have been written to the error
	 channel from inside CPLEX. The setting of
	 the parameter CPX_PARAM_SCRIND causes the error message to
	 appear on stdout.  */

	if (p_lp == NULL) {
		fprintf(stderr, "Error: Failed to create problem.\n");
		goto TERMINATE;
	}

	/* Use CPXcopylp to transfer the ILP part of the problem data into the cplex pointer lp */

	/*MUST BE FREED LATER*/
	offset = (int**) malloc(nV * sizeof(int*));
	conIndices = (int**) malloc(nV * sizeof(int*));

	if ((conIndices == NULL) || (offset == NULL)) {
		free_array((void ***)&offset, nV);
		free_array((void ***)&conIndices, nV);
		goto TERMINATE;
	}

	for (i = 0; i < nV; i++) {
		/*IDs[i]=malloc((nV-i-1)*sizeof(int));*/ //possibly out of this function
		conIndices[i] = (int*) malloc(3 * (nV - i - 1) * sizeof(int));
		offset[i] = (int*) malloc((nV - i - 1) * sizeof(int)); /*WAS:malloc(3*(nV-2)*size(int)).We need offset index for each variable Xij, i<j*/
		if ((conIndices[i] == NULL) || (offset[i] == NULL)) {
			free_array((void ***)&offset, i);
			free_array((void ***)&conIndices, i);
			goto TERMINATE;
		}
	}

	for (i = 0; i < nV; i++) {
		for (j = (i + 1); j < nV; j++) {
			offset[i][j] = 0;
			conIndices[i][j] = counter;
			counter += 3 * (nV - 2);
		}
	}
	if (counter != numRows) {
		printf(
				"DEBUG : Expected counter == numRow, but counter = %d, numRows = %d",
				counter, numRows);
	}

	// Build the data structures

	for (i = 0; i < numCols; i++) {
		lb[i] = 0;
		ub[i] = 2;
	}

	for (i = 0; i < numRows; i++) {
		rhs[i] = 1;
		sense[i] = 'L';
	}

	for (i = 0; i < numCols; i++) {
		prices[i] = -C; /*Initialize to -C(C is given in input), the rest will be generated by a network function, integrate into matbeg*/
	}
	get_prices_array(net, prices); // Check if all right.

	conNumbering = 0;

	for (i = 0; i < nV; i++) {
		for (j = (i + 1); j < nV; j++) {
			matbeg[IDs[i][j - i - 1]] = conIndices[i][j];
			matcnt[IDs[i][j - i - 1]] = 3 * (nV - 2);
		}
	}

	for (i = 0; i < nV; i++) {
		for (j = 0; j < nV - i - 1; j++) {
			for (k = 0; k < nV - j - 2; k++) {
				//building the constraint Xij-Xjk+Xik<=1
				matind[conIndices[i][j] + offset[i][j]] = conNumbering;
				matval[conIndices[i][j] + (offset[i][j]++)] = 1;
				matind[conIndices[j][k] + offset[j][k]] = conNumbering;
				matval[conIndices[j][k] + (offset[j][k]++)] = -1;
				matind[conIndices[i][k] + offset[i][k]] = conNumbering++;
				matval[conIndices[i][k] + (offset[i][k]++)] = 1;

				//building the constraint -Xij+Xjk+Xik<=1
				matind[conIndices[i][j] + offset[i][j]] = conNumbering;
				matval[conIndices[i][j] + (offset[i][j]++)] = -1;
				matind[conIndices[j][k] + offset[j][k]] = conNumbering;
				matval[conIndices[j][k] + (offset[j][k]++)] = 1;
				matind[conIndices[i][k] + offset[i][k]] = conNumbering++;
				matval[conIndices[i][k] + (offset[i][k]++)] = 1;

				//building the constraint Xij+Xjk-Xik<=1
				matind[conIndices[i][j] + offset[i][j]] = conNumbering;
				matval[conIndices[i][j] + (offset[i][j]++)] = 1;
				matind[conIndices[j][k] + offset[j][k]] = conNumbering;
				matval[conIndices[j][k] + (offset[j][k]++)] = 1;
				matind[conIndices[i][k] + offset[i][k]] = conNumbering++;
				matval[conIndices[i][k] + (offset[i][k]++)] = -1;
			}
		}
	}

	CPXcopylp(p_env, p_lp, numCols, numRows, objsen, objval, rhs, sense, matbeg,
			matcnt, matind, matval, lb, ub, NULL);

	counter = 0;
	for (i = 0; i < nV; i++) {
		for (j = 0; j < nV - i - 1; j++) {
			ids[counter++] = IDs[i][j]; // isn't that ids[i] = i?
		}
	}

	for (i = 0; i < numCols; i++) {
		ctypes[i] = CPX_BINARY;
	}

	CPXchgctype(p_env, p_lp, numCols, ids, ctypes);

	/* Optimize the problem. */
	*status = CPXmipopt(p_env, p_lp);
	if (status) {
		fprintf(stderr, "Error: Failed to optimize problem.\n");
		goto TERMINATE;
	}

	CPXsolution(p_env, p_lp, &solstat, objval, x, NULL, NULL, NULL);

	for (i = 0; i < numCols; i++) {
		if (IS_VALUE_1(x[i]))
			results[i] = 1;
		else
			results[i] = 0;
	}

	/* Write a copy of the problem to a file.
	 Please put into probname the following string: Output Directory + "clustering_solution.lp" to create clustering_solution.lp in your output directory */
	*status = CPXwriteprob(p_env, p_lp, probname, out_file);
	if (status) {
		fprintf(stderr, "Error: Failed to write LP to disk.\n");
		goto TERMINATE;
	}

	TERMINATE:

	/* Free up the problem as allocated by CPXcreateprob, if necessary */
	if (p_lp != NULL) {
		*status = CPXfreeprob(p_env, &p_lp);
		if (status) {
			fprintf(stderr, "Error: CPXfreeprob failed, error code %d.\n",
					*status);
		}
	}

	/* Free up the CPLEX environment, if necessary */
	if (p_env != NULL) {
		*status = CPXcloseCPLEX(&p_env);

		/* Note that CPXcloseCPLEX produces no output,
		 so the only way to see the cause of the error is to use
		 CPXgeterrorstring.  For other CPLEX routines, the errors will
		 be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON. */

		if (status) {
			char errmsg[1024];
			fprintf(stderr, "Could not close CPLEX environment.\n");
			CPXgeterrorstring(p_env, *status, errmsg);
			fprintf(stderr, "%s", errmsg);
		}
	}

	/* Free up the problem data arrays, if necessary. */
	free_array((void ***)&offset, nV);
	free_array((void ***)&conIndices, nV); // TODO all occurences are ints so convert prototype to int***

	return 0;
}


