#ifndef UNDO_STACK_H
#define UNDO_STACK_H

#include "core_types.h"

typedef struct StackNode {
    Order order;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
} UndoStack;

void init_stack(UndoStack *s);
void push(UndoStack *s, Order o);
Order pop(UndoStack *s);
int is_empty_s(UndoStack *s);
void destroy_stack(UndoStack *s);

#endif

