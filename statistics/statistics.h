struct elem {
	void* content;
	struct elem* next;
};

typedef struct elem elem;

struct queue {
	elem* head; // The first to go out
	elem* tail; // Last element - inserted first
};
typedef struct queue queue;



void destroy_elem(elem* elm);
elem* init_elem(void* content, elem* next);

queue* init_queue();
void destroy_queue(queue *q);
elem* dequeue(queue* q);
void enqueue(queue* q, elem* elm);
int is_empty(queue* q);


