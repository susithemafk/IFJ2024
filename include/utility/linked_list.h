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
enum startPoint {
    HEAD, 
    TAIL
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
 * convert index to positive index
 * 
 * @param index The index to convert
 * @return unsigned int The converted index
*/
unsigned int _convertIndex(int index, unsigned int size);

/**
 * check, if the list is empty
 * 
 * @return int value, if 1, the list is empty, 0 it is not, -1 the list is NULL
 */
int emptyList(struct LinkedList *list);

/**
 * Create a new Node
 * 
 * @param data The data of the node,
 * @param next The next node in the list
 * @param prev The previous node in the list
 * @return The New node
*/
struct Node *_createNewNode(void *data, struct Node *next, struct Node *prev);

/**
 * Find a good starting poin, for the search, either the head or the tail
 * 
 * @param sizeOfList The size of the list
 * @param wantedIndex The index of the node to find
 * 
 * @return enum startPoint The starting point for the search
*/
enum startPoint _findStartPoint(int sizeOfList, unsigned int wantedIndex);

/**
 * Find next Node
 * 
 * @param node The node to start the search from
 * @param start The "direction" to search in
 * 
 * @return struct Node * The next node
*/
struct Node *_findNextNode(struct Node *node, enum startPoint start);

/**
 * Find node at index
 * 
 * @param list The list to search through
 * @param index The index of the node to find
*/
struct Node *_findNode(struct LinkedList *list, unsigned int index);

/**
 * Add a new node to the end of the list
 * 
 * @param list The list to add the node to
 * @param data The data to add to the node
 * @return int 0 if successful, -1 if an error occurred
 */
bool _addNodeToEnd(struct LinkedList *list, void *data);

/**
 * pop a node from the end of the list
 * 
 * @param list The list to remove the node from
 * @param returnData pointer to which the removed data will be caed to
 * @return bool true if successful, false if an error occurred
 */
bool _popTailNode(struct LinkedList *list, void **returnData);


/**
 * Insert a new node at index 0
 * 
 * @param list The list to insert the node into
 * @param data The data to add to the node
 * @return bool true if successful, false if an error occurred
 */
bool _insertNodeAtHead(struct LinkedList *list, void *data);

/**
 * Pop a node form the head of the list
 * 
 * @param list The list to remove the node from
 * @param returnData pointer to which the removed data will be caed to
 * @return bool true if successful, false if an error occurred
 */
bool _popHeadNode(struct LinkedList *list, void **returnData);

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
bool removeList(struct LinkedList *list);

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
 * Print the list
 * 
 * @param list The list to print
 * @param printFunc The function to print the data
 */
void printList(struct LinkedList *list, void (*printFunc)(unsigned int, void *));

// Printing functions
void print_int(unsigned int idx, void* data);
void print_char(unsigned int idx, void* data);

#endif /* LINKED_LIST_H */
