#ifndef AFD_H
#define AFD_H

#include "afn.h"
#include <string.h>
#include <math.h>

#define NO_TRANSITION -1
#define CHAR_NOT_FOUND -2
#define NOT_FOUND -10
#define MAX_SIZE 512

/** STRUCTURES **/
typedef struct {
	states_v states; // Ensemble de tous les états 
	chars_v alphabet; // Alphabet 
	int idInitial; // Etat initial 
	int** transitionTable; // Table des transitions
} afd_t;



/** HEADERS **/
afd_t afd_create_from_input();
bool simulate(afd_t a, char* word, int sizeOfWord);
void afd_print(afd_t a);
void make_bilan(int nbRows, int nbCols, int minimTable[nbRows][nbCols]);
int get_alphabet_id(afd_t a, char c);
afd_t construct_afd(afd_t a, int minimTable[a.alphabet.size + 1][a.states.size]);
afd_t minimize(afd_t a);

void afd_free(afd_t a);




#endif

