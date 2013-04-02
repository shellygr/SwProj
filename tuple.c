#include "common.h"
#include "tuple.h"

void* init_tuple(int size, double score, int place) {
	tuple *ret = (tuple *) malloc(sizeof(tuple));
	if (ret == NULL)
		return NULL;
	ret->size = size;
	ret->score = score;
	ret->place = place;
	ret->diameter = 0;
	return ret;
}

int compare_to(const void *a, const void *b) {
	tuple *x = (tuple *) a;
	tuple *y = (tuple *) b;

	/* First criterion: size */
	if ((x->size) > (y->size)) {
		return 1;
	}
	if ((x->size) < (y->size)) {
		return -1;
	}

	/* Second criterion: score */
	if ((x->score) > (y->score)) {
		return 1;
	}
	if ((x->score) < (y->score)) {
		return -1;
	}

	/* Last criterion: original place */
	return (x->place) - (y->place);
}
