/** AUTHOR
 * 
 * @author <247581> Martin Mendl
 * @file linked_list.h
 * @date 28.9. 2024
 * @brief Header file for the linked list implementation
 */


#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

// Enum for starting point
enum searchDirection {
    FORWARD,
    BACKWARD
};

// Struct for Node
typedef struct Node {
    void *data;
    struct Node *next; // pointer to the next item in the list
    struct Node *prev;  // pointer to the previsous item in the list
} Node;

// Struct for LinkedList
typedef struct LinkedList {
    struct Node *head; // start point of the list
    struct Node *tail; // end poitn of the list
    unsigned int size; // size of the list
    unsigned int lastAccessedIndex; // index of the last accessed node
    struct Node *lastAccessedNode; // pointer to the last accessed node
    bool freeData; // Function pointer for freeing node data 
} LinkedList;

// >>>>>>>>>>>>>>>>> Function declarations <<<<<<<<<<<<<<<<<<<<

 /**
 * Allocates memory for a new LinkedList structure and assigns it to the pointer `list`.
 * 
 * The allocated memory is the size of the LinkedList structure.
 * 
 * @param freeData A boolean value, if true, when a node is removed, the data will be freed.
 * 
 * @return A pointer to the newly allocated LinkedList structure.
 */
struct LinkedList *initLinkedList(bool freeData);

/**
 * check, if the list is empty
 * 
 * @return int value, if 1, the list is empty, 0 it is not, -1 the list is NULL
 */
int emptyList(struct LinkedList *list);

/**
 * Insert a new Node at a specific index
 * 
 * @param list The list to insert the node into
 * @param data The data to add to the node
 * @param index The index to insert the node at
 * @note the data at the index will be pushed to the right if index > 0 else to the left
 * @return bool true if successful, false if an error occurred
 */
bool insertNodeAtIndex(struct LinkedList *list, void *data, int index);

/**
 * Pop a node from a specific index
 * 
 * @param list The list to remove the node from 
 * @param index The index of the node to remove
 * @param returnData pointer to which the remove data will be cased to
 * @return bool true if successful, false if an error occurred
 */
bool popNodeAtIndex(struct LinkedList *list, int index, void **returnData);

/**
 * Remove node at index
 * 
 * @param list The list to remove the node from
 * @param index The index of the node to remove
 * 
 * @return bool true if successful, false if an error occurred
 */
bool removeNodeAtIndex(struct LinkedList *list, int index);

/**
 * Remove the list
 * 
 * @param list The list to remove
 * 
 * @return bool true if successful, false if an error occurred
 */
bool removeList(struct LinkedList **list);

/**
 * Replace data at index
 * 
 * @param list The list to replace the data in
 * @param index The index, where the data should be replaced
 * @param data The new data
 * @param returnData a pointer, where the old data will be stored
 * @note if returnData is NULL, the old data will be freed
 * @return bool true if successful, false if an error occurred
 */
bool replaceDataAtIndex(struct LinkedList *list, int index, void *data, void **returnData);

/**
 * Get the size of the list
 * 
 * @param list The list to get the size of
 * @return The size of the list, -1 if the list is NULL
 */
unsigned int getSize(struct LinkedList *list);

/**
 * Get the data at a specific index
 * 
 * @param list The list to get the data from
 * @param index The index of the data to get
 * @return The data at the index, NULL if the list is NULL
 */
void *getDataAtIndex(struct LinkedList *list, int index);

/**
 * Function to create a new node
 * 
 * @param data The data to add to the node
 * @return The new node
 * @note this function is internal
 */
struct Node *_createNode(void *data);


/**
 * Function to insret a new node at the start of the list
 * 
 * @param list The list to insert the node into
 * @param node The node to insert
 * @return bool true if successful, false if an error occurred
*/
void _insertNodeAtStart(struct LinkedList *list, struct Node *node);


/**
 * Function to insert a new node at the end of the list
 * 
 * @param list The list to insert the node into
 * @param node The node to insert
 * @return bool true if successful, false if an error occurred
 */
void _insertNodeAtEnd(struct LinkedList *list, struct Node *node);

/**
 * Function to pop a node at the start
 * 
 * @param list The list to pop the node from
 * @param returnData The data of the popped node    
 * @return bool true if successful, false if an error occurred
 */
bool _popNodeAtStart(struct LinkedList *list, void **returnData);

/**
 * Function to pop a node at the end
 * 
 * @param list The list to pop the node from
 * @param returnData The data of the popped node
 * @return bool true if successful, false if an error occurred
*/
bool _popNodeAtEnd(struct LinkedList *list, void **returnData);

/**
 * Function to move the active element to some index
 * 
 * @param list The list to move the active element in
 * @param index The index to move the active element to
 * @return bool true if successful, false if an error occurred
 */
bool _moveActiveElement(struct LinkedList *list, int index);

/**
 * Function to insert a node before/after the active element
 * 
 * @param list The list to insert the node into
 * @param node The node to insert
 * @param before If true, the node will be inserted before the active element, else after
 * @return bool true if successful, false if an error occurred
 */
void _insertNodeBeforeAfterActive(struct LinkedList *list, struct Node *node, bool before);

/**
 * Print the list
 * 
 * @param list The list to print
 * @param printFunc The function to print the data
 */
void printList(struct LinkedList *list, void (*printFunc)(unsigned int, void *));

/**
 * Function to print the list linkages
 * 
 * @param list The list to print the linkages of
 */
void printListLinkages(struct LinkedList *list);


// Printing functions
void print_int(unsigned int idx, void* data);
void print_char(unsigned int idx, void* data);

#endif /* LINKED_LIST_H */
