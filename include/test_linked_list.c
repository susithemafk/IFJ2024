/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file test_linked_list.c
 * @date 28.9. 2024
 * @brief Test file for the linked list implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int main(void) {
    // Initialize the list
    struct LinkedList *list = initLinkedList();
    if (list == NULL) {
        printf("$$ Failed to initialize list\n");
        return 1;
    }

    // Allocate some test data
    int *data1 = malloc(sizeof(int));
    int *data2 = malloc(sizeof(int));
    int *data3 = malloc(sizeof(int));
    int *data4 = malloc(sizeof(int));
    int *data5 = malloc(sizeof(int));

    *data1 = 10;
    *data2 = 20;
    *data3 = 30;
    *data4 = 40;
    *data5 = 50;

    // Test 1: Add nodes to the end of the list
    printf("$$ Adding node to the end of the list (data1)\n");
    _addNodeToEnd(list, data1);
    printList(list, (void (*)(unsigned int, void *))print_int);

    printf("$$ Adding node to the end of the list (data2)\n");
    _addNodeToEnd(list, data2);
    printList(list, (void (*)(unsigned int, void *))print_int);

    // Test 2: Insert node at the head of the list
    printf("$$ Inserting node at head (data3)\n");
    _insertNodeAtHead(list, data3);
    printList(list, (void (*)(unsigned int, void *))print_int);

    // Test 3: Insert node at a specific index
    printf("$$ Inserting node at index 1 (data4)\n");
    insertNodeAtIndex(list, data4, 1);
    printList(list, (void (*)(unsigned int, void *))print_int);

    // Test 4: Insert node at a negative index (second-to-last)
    printf("$$ Inserting node at index -2 (data5)\n");
    insertNodeAtIndex(list, data5, -2);
    printList(list, (void (*)(unsigned int, void *))print_int);

    // Test 5: Attempt to pop node from an invalid index (out of bounds)
    printf("$$ Attempting to pop node at invalid index 100\n");
    int *removedData = NULL;
    if (!popNodeAtIndex(list, 100, (void *)&removedData)) {
        printf("$$ Failed to pop node at invalid index\n");
    }

    // Test 6: Pop node at a valid index (1)
    printf("$$ Popping node at index 1\n");
    if (popNodeAtIndex(list, 1, (void *)&removedData)) {
        printf("$$ Popped node at index 1: %d\n", *removedData);
        free(removedData);  // free the removed data
    }
    printList(list, (void (*)(unsigned int, void *))print_int);

    // Test 7: Remove node using a negative index (last element)
    printf("$$ Removing node at index -1\n");
    if (!removeNodeAtIndex(list, -1)) {
        printf("$$ Failed to remove node at index -1\n");
    } else {
        printf("$$ Removed node at index -1\n");
    }
    printList(list, (void (*)(unsigned int, void *))print_int);

    // Test 8: Size check after multiple removals
    printf("$$ List size after removals: %u\n", getSize(list));

    // Test 9: Try to pop from an empty list
    printf("$$ Clearing the list and attempting to pop\n");
    removeList(list);  // this will free the list and its nodes
    list = initLinkedList();  // reinitialize for test
    if (popNodeAtIndex(list, 0, (void *)&removedData)) {
        printf("$$ Popped node from empty list (unexpected)\n");
    } else {
        printf("$$ Could not pop node from empty list (expected)\n");
    }

    // Clean up allocated data (if not already freed)
    free(data4); 
    /*
    data 1, 3, 5 ware removed when removing the list 
    (they ware still present in the list, so the function freed them)

    data2 was removed, when we called the removeNodeAtIndex
    which removed the node, and freed the data
    */

    printf("$$ Freeing list\n");
    return 0;
}
