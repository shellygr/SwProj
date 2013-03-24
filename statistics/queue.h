struct queue {
	elem* head; // The first to go out
	elem* tail; // Last element - inserted first
};
typedef struct queue queue;

struct elem {
	void* content;
	elem* next;
};

typedef struct elem elem;

typedef struct tuple {
	int size;
	double score;
	int place;
	int diameter;
} tuple;

void destroy_elem(elem* elm);
elem* init_elem(void* content, elem* next);

void init_queue(queue *q);
void destroy_queue(queue *q);
elem* dequeue(queue* q);
void enqueue(queue* q, elem* elm);
int is_empty(queue* q);
