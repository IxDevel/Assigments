#include<stdio.h>
#include <malloc.h>
#include <time.h>

/*  Generate 100 random numbers on a list
 *  Created at: 22-09-2020
 *  Last modification: 22-09-2020
 *  Made by: Isaac Da Pena
 */

/*  Define a structure to load the data
 *  "value" is used to store the weight
 *  "next" is the pointer to the next struct and its the way that you can access the next data
 */
struct list {
    int value;
    struct list *next;
};

/* Just for print the list that we defined
 */
void printList(struct list *dataList) {
    printf("\nList: \n\t");
    for (int i = 0; dataList != NULL; dataList = dataList->next, i++) {
        printf("[%2d]->", dataList->value);
        if(i > 4) {
            i = -1;
            printf("\n\t");
        }
    }
    printf("NULL\n");
}

/* This function inserts on the top of the list (like a stack) the values
 */
void insertValueOnTop (struct list **Top, int value) {
    // malloc is an spacial function that allow us to reserve space in main memory (ram)
    struct list *newTop = (struct list *) malloc(sizeof(struct list));
    // Save the value
    newTop->next = *Top;
    // Point to the next value
    newTop->value = value;
    // Track the new top of the list
    *Top = newTop;
}

void swap (int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void bubbleSort(struct list *l) {
    if(l) {
        struct list *end = NULL;
        struct list *aux;
        int changes;
        while(1) {
            changes = 0;
            for(aux = l; aux->next != end; aux = aux->next) {
                if(aux->value > aux->next->value) {
                    swap(&(aux->value), &(aux->next->value));
                    changes++;
                }
            }
            end = aux;
            if(end == l->next || !changes)
                break;
        }
    }
}


int main(void) {
    struct list *randList = NULL;
    srand((unsigned) time(NULL));
    for (int i  = 0; i < 100; i++)
        insertValueOnTop(&randList, rand() % 100);
    bubbleSort(randList);
    printList(randList);
}

