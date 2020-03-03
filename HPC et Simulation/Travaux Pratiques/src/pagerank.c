//
// Created by Clément Lefevre on 2020-02-05.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define FILENAMESIZE 256
#define UNDEFINED -1


/** HEADERS **/
double vectorNorm(double* currVect, double* prevVect, int size);
void fillInitialVector(int size, double** vector);
void printDoubleMatrix(double** matrix, int size);
void printIntMatrix(int** matrix, int size);
void printVector(double* vector, int size);
void matricxVectorProduct(double** matrix, double* vector, int size);
double vectorNorm(double* currVect, double* prevVect, int size);
void improvedPageRank(int** adjMatrix, double* initialVect, int size, double alpha, double epsilon);
void initVector(double** vector, int size, bool fillWithZeros);
void initDoubleMatrix(double*** matrix, int size, bool fillWithZeros);
void initIntMatrix(int*** matrix, int size, bool fillWithZeros);
void createAdjMatrixFromGraph(int*** adjMatrix, int** graph, int size);
void readGraphFromFile(int*** graph, int* size, char filename[FILENAMESIZE]);


/** MAIN **/
int main(int argc, char* argv[]) {
    int opt;
    int size = 0;
    double epsilon = 0;
    double alpha = 0;
    char filename[FILENAMESIZE] = "graphs/sample.dat";
    int** graph;
    double* vector;
    int** adjMatrix;

    // Command line parsing
    while((opt = getopt(argc, argv, "f:a:e:")) != -1) {
        switch(opt) {
            case 'f':
                strcpy(filename, optarg);
                printf("Setting filename to %s\n", filename);
                break;
            case 'a':
                alpha = atof(optarg);
                printf("Setting alpha to %.10f\n", alpha);
                break;
            case 'e':
                epsilon = atof(optarg);
                printf("Setting epsilon to %.10f\n", epsilon);
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                break;
        }
    }

    // Warning message
    if(epsilon == 0 || alpha == 0) {
    	printf("You can submit your own values for alpha and epsilon with the following usage :\nRecommended usage : \n\t./pagerank -f [filename] -e [epsilon] -a [alpha]\n");
    	printf("The program will continue with standards values..\n");
    	alpha = 0.85;
    	epsilon = 0.000001;
    }

    printf("Reading graph from file %s and start the improved pagerank algorithm with alpha = %.10f and epsilon = %.10f..\n", filename, alpha, epsilon);
	readGraphFromFile(&graph, &size, filename);
	printIntMatrix(graph, size + 1);
	
	printf("Creating Adjency Matrix..");
	createAdjMatrixFromGraph(&adjMatrix, graph, size);
    printIntMatrix(adjMatrix, size);

    printf("Please enter initial vector values : \n");
    fillInitialVector(size, &vector);
    printVector(vector, size);

    printf("Executing improved pagerank..");
    improvedPageRank(adjMatrix, vector, size, alpha, epsilon);

    return 0;
}


/** FUNCTIONS **/

/**
 * Read graph from file
 * @param graph
 * @param size
 * @param filename
 */
void readGraphFromFile(int*** graph, int* size, char filename[FILENAMESIZE]) {
	// Open the file
	FILE* file; 
	file = fopen(filename, "r");
	if(file) {
		fscanf(file, "%d", size); // Get the size to initialize array
		*graph = (int**) malloc((*size + 1) * sizeof(int*)); // Allocate memory
		for(int i = 0; i < (*size) + 1; i++) {
			(*graph)[i] = (int*) malloc((*size + 1) * sizeof(int)); // Allocate memory
            if(i == 0) { // First line
                (*graph)[0][0] = *size; // First element = size
                for(int j = 1; j < (*size) + 1; j++) { // Others = UNDEFINED (unused)
                    (*graph)[0][j] = UNDEFINED; 
                }
            } else {
                for(int j = 0; j < (*size) + 1; j++) {
                    fscanf(file, "%d", &(*graph)[i][j]);
                }
            }
		}
		fclose(file);
	} else {
		printf("File %s not found. Aborting..\n", filename);
		exit(1);
	}
}

/**
 * Calculate the norm of the difference of two vectors
 * @param currVect
 * @param prevVect
 * @param size
 * @return the norm of the difference of the two vectors
 */
double vectorNorm(double* currVect, double* prevVect, int size) {
    double sum = 0;
    for(int i = 0; i < size; i++) {
        sum += (currVect[i] - prevVect[i]) * (currVect[i] - prevVect[i]);
    }

    printf(">> Norm = %.10f\n", sqrt(sum));
    return sqrt(sum);
}


/**
 * Calculate the product of a matrix and a vector
 * @param matrix
 * @param vector
 * @param size
 */
