# Linked List Implementation by Martin Mendl

I have created an implementation of a Linked List, which can be used as a dynamically allocated, changing array in C.

## Motivation

My motivation behind its creation was to streamline the handling of arrays in C.

### The Problem

In C, you have limited options when it comes to storing data in an array. Either you can use fixed-length arrays, like:

```c
int array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
```

Once created, you cannot do much with them. You can reassign and read values, but operations like Python's .pop(index)—which pops the element at some index, reduces the array size, and returns the element—are not easily achievable.

Another issue is that after a certain size, an array may not be allocatable, and your program might crash due to system memory limitations.

You can overcome size limitations with malloc() in C, but that doesn't change the array's lack of flexibility. Additionally, malloc() (by my understanding) tries to allocate contiguous memory, which might not always be feasible, especially when working with large, different structures.

The Linked List Solution
This is where my Linked List implementation comes in. Linked Lists consist of small nodes, similar to elements in an array, but each node is malloced individually. There are virtually no size limitations. Each node (which is a struct) contains a data field, which is a void pointer, allowing you to store any data type (int, char, struct, etc.).

Because this is C, some casting is necessary, like:
```c
(void *)&removedData
```
or

```c
int *data = (int *)getDataAtIndex(list, i);. 
```

But once you're familiar with this, you won't have to worry about it.

Functions

# initLinkedList()
Initializes the linked list.
Example usage:

```c
struct LinkedList *list = initLinkedList();
```

# emptyList()
Returns true if the list is empty, otherwise returns false.

# insertNodeAtIndex()
Inserts a node at a given index. The data is a pointer to some malloc'ed memory. Example usage:
```c
InsertNodeAtIndex(list, data, 1);
```

# popNodeAtIndex()
Removes the data at a given index and saves it into a provided variable, similar to Python's data = array.pop(3).
Example usage:

```c
popNodeAtIndex(list, 1, (void *)&removedData);
```

# removeNodeAtIndex()
Removes the data at a given index without returning it, effectively destroying it. Example usage:

```c
removeNodeAtIndex(list, -2);
```

# replaceDataAtIndex()
Replaces the data at a given index. If you need to save the overwritten data, provide a valid pointer; otherwise, use NULL to destroy the original data.
Example usage:

```c
replaceDataAtIndex(list, 3, data_pointer, return_data);
```

# removeList()
Frees all the nodes and allocated memory associated with the linked list.
Example usage:

```c
removeList(list);
```

# getSize()
Returns the number of items stored in the list.

# printList()
For debugging purposes. It prints the list in the format:

```c
{
    0:data1,
    1:data2,
    ...
    n:datan
}
```

Since the list can store different types of data, you need to create a custom print function for each element type.
Example usage:

```c
// For integers
printList(list, (void (*)(unsigned int, void *))print_int);

// For characters
printList(list, (void (*)(unsigned int, void *))print_char);

```

The print_int and print_char functions are simple:

```c
// Function to print an integer
void print_int(unsigned int idx, void* data) {
    printf("    %d: %d", idx,  *(int*)data);
}

// Function to print a character
void print_char(unsigned int idx, void* data) {
    printf("    %d: %c", idx, *(char*)data);
}

```

# getDataAtIndex()
Accesses the value at a given index.
Example usage:

```c
int *data = (int *)getDataAtIndex(list, i);
```

Again, casting like (int *) is necessary but manageable.

Disadvantages

This implementation relies on you using malloc'ed variables in each node (each index stores a pointer to the data). Based on my understanding, this is something expected of us in C programming. We should structure our code to have functions that create and modify structs, which is exactly how this linked list is constructed.

Performance

Inserting: O(n/2) (each node knows both the previous and next node).
Accessing: Same as inserting.
Final Notes

You can also use negative indexing in all functions (e.g., index -1 is the last item in the list, -2 is the second-to-last, etc.).