#ifndef CORE_TYPES_H
#define CORE_TYPES_H

typedef struct {
    int id;
    char name[50];
    double price;
    int qty;
} Product;

typedef struct {
    int id;
    int prodId;
    int change;
} Order;

#endif