void matricxVectorProduct(double** matrix, double* vector, int size) {
    double* res = (double*) malloc(size * sizeof(double));
    for(int i = 0; i < size; i++) {
        res[i] = 0;
        for(int j = 0; j < size; j++) {
            res[i] += matrix[i][j] * vector[j];
        }
    }

    for(int i = 0; i < size; i++) {
        vector[i] = res[i];
    }
}


/**
 * Improved Page Rank
 * @param adjMatrix
 * @param initialVect
 * @param size
 * @param alpha
 * @param epsilon
 */
void improvedPageRank(int** adjMatrix, double* initialVect, int size, double alpha, double epsilon) {
    /** INITIALIZATION **/
    // Probability Matrix
    double** A = NULL;
    initDoubleMatrix(&A, size, false);
    for(int i = 0; i < size; i++) {
        int sum = 0;
        for(int j = 0; j < size; j++) {
            sum += adjMatrix[i][j];
        }
        for(int j = 0; j < size; j++) {
            if(sum != 0) {
                A[j][i] = (alpha * adjMatrix[i][j] / sum) + (1 - alpha)/size;
            } else {
                A[j][i] = 1.0/size;
            }
        }
    }
    printDoubleMatrix(A, size);

    double* currentVect = initialVect;
    int k = 0;
    double* prevVect = NULL;
    initVector(&prevVect, size, true);

    /** ITERATION **/
    while(vectorNorm(currentVect, prevVect, size) > epsilon) {
        printf("ITERATION N°%d\n", k);
        // Vector switch
        for(int i = 0; i < size; i++) {
            prevVect[i] = currentVect[i];
        }
        // Product Matrix-Vector
        matricxVectorProduct(A, currentVect, size);
        k++;
    }
    printf("\nSuccessfully executed improved PageRank algorithm. Final vector is : ");
    printVector(currentVect, size);
}


/**
 * Create Adjency matrix from graph
 * @param adjMatrix
 * @param graph
 * @param size
 */
void createAdjMatrixFromGraph(int*** adjMatrix, int** graph, int size) {
    initIntMatrix(adjMatrix, size, true);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            int target = graph[i+1][j+1];
            if(target != UNDEFINED) {
                (*adjMatrix)[i][target] = 1;
            }
        }
    }
}


/**
 * Create position vector and fill it with user input
 * @param size
 * @param vector
 */
void fillInitialVector(int size, double** vector) {
    initVector(vector, size, false);
    *vector = (double*) malloc(size * sizeof(double));
    for(int i = 0; i < size; i++) {
        char buffer[50];
        printf("Vector[%d]: ", i);
        gets(buffer);
        (*vector)[i] = atof(buffer);
    }
}


/**
 * Print the content of a given matrix (of doubles element)
 * @param matrix
 * @param size
 */
void printDoubleMatrix(double** matrix, int size) {
    printf("\n");
    printf("********** MATRIX **********\n");
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            printf("%.10f  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("****************************\n\n");
    printf("\n");
}


/**
 * Print the content of a given vector
 * @param vector
 * @param size
 */
void printVector(double* vector, int size) {
    printf("\n");
    printf("********** VECTOR **********\n");
    for(int i = 0; i < size; i++) {
        printf("%.10f  ", vector[i]);
    }
    printf("\n****************************\n\n");
}


/**
 * Init a matrix
 * @param matrix
 * @param size
 * @param fillWithZeros
 */
void initDoubleMatrix(double*** matrix, int size, bool fillWithZeros) {
    *matrix = (double**) malloc(size * sizeof(double*));
    for(int i = 0; i < size; i++) {
        (*matrix)[i] = (double*) malloc(size * sizeof(double));
        if(fillWithZeros) {
            for(int j = 0; j < size; j++) {
                (*matrix)[i][j] = 0;
            }
        }
    }
}

/**
 * Init a matrix
 * @param matrix
 * @param size
 * @param fillWithZeros
 */
void initIntMatrix(int*** matrix, int size, bool fillWithZeros) {
    *matrix = (int**) malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++) {
        (*matrix)[i] = (int*) malloc(size * sizeof(int));
        if(fillWithZeros) {
            for(int j = 0; j < size; j++) {
                (*matrix)[i][j] = 0;
            }
        }
    }
}


/**
 * Init a vector
 * @param vector
 * @param size
 * @param fillWithZeros
 */
void initVector(double** vector, int size, bool fillWithZeros) {
    *vector = (double*) malloc(size * sizeof(double));
    if(fillWithZeros) {
        for(int i = 0; i < size; i++) {
            (*vector)[i] = 0;
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