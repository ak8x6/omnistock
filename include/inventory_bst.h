#ifndef INVENTORY_BST_H
#define INVENTORY_BST_H

#include "core_types.h"

typedef struct BSTNode {
    Product prod;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct {
    BSTNode *root;
} InventoryTree;

BSTNode* createNode(Product p);
void insert(InventoryTree *tree, Product p);
BSTNode* search(BSTNode *root, int id);
void display_inorder(BSTNode *root);
void destroy_tree(BSTNode *root);

#endif

