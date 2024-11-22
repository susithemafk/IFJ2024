/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file test_list.c
 * @date 28.9. 2024
 * @brief Test file for the linked list implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include "utility/linked_list.h"
#include "utility/enumerations.h"

void test1(void) {

    TestInstancePtr testInstance = initTestInstance("Linked List Test with ints, malloc");

    // Initialize the list
    struct LinkedList *list = initLinkedList(true);
    testCase(
        testInstance,
        list != NULL,
        "Initializing the linked list",
        "List initialized successfully (expected)",
        "Failed to initialize list (unexpected)"
    );

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

    // list should be in the end in this order: 20, 40, 50, 10, 30

    // Test 1: Insert node at the start
    testCase(
        testInstance,
        insertNodeAtIndex(list, (void*)data1, 0),
        "Inserting node at the start (data1)",
        "Successfully inserted node at the start (expected)",
        "Failed to insert node at the start (unexpected)"
    );
    // list -> 10
    printList(list, (void (*)(unsigned int, void *))print_int);
    printListLinkages(list);


    // Test 2: Insert Node at start again
    testCase(
        testInstance,
        insertNodeAtIndex(list, (void*)data2, 0),
        "Inserting node at the start (data3)",
        "Successfully inserted node at the start (expected)",
        "Failed to insert node at the start (unexpected)"
    );
    // list -> 20, 10
    printList(list, (void (*)(unsigned int, void *))print_int);
    printListLinkages(list);


    // Test 3: Insert node at the end
    testCase(
        testInstance,
        insertNodeAtIndex(list, (void*)data3, -1),
        "Inserting node at the end (data2)",
        "Successfully inserted node at the end (expected)",
        "Failed to insert node at the end (unexpected)"
    );

    // list -> 20, 10, 30
    printList(list, (void (*)(unsigned int, void *))print_int);
    printListLinkages(list);

    // Test 4: Insert node at a specific index
    testCase(
        testInstance,
        insertNodeAtIndex(list, (void*)data4, 1),
        "Inserting node at index 1 (data4)",
        "Successfully inserted node at index 1 (expected)",
        "Failed to insert node at index 1 (unexpected)"
    );

    // list -> 20, 40, 10, 30
    printList(list, (void (*)(unsigned int, void *))print_int);
    printListLinkages(list);


    // Test 5: Insert node at a negative index (second-to-last)
    testCase(
        testInstance,
        insertNodeAtIndex(list, (void*)data5, -2),
        "Inserting node at index -2 (data5)",
        "Successfully inserted node at index -2 (expected)",
        "Failed to insert node at index -2 (unexpected)"
    );
    // list -> 20, 40, 10, 50, 30
    printList(list, (void (*)(unsigned int, void *))print_int);
    printListLinkages(list);

    // Test 6: Attempt to pop node from an invalid index (out of bounds)
    int *removedData = NULL;
    testCase(
        testInstance,
        !popNodeAtIndex(list, 100, (void *)&removedData),
        "Popping node at invalid index 100",
        "Failed to pop node at invalid index (expected)",
        "Popped node at invalid index (unexpected)"
    );

    // Test 7: Pop node at a valid index (1)
    printf("$$ Popping node at index 1\n");
    if (popNodeAtIndex(list, 1, (void *)&removedData)) {
        printf("$$ Popped node at index 1: %d\n", *removedData);
    }
    printList(list, (void (*)(unsigned int, void *))print_int);

    // Test 7: Remove node using a negative index (last element)
    printf("$$ Removing node at index -1\n");
    testCase(
        testInstance,
        removeNodeAtIndex(list, -1),
        "Removing node at index -1",
        "Successfully removed node at index -1 (expected)",
        "Failed to remove node at index -1 (unexpected)"
    );
    printList(list, (void (*)(unsigned int, void *))print_int);

    // Test 8: Size check after multiple removals
    printf("$$ List size after removals: %u\n", getSize(list));

    printf("$$ Clearing the list, initing with malloc free setting\n");
    removeList(&list);  // this will free the list and its nodes

    // Clean up allocated data (if not already freed)
    free(data4); 
    // data 1, 3, 5 were removed when removing the list 
    // (they were still present in the list, so the function freed them)
    // data2 was removed when we called the removeNodeAtIndex
    // which removed the node and freed the data
    finishTestInstance(testInstance);
}

