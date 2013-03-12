#include "queue.h"

void destroy_elem(elem* elm) {
  free(elm);  
}

elem* init_elem(void* content, elem* next) {
  elem *elm = (elem*)malloc(sizeof(elem));
  elm->content = content;
  elm->next = next;  
  return elm;
}

void init_queue(queue *q) {
  queue *q = (queue*)malloc(sizeof(queue));
  q->head = NULL;
  q->tail = NULL;
  return q;
}

void destroy_queue(queue *q) {
  elem *current = q->head;
  elem *nxt = current->next;
  while (nxt != NULL) {
    free(current);
    current = nxt;
    nxt = current->next;
  }
  free(current);
  
  free(q);
  
}

/* Return null when queue is empty */
elem* dequeue(queue* q) {
  elem *dequeued;
  
  if (is_empty(q)) {
    return NULL;    
  }
  
  dequeued = q->head;
  q->head = (q->head)->next;
  
  return dequeued;
  
}

void enqueue(queue* q, elem* elm) {
    
  (q->tail)->next = elm;
  q->tail = elm;
  
}

int is_empty(queue* q) {
  
 if ((head == tail) && (head == NULL)) 
   return TRUE;
 return FALSE;
 
}