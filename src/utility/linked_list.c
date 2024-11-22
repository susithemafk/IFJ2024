/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file linked_list.c
 * @date 28.9. 2024
 * @brief Implementation of universal linked list
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utility/linked_list.h"

// Function to check, if list is empty
int emptyList(struct LinkedList *list) {

    // check for internal error
    if (list == NULL) return -1;

    // check if the list is empty
    if (list->size == 0) return 1;

    return 0;
}

// Function to get the size of the list
unsigned int getSize(struct LinkedList *list) {
    
    // check for internal error
    if (list == NULL) return 0;
    return list->size;
}



// function to init the linked list
struct LinkedList *initLinkedList(bool freeData) {
    
    // create the instance of the linked list
    struct LinkedList *list = (struct LinkedList *)malloc(sizeof(struct LinkedList)); 

    if (list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->lastAccessedIndex = 0;
    list->lastAccessedNode = NULL;
    list->freeData = freeData;

    return list;
}

// Function to create a new node
struct Node *_createNode(void *data) {

    // alocate space for the new node
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));

    if (node == NULL) return NULL;

    // intialize the node
    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

// Function to insert a node at the start of the list
void _insertNodeAtStart(struct LinkedList *list, struct Node *node) {

    // if the list is empty insert the new node
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        list->lastAccessedIndex = 0;
        list->lastAccessedNode = node;
        list->size++;
        return;
    }

    // insert the node at the start
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
    list->lastAccessedIndex = 0;
    list->lastAccessedNode = node;

    // in case we have size of list one, we need to remove the tail 
    if (list->size == 1) list->tail = list->head->next;

    list->size++;
}

// Function to insert a node at the end of the list
void _addNodeToEnd(struct LinkedList *list, struct Node *node) {

    // if the list is empty insert the new node
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        list->lastAccessedIndex = 0;
        list->lastAccessedNode = node;
        return;
    }

    // insert the node at the end
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
    list->lastAccessedIndex = list->size;
    list->lastAccessedNode = node;

    // in case we have size of list one, we need to remove the head
    if (list->size == 1) list->head = list->tail->prev;

    list->size++;
}

// Function to move the active element to some index
bool _moveActiveElement(struct LinkedList *list, int index) {

    // convert the index to the positive index
    unsigned int newIndex = (index < 0) ? list->size + index : (unsigned int)index;

    // check for out of bounds
    if (newIndex >= list->size) return false;

    // if the index is the same as the last accessed index, return true
    if (list->lastAccessedIndex == newIndex) return true;

    // chose the direction of the search and the start node
    unsigned int steps;
    enum searchDirection direction;
    struct Node *startNode;
    int disHead, disTail, disLast;
    disHead = (int)newIndex;
    disTail = (int)list->size-1 - (int)newIndex;
    disLast = (int)list->lastAccessedIndex - (int)newIndex;

    // choose the smaller of head or tail
    if (disHead < abs(disTail)) {
        steps = newIndex;
        direction = FORWARD;
        startNode = list->head;
    } else {
        steps = list->size-1 - newIndex;
        direction = BACKWARD;
        startNode = list->tail;
    }

    // if active element is closer, than choose it
    if (list->lastAccessedNode != NULL && (unsigned int)abs(disLast) < steps) {
        steps = (unsigned int)abs(disLast);
        direction = (disLast < 0) ? FORWARD : BACKWARD;
        startNode = list->lastAccessedNode;
    }

    // now we need to move the active element
    for (unsigned int i = 0; i < steps; i++) {
        if (direction == FORWARD) {
            startNode = startNode->next;
        } else {
            startNode = startNode->prev;
        }
    }

    // update the last accessed node and index
    list->lastAccessedNode = startNode;
    list->lastAccessedIndex = newIndex;

    return true;
}

// Function to insert a node before/after the active element
void _insertNodeBeforeAfterActive(struct LinkedList *list, struct Node *node, bool before) {

    // if the active element is the head
    if (list->lastAccessedIndex == 0 && !before) {
        _insertNodeAtStart(list, node);
        return;
    } 

    // if the active element is the tail
    if (list->lastAccessedIndex == list->size - 1 && before){
        _addNodeToEnd(list, node);
        return;
    }  

    // insert the node before or after the active element
    if (!before) { // the initial index >= 0
        node->next = list->lastAccessedNode;
        node->prev = list->lastAccessedNode->prev;
        list->lastAccessedNode->prev->next = node;
        list->lastAccessedNode->prev = node;
    } else { // the initial index < 0
        node->prev = list->lastAccessedNode;
        node->next = list->lastAccessedNode->next;
        list->lastAccessedNode->next->prev = node;
        list->lastAccessedNode->next = node;
    }

    list->lastAccessedNode = node;
    list->size++;

}

// Function to insert node at index
bool insertNodeAtIndex(struct LinkedList *list, void *data, int index) {

    // check for internal error
    if (list == NULL) return false;

    // create the new node
    struct Node *node = _createNode(data);
    if (node == NULL) return false;

    if (list->size == 0) {
        _insertNodeAtStart(list, node);
        return true;
    }
    
    // move the active elemnt to the index
    if (!_moveActiveElement(list, index)) return false;

    // insert the node before if index is < 0, else after
    _insertNodeBeforeAfterActive(list, node, index < 0);

    return true;
}

