#include <stdio.h>
#include <stdlib.h>
#include "undo_stack.h"

void init_stack(UndoStack *s) {
    s->top = NULL;
}

void push(UndoStack *s, Order o) {
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Memory error\n");
        return;
    }

    newNode->order = o;
    newNode->next = s->top;
    s->top = newNode;
}

Order pop(UndoStack *s) {
    Order empty = {-1, -1, 0};
    if (is_empty_s(s)) {
        return empty;
    }

    StackNode *temp = s->top;
    Order ord = temp->order;

    s->top = s->top->next;
    free(temp);

    return ord;
}

int is_empty_s(UndoStack *s) {
    return s->top == NULL;
}

void destroy_stack(UndoStack *s) {
    while (!is_empty_s(s)) {
        pop(s);
    }
}

