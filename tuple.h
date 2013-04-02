struct tuple {
	int size;
	double score;
	int place;
	int diameter;
};

typedef struct tuple tuple;

void* init_tuple(int size, double score, int place);
int compare_to(const void *a, const void *b);
