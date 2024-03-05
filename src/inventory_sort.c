#include <stdio.h>
#include <stdlib.h>
#include "inventory_sort.h"

int countNodes(BSTNode *root) {
    if (root == NULL)
        return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void toArray(BSTNode *root, Product *arr, int *index) {
    if (root == NULL)
        return;

    toArray(root->left, arr, index);
    arr[*index] = root->prod;
    (*index)++;
    toArray(root->right, arr, index);
}

void swapProd(Product *a, Product *b) {
    Product temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Product *arr, int low, int high) {
    double pivot = arr[high].price;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].price <= pivot) {
            i++;
            swapProd(&arr[i], &arr[j]);
        }
    }
    swapProd(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(Product *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void display_sorted(BSTNode *root) {
    int count = countNodes(root);
    if (count == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    Product *arr = (Product *)malloc(count * sizeof(Product));
    if (arr == NULL) {
        printf("Memory error\n");
        return;
    }

    int idx = 0;
    toArray(root, arr, &idx);

    quickSort(arr, 0, count - 1);

    printf("\n--- Sorted by Price ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Name: %s | Price: $%.2f | Stock: %d\n",
               arr[i].id, arr[i].name,
               arr[i].price, arr[i].qty);
    }

    free(arr);
}

