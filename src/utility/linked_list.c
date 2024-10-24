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

struct LinkedList *initLinkedList(bool freeData) {
    
    // create the instance of the linked list
    struct LinkedList *list = (struct LinkedList *)malloc(sizeof(struct LinkedList)); 

    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->lastAccessedIndex = 0;
    list->lastAccessedNode = NULL;
    list->freeData = freeData;

    return list;
}

unsigned int _convertIndex(int index, unsigned int size) {
    return index < 0 ? (unsigned int)(size + index) : (unsigned int)index;
}

int emptyList(struct LinkedList *list) {

    if (list == NULL) {
        return -1;
    }

    if (list->size == 0) {
        return 1;
    }

    return 0;  
}

struct Node *_createNewNode(void *data, struct Node * next, struct Node * prev) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

    if (newNode == NULL)
        return NULL;

    newNode->data = data;
    newNode->next = next;
    newNode->prev = prev;

    return newNode;
}

struct Node *_findNode(struct LinkedList *list, unsigned int index) {

    if (emptyList(list) != 0) 
        return NULL;

    if (index == 0) 
        return list->head;

    if (index == list->size-1)
        return list->tail;

    if (index >= list->size)
        return NULL;


    /*
    In here we have a few options,
    1. we have some lastAccesedNode, that we can start at
    2. we dont have a lastAccesedNode, but we can start at the head or the tail
    conditions for each case:
    1) we calc the distance form the lastAccesedNode to the wanted Node
       - than we calculate the distance from the tail to the wanted node
       - distance from head to wanted node is just the index
       - we choose the smallest distance
    */

    struct Node *node = NULL;
    unsigned int distanceFromTail = list->size - index;
    unsigned int distanceFromHead = index;
    enum searchDirection direction; 

    unsigned int finalDistance = 0;

    // first we choose the closest form the head or tail
    if (distanceFromHead < distanceFromTail){
        node = list->head;
        direction = FORWARD;
        finalDistance = (unsigned int)distanceFromHead;
    } else {
        node = list->tail;
        direction = BACKWARD;
        finalDistance = distanceFromTail;
    }

    // now, in case the last accessed node is not NULL, we calculate the distance from it
    if (list->lastAccessedNode != NULL) {
        int distanceFromLastAccessed = (int)list->lastAccessedIndex - index;
        if ((unsigned int)(abs(distanceFromLastAccessed) < (int)finalDistance)) {
            node = list->lastAccessedNode;
            finalDistance = (unsigned int)abs(distanceFromLastAccessed);
            direction = distanceFromLastAccessed > 0 ? BACKWARD : FORWARD;
        }
    }

    // now we just iterate over the list
    for (unsigned int i = 0; i < finalDistance; i++) {
        if (direction == FORWARD) {
            node = node->next;
        } else {
            node = node->prev;
        }
    }

    list->lastAccessedIndex = index;
    list->lastAccessedNode = node;
    return node;
}

bool _addNodeToEnd(struct LinkedList *list, void *data) {
    
    if (emptyList(list) == -1)
        return false;

    // alacate memory for the new node
    struct Node *newNode = _createNewNode(data, NULL, list->tail);

    if (newNode == NULL)
        return false;

    list->size++;

    // if the list is empty, the new node will be the head, and the tail
    if (emptyList(list) == 1) {
        list->head = newNode;
        list->tail = newNode;
        return true;
    } 
    // if the list is not empty, the new node will be the new tail

    if (list->tail != NULL) {
        list->tail->next = newNode;
    } else {
        list->head = newNode; // if the list is empty
    }

    list->tail = newNode;

    return true;
}

bool _popTailNode(struct LinkedList *list, void **returnData) {

    if (emptyList(list) == 1)
        return NULL;

    struct Node * node = list->tail;
    *returnData = node->data;

    list->tail = node->prev;
    
    if (list->tail != NULL) {
        list->tail->next = NULL;
    }
    free(node);

    list->size--;

    if (list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
    }

    if (list->size == 1) {
        list->head = list->tail;
    }

    return true;
}

