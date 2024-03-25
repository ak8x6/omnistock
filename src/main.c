#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core_types.h"
#include "inventory_bst.h"
#include "order_queue.h"
#include "undo_stack.h"
#include "inventory_sort.h"

void show_menu() {
    printf("\n=== OmniStock Menu ===\n");
    printf("1. Add Product\n");
    printf("2. Display (ID)\n");
    printf("3. Add Order\n");
    printf("4. Process Order\n");
    printf("5. Undo\n");
    printf("6. Display (Price)\n");
    printf("7. Exit\n");
    printf("Choice: ");
}

int main() {
    InventoryTree inv;
    inv.root = NULL;

    OrderQueue q;
    init_queue(&q);

    UndoStack s;
    init_stack(&s);

    int choice;
    int orderCounter = 1;

    while (1) {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }

        if (choice == 1) {
            Product p;
            printf("ID: ");
            scanf("%d", &p.id);
            printf("Name: ");
            scanf("%s", p.name);
            printf("Price: ");
            scanf("%lf", &p.price);
            printf("Quantity: ");
            scanf("%d", &p.qty);

            insert(&inv, p);
            printf("Done!\n");
        }
        else if (choice == 2) {
            printf("\n--- Inventory ---\n");
            if (inv.root == NULL) {
                printf("Empty.\n");
            } else {
                display_inorder(inv.root);
            }
        }
        else if (choice == 3) {
            Order o;
            o.id = orderCounter++;
            printf("Product ID: ");
            scanf("%d", &o.prodId);
            printf("Change (-/+) : ");
            scanf("%d", &o.change);

            enqueue(&q, o);
            printf("Queued %d\n", o.id);
        }
        else if (choice == 4) {
            if (is_empty_q(&q)) {
                printf("No orders.\n");
            } else {
                Order o = dequeue(&q);
                BSTNode *node = search(inv.root, o.prodId);
                if (node != NULL) {
                    node->prod.qty += o.change;
                    printf("Processed %d. Stock: %d\n", o.id, node->prod.qty);
                    push(&s, o);
                } else {
                    printf("Not found.\n");
                }
            }
        }
        else if (choice == 5) {
            if (is_empty_s(&s)) {
                printf("Nothing to undo.\n");
            } else {
                Order o = pop(&s);
                BSTNode *node = search(inv.root, o.prodId);
                if (node != NULL) {
                    node->prod.qty -= o.change;
                    printf("Undid %d. Stock: %d\n", o.id, node->prod.qty);
                } else {
                    printf("Not found.\n");
                }
            }
        }
        else if (choice == 6) {
            display_sorted(inv.root);
        }
        else if (choice == 7) {
            printf("Bye.\n");
            break;
        }
        else {
            printf("Invalid.\n");
        }
    }

    destroy_tree(inv.root);
    destroy_queue(&q);
    destroy_stack(&s);

    return 0;
}

