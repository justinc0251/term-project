/* list.c 
Justin Chung 
6/4 COEN 12 Term Project
Linked List of Arrays implementation
*/

#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define arraySize 8

// Data structure for node
typedef struct node
{
	void **data; // array
    int first; // first variable
    int arrayCount; // number of items in array
    struct node *next; // next node
    struct node *prev; // prev node
} NODE;

// Data structure for list
typedef struct list{
    int nodeCount; // number of nodes or arrays
    int itemCount; // number of items in list
    NODE *head; // head pointer
    NODE *tail; // tail pointer
} LIST;

// Create List
// Big O: O(1)
LIST *createList(void)
{
    LIST *lp;
    lp = malloc(sizeof(LIST));
    assert(lp != NULL);

    lp->head = malloc(sizeof(NODE));
    assert(lp->head != NULL);
    lp->tail = lp->head;
    
    lp->head->next = NULL;
    lp->head->prev = NULL;
    
    lp->nodeCount = 0;
    lp->itemCount = 0;

    return lp;
}

// Destroy List
// Big O: O(n)
void destroyList(LIST *lp)
{
    assert(lp != NULL);
    NODE *p = lp->head;
    NODE *temp;
    while(temp != NULL)
    {
        temp = p->next;
        free(p->data);
        free(p);
        p = temp;
    }
    free(lp);
}

// Return Number of Items in list
// Big O: O(1)
int numItems(LIST *lp)
{
    assert(lp != NULL);
    return lp->itemCount;
}

// Add Item to Beginning of List
// Big O: O(1)
void addFirst(LIST *lp, void *item)
{
    assert(lp != NULL && item != NULL);
    if(lp->nodeCount == 0 || (lp->head->arrayCount == arraySize))
    {
        // Allocate New Node
        NODE* pNew = malloc(sizeof(NODE));
        assert(pNew != NULL);
        // Allocate Data Array
        pNew->data = malloc(sizeof(void *) * arraySize);
        assert(pNew->data != NULL);
        // Initialize Node Variables
        pNew->arrayCount = 0;
        pNew->first = 0;
        pNew->prev = NULL;
        // No nodes in list
        if(lp->nodeCount == 0)
        {
            lp->head = pNew;
            lp->tail = lp->head;
        }
        // Node is Full
        if(lp->head->arrayCount == arraySize)
        {
            pNew->next = lp->head;
            lp->head->prev = pNew;
            lp->head = pNew;
        }
        lp->nodeCount++;
    }
    lp->head->data[((lp->head->first - 1) + arraySize) % arraySize] = item;
    lp->head->first = ((lp->head->first - 1) + arraySize) % arraySize;
    lp->head->arrayCount++;
    lp->itemCount++;
    return;
}

// Add Item to End of List
// Big O: O(1)
void addLast(LIST *lp, void *item)
{
    assert(lp != NULL && item != NULL);
    if(lp->nodeCount == 0 || (lp->head->arrayCount == arraySize))
    {
        // Allocate New Node
        NODE* pNew = malloc(sizeof(NODE));
        assert(pNew != NULL);
        // Allocate Data Array
        pNew->data = malloc(sizeof(void *) * arraySize);
        assert(pNew->data != NULL);
        // Initialize Node Variables
        pNew->arrayCount = 0;
        pNew->first = 0;
        pNew->next = NULL;
        // No Nodes in List
        if(lp->nodeCount == 0)
        {
            lp->head = pNew;
            lp->tail = lp->head;
        }
        // Node is Full
        if(lp->head->arrayCount == arraySize)
        {
            pNew->prev = lp->tail;
            lp->tail->next = pNew;
            lp->tail = pNew;
        }
        lp->nodeCount++;
    }
    lp->tail->data[(lp->tail->arrayCount + lp->tail->first) % arraySize] = item;
    lp->tail->arrayCount++;
    lp->itemCount++;
    return;
}

// Remove Item from Beginning of List
// Big O: O(1)
void *removeFirst(LIST *lp)
{
    assert(lp != NULL && lp->itemCount > 0);
    void *deleted;
    // Delete node if node is empty
    if(lp->head->arrayCount == 0)
    {
        NODE *pDel = lp->head;
        lp->head=lp->head->next;
        lp->head->prev = NULL;
        free(pDel->data);
        free(pDel);
        lp->nodeCount--;
    }

    // Decrement Count of Items in Array and Count of items total
    lp->head->arrayCount--;
    lp->itemCount--;

    deleted = lp->head->data[lp->head->first];
    lp->head->data[lp->head->first] = NULL;
    lp->head->first = (lp->head->first + 1) % arraySize;
    return deleted;
}

// Remove Item From End Of List
// Big O: O(1)
void *removeLast(LIST *lp)
{
    assert(lp != NULL && lp->itemCount > 0);
    void *deleted;
    // Delete node if node is empty
    if(lp->tail->arrayCount == 0)
    {
        NODE *pDel = lp->tail;
        lp->tail=lp->tail->prev;
        lp->tail->next = NULL;
        free(pDel->data);
        free(pDel);
        lp->nodeCount--;
    }

    // Decrement Count of Items in Array and Count of items total
    lp->tail->arrayCount--;
    lp->itemCount--;
    
    deleted = lp->tail->data[(lp->tail->first + lp->tail->arrayCount - 1) % arraySize];
    lp->head->data[lp->head->first] = NULL;

    return deleted;
}

// Get First Item in List
// Big O: O(1)
void *getFirst(LIST *lp)
{
    assert(lp != NULL && lp->itemCount > 0);
    return lp->head->data[lp->head->first];
}

// Get Last Item in List
// Big O: O(1)
void *getLast(LIST *lp)
{
    assert(lp != NULL && lp->itemCount > 0);
    return lp->tail->data[(lp->tail->first + lp->tail->arrayCount - 1) % arraySize];
}

// Get Item at a Given Index
// Big O: O(n)
void *getItem(LIST *lp, int index)
{
    assert(lp != NULL && index >= 0);
    int location = 0;
    int i;
    NODE *p = lp->head;
    for(i = 0; i < lp->nodeCount; i++)
    {
        if(index < p->arrayCount)
        {
            location = index;
            break;
        }
        else
        {
            index -= p->arrayCount;
            p = p->next;
        }
    }
    return p->data[(p->first + location) % arraySize];
}

// Set item at a given index to the given item
// Big O: O(n)
void setItem(LIST *lp, int index, void *item)
{
    assert(lp != NULL && index >= 0);
    int location = 0;
    int i;
    NODE *p = lp->head;
    for(i = 0; i < lp->nodeCount; i++)
    {
        if(index < p->arrayCount)
        {
            location = index;
            break;
        }
        else
        {
            index -= p->arrayCount;
            p = p->next;
        }
    }
    p->data[(p->first + location) % arraySize] = item;
    return;
}