bool _insertNodeAtHead(struct LinkedList *list, void *data) {
    
    if (emptyList(list) == -1)
        return false;

    struct Node *newNode = _createNewNode(data, list->head, NULL);

    if (newNode == NULL)
        return false;

    list->size++;

    if (emptyList(list) == 1) {
        list->head = newNode;
        list->tail = newNode;
        return true;
    }

    if (list->head != NULL) {
        list->head->prev = newNode;
    } else {
        list->tail = newNode;
    }


    list->head = newNode;

    return true;
}

bool _popHeadNode(struct LinkedList *list, void **returnData) {

    if (emptyList(list) == 1)
        return NULL;

    struct Node *node = list->head;
    *returnData = node->data;

    list->head = node->next;

    if (list->head != NULL) {
        list->head->prev = NULL;
    }
    free(node);

    list->size--;

    if (list->size == 0) {
        list->head = NULL;
        list->tail = NULL;
    }

    if (list->size == 1) {
        list->tail = list->head;
    }

    return true;
}

bool insertNodeAtIndex(struct LinkedList *list, void *data, int index) {

    if (!index) {
        return _insertNodeAtHead(list, data);
    }

    unsigned int new_index = _convertIndex(index, list->size);

    if (new_index == list->size-1) {
        return _addNodeToEnd(list, data);
    }

    if (emptyList(list) != 0) {
        return false;
    }

    struct Node *node = _findNode(list, new_index);

    if (node == NULL)
        return false;

    struct Node *newNode = NULL;

    if (index > 0) {
        newNode = _createNewNode(data, node, node->prev);
    } else {
        newNode = _createNewNode(data, node->next, node);
    }

    if (newNode == NULL)
        return false;

    if (index > 0) {
        node->prev->next = newNode;
        node->prev = newNode;
    } else {
        node->next->prev = newNode;
        node->next = newNode;
    }

    list->size++;

    return true;
}

bool popNodeAtIndex(struct LinkedList *list, int index, void **returnData) {

    if (emptyList(list) != 0) 
        return false;

    unsigned int new_index = _convertIndex(index, list->size);

    if (new_index == 0) 
        return _popHeadNode(list, returnData);

    if (new_index == list->size-1) 
        return _popTailNode(list, returnData);

    struct Node *node = _findNode(list, new_index);

    if (node == NULL)
        return false;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    *returnData = node->data;
    
    free(node);

    list->size--;

    return true;
}

bool removeNodeAtIndex(struct LinkedList *list, int index) {
    void *data = NULL;
    if (!popNodeAtIndex(list, index, &data))
        return false;
    
    if (list->freeData) {
        free(data);
    }

    return true;
}

bool removeList(struct LinkedList **list) {
    if (list == NULL || *list == NULL) {
        return false; // Handle null pointer or empty list
    }

    LinkedList *list1 = *list;

    if (emptyList(list1) == -1) {
        return false;
    }

    struct Node *node = list1->head;
    struct Node *nextNode = NULL;

    // Free all nodes in the list
    while (node != NULL) {
        nextNode = node->next;
        if (list1->freeData && node->data) {
            free(node->data); // Free the node's data if freeData is set
            node->data = NULL;
        }
        free(node); // Free the node itself
        node = nextNode;
    }

    // Finally, free the LinkedList structure
    free(list1);

    // Set the original pointer to NULL to avoid dangling pointers
    *list = NULL;

    return true;
}


bool replaceDataAtIndex(struct LinkedList *list, int index, void *data, void **returnData) {

    if (emptyList(list) != 0 || data == NULL) {
        return false;
    }

    unsigned int new_index = _convertIndex(index, list->size);

    struct Node *node = _findNode(list, new_index);

    if (node == NULL) {
        return false;
    }

    if (returnData != NULL) {
        *returnData = node->data;
    } else if (list->freeData) {
        free(node->data);
    }

    node->data = data; // need to test this

    return true;
}

unsigned int getSize(struct LinkedList *list) {
    if (list == NULL) {
        return -1;
    }
    return list->size;
}

void *getDataAtIndex(struct LinkedList *list, int index) {
    if (list == NULL) {
        return NULL;
    }

    unsigned int new_index = _convertIndex(index, list->size);
    struct Node *node = _findNode(list, new_index);

    if (node == NULL) {
        return NULL;
    }

    return node->data;
}

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