// Function to pop a node at the start
bool _popNodeAtStart(struct LinkedList *list, void **returnData) {

    struct Node *node = list->head;
    if (node == NULL) return false;

    *returnData = node->data;

    if (node->next == NULL) {
        list->tail = NULL;
        list->head = NULL;
        list->lastAccessedNode = NULL;
        list->lastAccessedIndex = 0;
        list->size = 0;
        free(node);
        return true;
    }

    // remove the node from the list
    node->next->prev = NULL;
    list->head = node->next;
    list->lastAccessedNode = list->head;
    list->lastAccessedIndex = 0;
    list->size--;

    // handle list size 1
    if (list->size == 1)  list->tail = list->head;

    free(node);

    return true;
}

// Function to pop a node at the end
bool _popNodeAtEnd(struct LinkedList *list, void **returnData) {

    struct Node *node = list->tail;
    if (node == NULL) return false;

    *returnData = node->data;

    if (node->prev == NULL) {
        list->tail = NULL;
        list->head = NULL;
        list->lastAccessedNode = NULL;
        list->lastAccessedIndex = 0;
        list->size = 0;
        free(node);
        return true;
    }

    // remove the node from the list
    node->prev->next = NULL;
    list->tail = node->prev;
    list->lastAccessedNode = list->tail;
    list->lastAccessedIndex = list->size - 1;
    list->size--;

    // handle list size 1
    if (list->size == 1)  list->head = list->tail;

    free(node);

    return true;
}

// Function to pop node at index
bool popNodeAtIndex(struct LinkedList *list, int index, void **returnData) {

    // check for internal error
    if (list == NULL) return false;

    // move the active elemnt to the index
    if (!_moveActiveElement(list, index)) return false;

    // head pop
    if (list->lastAccessedIndex == 0) return _popNodeAtStart(list, returnData);

    // tail pop
    if (list->lastAccessedIndex == list->size - 1) return _popNodeAtEnd(list, returnData);

    // remove the node
    struct Node *node = list->lastAccessedNode;
    if (node == NULL) return false;

    // remove the node from the list
    node->prev->next = node->next;
    node->next->prev = node->prev;

    // shifting the last accessed node to the next
    list->lastAccessedNode = node->next;

    // free the node
    *returnData = node->data;

    free(node);
    list->size--;

    return true;
}

// Function to remove a node at index
bool removeNodeAtIndex(struct LinkedList *list, int index) {

    void *data = NULL;
    bool result = popNodeAtIndex(list, index, (void *)&data);

    if (!result) return false;

    if (list->freeData) free(data);

    return true;
}

// Function to get data at indexexit
void *getDataAtIndex(struct LinkedList *list, int index) {

    // check for internal error
    if (list == NULL) return NULL;

    // move the active elemnt to the index
    if (!_moveActiveElement(list, index)) return NULL;

    return list->lastAccessedNode->data;
}


// Function to replace data at index
bool replaceDataAtIndex(struct LinkedList *list, int index, void *data, void **returnData) {

    // check for internal error
    if (list == NULL) return false;

    // move the active elemnt to the index
    if (!_moveActiveElement(list, index)) return false;

    // replace the data
    *returnData = list->lastAccessedNode->data;
    list->lastAccessedNode->data = data;

    return true;    
}


// Function to remove the list
bool removeList(struct LinkedList **list) {

    // check for internal error
    if (list == NULL || *list == NULL) return false;

    // remove all nodes
    while ((*list)->size > 0) {
        removeNodeAtIndex(*list, 0);
    }

    // free the list
    free(*list);
    *list = NULL;

    return true;
}


// function to print list content
void printList(struct LinkedList *list, void (*printFunc)(unsigned int, void *)) {

    int idx = 0;
    Node *node = list->head;
    printf("{\n");

    while (node != NULL) {
        // Call the print function with the current index and node data
        printFunc(idx, node->data);
        printf(",\n");
        node = node->next;
        idx++;
    }

    printf("}\n");
}

// Function to print the list linkages
void printListLinkages(struct LinkedList *list) {
    
    Node *node = list->head;
    printf("head: %p\n", (void *)list->head);   
    printf("tail: %p\n", (void *)list->tail);
    int index = 0;
    printf("\nList linkages: ");
    while (node != NULL) {
        printf("\nNode: %d:\n   curPtr: %p\n   nextPtr: %p\n   prevPtr: %p\n", index, (void *)node, (void *)node->next, (void *)node->prev);
        node = node->next;
        index++;
    }
}

// call like this -> (void (*)(unsigned int, void *))print_int
// Function prototype
void (*printFunc)(unsigned int, void*);

// Function to print an integer
void print_int(unsigned int idx, void* data) {
    printf("    %d: %d", idx,  *(int*)data);
}

// Function to print a character
void print_char(unsigned int idx, void* data) {
    printf("    %d: %c", idx, *(char*)data);
}
