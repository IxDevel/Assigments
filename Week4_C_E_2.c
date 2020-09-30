#include<stdio.h>
#include <stdlib.h>

#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "answer-hw3.txt"

/*  Week 4 Assignment: Using array and files
 *  Created at: 28-09-2020
 *  Last modification: 28-09-2020
 *  Made by: Isaac Da Pena
 */

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

/*  This function calc the average and get the biggest number from the dataArray
 */
void calcAverageAndMax (const int *dataArray, const int dataSize, double *avg, int *max) {
    if (dataSize < 1) {
        printf("Invalid data array\n");
        return;
    }
    *max = dataArray[0];
    *avg = 0;
    for (int i=0; i < dataSize; i++) {
        *avg += dataArray[i];
        if (dataArray[i] > *max) *max = dataArray[i];
    }
    *avg /= dataSize;
}

/*  This function just print and save :)
 */
void printAndSave (const double average, const int max) {
    FILE *fileHandler = NULL;
    printf("The average is: %lf\n", average);
    printf("The max is: %d\n", max);
    fileHandler = fopen(OUTPUT_FILE_NAME, "wt");
    if (fileHandler == NULL) {
        printf("Can't create the file\n");
        return;
    }
    fprintf(fileHandler, "Average: %lf\nMax: %d", average, max);
    fclose(fileHandler);
}

int main (void) {
    int *dataArray = NULL;
    int dataSize = 0;
    double average = 0;
    int max = 0;
    // Load the data from the file
    loadData(&dataArray, &dataSize, INPUT_FILE_NAME);
    // Calc the average and the max
    calcAverageAndMax(dataArray, dataSize, &average, &max);
    // Print and Save
    printAndSave (average, max);
    return 0;
}
