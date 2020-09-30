#include<stdio.h>
#include <stdlib.h>
#include <time.h>

/*  Week 3 Honors Assignment: Double linked lists
 *  Created at: 30-09-2020
 *  Last modification: 30-09-2020
 *  Made by: Isaac Da Pena
 */
struct list {
    int value;
    struct list *next;
    struct list *prev;
};

/*
 *  Insert on the top of the list
 */
void insertOnTop(struct list **ppList, int value) {
    struct list *newList = (struct list*) malloc(sizeof(struct list));
    newList->next = *ppList;
    if (*ppList != NULL)
        (*ppList)->prev = newList;
    newList->prev = NULL;
    newList->value = value;
    *ppList = newList;
}

/*
 *  Print the list
 */
void printList(const struct list* pList) {
    printf("List: ");
    while (pList != NULL) {
        printf("[%2d]->", pList->value);
        pList = pList->next;
    }
    printf("NULL\n");
}

/*
 *  Delete an element of the list
 */
void deleteListNode(struct list **pList) {
    if (pList != NULL) {
        struct list *aux = *pList;
        *pList = (*pList)->next;
        if (*pList != NULL)
            (*pList)->prev = aux->prev;
        if (aux->prev != NULL)
            aux->prev->next = *pList;
        free(aux);
    }
}

/*
 * Search and delete concurrences of an number (First way)
 */
void delDupSearchAndDestroy(struct list *pList) {
    for (   ;pList != NULL; pList = pList->next)
        for (struct list *aux = pList->next; aux != NULL;   )
            if(pList->value == aux->value)
                deleteListNode(&aux);
            else
                aux = aux->next;
}

/*
 *  Generate a random list using an modulus
 */
void getRandList(struct list **ppList, const int size, const int modulus) {
    for (int i = 0; i < size; i++)
        insertOnTop(ppList, rand() % modulus);
}


/*
 *  Delete al nodes of the list
 */
void deleteList (struct list **ppList) {
    for (struct list *aux = *ppList; *ppList != NULL; aux = *ppList) {
        *ppList = (*ppList)->next;
        free(aux);
    }
}

/*
 *  Delete a sorted list
 */
void delDupSortAndDestroy(struct list *pList) {
    for (   ;pList != NULL; pList = pList->next)
        for (struct list *aux = pList->next; aux != NULL;   )
            if(pList->value == aux->value)
                deleteListNode(&aux);
            else
                break;
}

void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

/*
 *  Apply bubble sort algorithm to a list
 */
void listBubbleSort(struct list *pList) {
    if(pList != NULL) {
        struct list *end = NULL;
        struct list *aux;
        int changes;
        while(1) {
            changes = 0;
            for(aux = pList; aux->next != end; aux = aux->next)
                if(aux->value > aux->next->value) {
                    swap(&(aux->value), &(aux->next->value));
                    changes++;
                }
            end = aux;
            /*  This is an optimization of the bubble sort algorithm
             *  If you make a pas through the list and didn't change
             *  any value, it means the list is ordered
             */
            if(end == pList->next || !changes)
                break;
        }
    }
}

int main (void) {
    struct list *dList = NULL;
    srand((unsigned) time(NULL));
    // Generate a random list
    getRandList(&dList, 200, 50);
    // Print the list
    printf("\nInitial ");
    printList(dList);
    // Apply an search and destroy pattern
    delDupSearchAndDestroy(dList);
    printf("\nSearch and Destroy mode ");
    printList(dList);
    // Delete the old list
    deleteList(&dList);
    // Generate a new random list
    getRandList(&dList, 200, 50);
    // Print the new random list
    printf("\nInitial ");
    // Sort the list
    listBubbleSort(dList);
    printList(dList);
    printf("Sort and Destroy mode ");
    // Apply an search and destroy patter over a sorted list
    delDupSortAndDestroy(dList);
    printList(dList);
    return 0;
}
