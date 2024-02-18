#include <stdio.h>
#include <stdlib.h>
#include "order_queue.h"

void init_queue(OrderQueue *q) {
    q->front = NULL;
    q->rear = NULL;
}

void enqueue(OrderQueue *q, Order o) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        printf("Memory error\n");
        return;
    }
    newNode->order = o;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    q->rear->next = newNode;
    q->rear = newNode;
}

Order dequeue(OrderQueue *q) {
    Order empty = {-1, -1, 0};
    if (q->front == NULL) {
        return empty;
    }

    QueueNode *temp = q->front;
    Order ord = temp->order;

    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return ord;
}

int is_empty_q(OrderQueue *q) {
    return q->front == NULL;
}

void destroy_queue(OrderQueue *q) {
    while (!is_empty_q(q)) {
        dequeue(q);
    }
}

