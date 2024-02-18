#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory_bst.h"

BSTNode* createNode(Product p) {
    BSTNode *newNode = (BSTNode *)malloc(sizeof(BSTNode));
    if (newNode == NULL) {
        printf("Memory error\n");
        return NULL;
    }
    newNode->prod = p;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

BSTNode* insertRec(BSTNode *root, Product p) {
    if (root == NULL)
        return createNode(p);

    if (p.id < root->prod.id)
        root->left = insertRec(root->left, p);
    else if (p.id > root->prod.id)
        root->right = insertRec(root->right, p);

    return root;
}

void insert(InventoryTree *tree, Product p) {
    tree->root = insertRec(tree->root, p);
}

BSTNode* search(BSTNode *root, int id) {
    if (root == NULL)
        return NULL;

    if (root->prod.id == id)
        return root;

    if (id < root->prod.id)
        return search(root->left, id);

    return search(root->right, id);
}

void display_inorder(BSTNode *root) {
    if (root == NULL)
        return;

    display_inorder(root->left);
    printf("ID: %d | Name: %s | Price: $%.2f | Stock: %d\n",
           root->prod.id, root->prod.name,
           root->prod.price, root->prod.qty);
    display_inorder(root->right);
}

void destroy_tree(BSTNode *root) {
    if (root != NULL) {
        destroy_tree(root->left);
        destroy_tree(root->right);
        free(root);
    }
}

