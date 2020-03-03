//
// Created by Clément Lefevre on 2020-02-05.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define FILENAMESIZE 256
#define NBVALUES 512
#define UNDEFINED -1

/** HEADERS **/
int randomGenerator(int min, int max);
void randomGraphGenerator(int*** graph, int size, char file[FILENAMESIZE]);
void writeGraphToFile(int** graph, int size, char filename[FILENAMESIZE]);
bool isAlreadyStored(int value, int storedValues[NBVALUES], int nbValues);
void printIntMatrix(int** matrix, int size);

/** MAIN **/
int main(int argc, char* argv[]) {
    time_t t;
    srand((unsigned) time(&t)); // srand initialization

    int opt; 
    int** graph;

    int size = 0;
    char filename[FILENAMESIZE] = "graphs/sample.dat";

    // Command line parsing
    while((opt = getopt(argc, argv, "f:s:")) != -1) {
        switch(opt) {
            case 's':
                size = atoi(optarg);
                printf("Setting size to %d\n", size);
                break;
            case 'f':
                strcpy(filename, optarg);
                printf("Setting filename to %s\n", filename);
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                break;
        }
    }

    if(size != 0) {
    	printf("A random graph of size %d will be saved to file %s..\n", size, filename);
        randomGraphGenerator(&graph, size, filename);
    } else {
        printf("Bad usage.\nRecommended usage : \n\t./generator -s [size] -f [filename]\n");
    }

    return 0;
}


/** FUNCTIONS **/

/**
 * Generate a graph randomly
 * @param graph
 * @param size
 * @param file
 */
void randomGraphGenerator(int*** graph, int size, char file[FILENAMESIZE]) {
    printf("Generating graph of %d pages...\n", size);
    // Allocate memory
    *graph = (int**) malloc((size + 1) * sizeof(int*));
    for(int i = 0; i < size + 1; i++) {
        (*graph)[i] = (int*) malloc((size + 1) * sizeof(int));
        (*graph)[0][i] = UNDEFINED;
    }

    // First elemnt = size of graph
    (*graph)[0][0] = size;
    for(int i = 1; i < size + 1; i++) {
        (*graph)[i][0] = i - 1;
        int numArcs = randomGenerator(1, size);
        printf("Generating %d arcs from page %d...\n", numArcs, i - 1);
        for(int j = 1; j < size + 1; j++) {
            if(j <= numArcs) {
                int value;
                int storedValues[numArcs];
                do {
                    value = randomGenerator(0, size - 1);
                } while(isAlreadyStored(value, storedValues, numArcs));
                storedValues[j] = value;
                (*graph)[i][j] = value;
            } else {
                (*graph)[i][j] = UNDEFINED;
            }
        }
    }

    printIntMatrix(*graph, size + 1);
    writeGraphToFile(*graph, size, file);
}


/**
 * Generate a random number in a range
 * @param min
 * @param max
 * @return
 */
int randomGenerator(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}


/**
 * Check is a value is already stored in array
 * @param value 
 * @param storedValues 
 * @param nbValues 
 * @return 
 */
bool isAlreadyStored(int value, int storedValues[NBVALUES], int nbValues) {
    for(int i = 0; i < nbValues; i++) {
        if(value == storedValues[i]) {
            return true;
        }
    }
    return false;
}

/**
 * Write a graph to a file
 * @param graph
 * @param size
 * @param filename
 */
void writeGraphToFile(int** graph, int size, char filename[FILENAMESIZE]) {
    FILE *fp = fopen(filename, "w");

    if(fp) {
    	fprintf(fp, "%d\n", graph[0][0]);
        for(int i = 1; i < size + 1; i++) {
            for(int j = 0; j < size + 1; j++) {
                if(graph[i][j] != UNDEFINED) {
                    fprintf(fp, "%d ", graph[i][j]);
                } else {
                	fprintf(fp, "-1 ");
                }
            }
            fprintf(fp, "\n");
        }
    }
}

/**
 * Print the content of a given matrix (of integer)
 * @param matrix
 * @param size
 */
void printIntMatrix(int** matrix, int size) {
    printf("\n");
    printf("********** MATRIX **********\n");
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(matrix[i][j] == UNDEFINED) {
                printf("   ");
            } else {
                printf("%d  ", matrix[i][j]);
            }
        }
        printf("\n");
    }
    printf("****************************\n\n");
}
