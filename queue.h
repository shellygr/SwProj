
struct queue {
  elem* head;
  elem* tail;
};
typedef struct queue queue;

struct elem {
  void* content;
  elem* next;
};
typedef struct elem elem;

void destroy_elem(elem* elm);
elem* init_elem(void* content)

