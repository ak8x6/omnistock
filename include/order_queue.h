#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "core_types.h"

typedef struct QueueNode {
    Order order;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} OrderQueue;

void init_queue(OrderQueue *q);
void enqueue(OrderQueue *q, Order o);
Order dequeue(OrderQueue *q);
int is_empty_q(OrderQueue *q);
void destroy_queue(OrderQueue *q);

#endif

