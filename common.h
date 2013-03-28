#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define FLIP(x) (1-(x))

#define LP_FILE "clustering_solution.lp"

void send_error(int code);
void send_perror(char *func);
