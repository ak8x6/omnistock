#ifndef INVENTORY_SORT_H
#define INVENTORY_SORT_H

#include "core_types.h"
#include "inventory_bst.h"

int countNodes(BSTNode *root);
void toArray(BSTNode *root, Product *arr, int *index);
void quickSort(Product *arr, int low, int high);
void display_sorted(BSTNode *root);

#endif

