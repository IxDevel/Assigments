#include<stdio.h>
#include <stdlib.h>

#define INPUT_FILE_NAME "input.txt"

/*  Week 4 Assignment: Using array and files
 *  Created at: 30-09-2020
 *  Last modification: 30-09-2020
 *  Made by: Isaac Da Pena
 */

/*
 *  This is an ordered tree definition with no value reps allowed
 */
struct node {
    int value;
    struct node *left;  // If a node is on the left of a node it means that is lower (value)
    struct node *right; // If a node is on the right of a node it means that is grater (value)
};

/*
 *  Create a new node
 */
struct node *createNode (int value) {
    struct node *newNode = (struct node *) malloc(sizeof(struct node));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/*
 *  Insert a new node in an tree (No reps are allowed)
 */
void insertOnTree(struct node **tree, int value) {
    if (*tree == NULL)
        *tree = createNode(value);
    else {
        for (struct node *aux = *tree; 1; ) {
            if (aux->value > value) {
                if (aux->left == NULL) {
                    aux->left = createNode(value);
                    break;
                }
                else
                    aux = aux->left;
            }
            else if (aux->value < value) {
                if (aux->right == NULL) {
                    aux->right = createNode(value);
                    break;
                } else
                    aux = aux->right;
            }
            else
                break;  // No rep values
        }
    }
}

/*
 *  Convert the array to a tree
 */
void arrayToTree (const int *dataArray, const int dataSize, struct node **tree) {
    for (int i = 0; i < dataSize; i++)
        insertOnTree(tree, dataArray[i]);
}

/*
 *  Print the tree in-order
 */
void inOrder (const struct node *tree) {
    if (tree != NULL) {
        inOrder(tree->left);
        printf("[%3d] ", tree->value);
        inOrder(tree->right);
    }
}

/*
 *  Print the tree in pre-order
 */
void preOrder (const struct node *tree) {
    if (tree != NULL) {
        printf("[%3d] ", tree->value);
        preOrder(tree->left);
        preOrder(tree->right);
    }
}

/*
 *  Print the tree in post-order
 */

void postOrder (const struct node *tree) {
    if (tree != NULL) {
        postOrder(tree->left);
        postOrder(tree->right);
        printf("[%3d] ", tree->value);
    }
}

/*  This function loads the data and stores it on the array
 *  The file must be made in this format (Each number per line)
 *  Eg.
 *  4
 *  2
 *  1
 *  4
 *  5
 *  The file name must be input.txt
 */
void loadData (int **dataArray, int *dataSize, const char *filePath) {
    // We need a file handler to manage a file
    FILE *fileHandler = fopen(filePath, "rt");
    // Auxiliary variables
    char buffer[20];
    // Clean old dataArray
    *dataSize = 0;
    free(*dataArray);
    // If we didn't found or open the file -> exit
    if(fileHandler == NULL) {
        printf("File not found or can't be open\n");
        return;
    }
    fscanf(fileHandler, "%d", dataSize);
    if (*dataSize < 1) {
        printf("Invalid file format\n");
        return;
    }
    *dataArray = (int *) malloc(sizeof(int)*(*dataSize));
    for (int i=0; i < *dataSize; i++) {
        fscanf(fileHandler,"%d", *dataArray+i);
    }
    fclose(fileHandler);
}

int main (void) {
    int *dataArray = NULL;
    int dataSize = 0;
    struct node *tree = NULL;
    // Load the data from the file
    loadData(&dataArray, &dataSize, INPUT_FILE_NAME);
    // Convert the array to a tree
    arrayToTree(dataArray, dataSize, &tree);
    // You need to use at least two traces in order to know the structure of the tree
    printf("\nInorder: ");
    inOrder(tree);
    printf("\nPreorder: ");
    preOrder(tree);
    printf("\nPostorder: ");
    postOrder(tree);
    return 0;
}