void test_2(void) {

    /*
    I will be testing these functions:
    popNodeAtIndex()
    insertNodeAtIndex()
    getDataAtIndex()
    */

    TestInstancePtr testInstance = initTestInstance("Linked List Test with remembered index");

    // Test: Setup linked list with 20 chars    
    LinkedList *list = initLinkedList(false);  // list won't free the data

    char data[20] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't'
    };

    printf("$$ Inserting 20 characters into the list\n");
    for (int i = 0; i < 20; i++) {
        insertNodeAtIndex(list, (void *)&data[i], 0);  // Insert at the end
    }
    printList(list, (void (*)(unsigned int, void *))print_char);

    // Test: Accessing from the middle
    printf("$$ Accessing index 10 (forward access)\n");
    char *removeData = (char *)getDataAtIndex(list, 10);  // Cast void* to char*
    testCase(
        testInstance,
        removeData != NULL && *removeData == 'j',
        "Accessing index 10",
        "Successfully accessed index 10 and got 'j'",
        "Failed to access index 10 or incorrect data"
    );
    printf("$$ Data at index 10: %c\n", *removeData);

    // Test: Access backwards to index 7
    printf("$$ Accessing index 7 (backward access from 10)\n");
    removeData = (char *)getDataAtIndex(list, 7);  // Cast void* to char*
    testCase(
        testInstance,
        removeData != NULL && *removeData == 'm',
        "Accessing index 7",
        "Successfully accessed index 7 and got 'm'",
        "Failed to access index 7 or incorrect data"
    );
    printf("$$ Data at index 7: %c\n", *removeData);

    // Test: Access forwards to index 13
    printf("$$ Accessing index 13 (forward access from 7)\n");
    removeData = (char *)getDataAtIndex(list, 13);  // Cast void* to char*
    testCase(
        testInstance,
        removeData != NULL && *removeData == 'g',
        "Accessing index 13",
        "Successfully accessed index 13 and got 'g'",
        "Failed to access index 13 or incorrect data"
    );
    printf("$$ Data at index 13: %c\n", *removeData);

    // Test: Access backwards to index 3
    printf("$$ Accessing index 3 (backward access from 13)\n");
    removeData = (char *)getDataAtIndex(list, 3);  // Cast void* to char*
    testCase(
        testInstance,
        removeData != NULL && *removeData == 'q',
        "Accessing index 3",
        "Successfully accessed index 3 and got 'q'",
        "Failed to access index 3 or incorrect data"
    );
    printf("$$ Data at index 3: %c\n", *removeData);

    // Test: Access forwards to index 18
    printf("$$ Accessing index 18 (forward access from 3)\n");
    removeData = (char *)getDataAtIndex(list, 18);  // Cast void* to char*
    testCase(
        testInstance,
        removeData != NULL && *removeData == 'b',
        "Accessing index 18",
        "Successfully accessed index 18 and got 'b'",
        "Failed to access index 18 or incorrect data"
    );
    printf("$$ Data at index 18: %c\n", *removeData);

    // Test: Pop a node from the middle (index 10)
    printf("$$ Popping node at index 10\n");
    testCase(
        testInstance,
        popNodeAtIndex(list, 10, (void *)&removeData) && *removeData == 'j',
        "Popping node at index 10",
        "Successfully popped node at index 10 and got 'j'",
        "Failed to pop node at index 10 or incorrect data"
    );
    printf("$$ Data popped at index 10: %c\n", *removeData);

    // Test: Final cleanup
    printf("$$ Removing, with malloc free setting\n");
    removeList(&list);  // this will free the list and its nodes

    printf("$$ Freeing list\n");
    finishTestInstance(testInstance);
}


int main(void) {
    test1();
    test_2();
    return 0;
}
