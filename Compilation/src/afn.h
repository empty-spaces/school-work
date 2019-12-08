#ifndef AFN_H
#define AFN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VECTOR_INITIAL_CAPACITY 4

/** STRUCTURES **/
// 1. Représente un état
typedef struct {
	int id; // Numéro de l'état
	bool isAcceptor; // Etat accepteur
} state_t;

typedef struct {
  int size; // Taille utilisée
  int capacity; // Taille totale disponible
  state_t* data; // Données
} states_v;


// 2. Représente une transition
typedef struct {
	state_t* starting; // Etat de départ
	char c; // Caractère sur lequel s'effectue la transition
	state_t* arrival; // Etat d'arrivée
} transition_t;

typedef struct {
  int size;      // Taille utilisée
  int capacity;  // total available slots
  transition_t* data;     // array of integers we're storing
} transitions_v;

typedef struct {
  int size;      // slots used so far
  int capacity;  // total available slots
  char* data;     // array of integers we're storing
} chars_v;


// 3. Représente un AFN (Automate Fini Non-Déterministe)
typedef struct {
	states_v states; // Ensemble de tous les états 
	chars_v alphabet; // Alphabet 
	int idInitial; // Etat initial 
	transitions_v transitions; // Transitions
} afn_t;


/** FONCTIONS AFN **/
afn_t afn_empty_language();
afn_t afn_empty_word();
afn_t afn_single_char(char car);
afn_t afn_union(afn_t a1, afn_t a2);
afn_t afn_concat(afn_t a1, afn_t a2);
afn_t afn_kleene_star(afn_t a);

void afn_print(afn_t a);
void afn_free(afn_t a);


/* FONCTIONS ÉTATS */
void init_states(states_v* states);
void append_state(states_v* states, state_t value);
state_t get_state(states_v* states, int index);
void set_state(states_v* states, int index, state_t value);
void double_capacity_states(states_v* states);
void free_states(states_v* states);


/* FONCTIONS TRANSITIONS */
void init_transitions(transitions_v* trs);
void append_transition(transitions_v* trs, transition_t value);
transition_t get_transition(transitions_v* trs, int index);
void set_transition(transitions_v* trs, int index, transition_t value);
void double_capacity_transitions(transitions_v* trs);
void free_transitions(transitions_v* trs);

/* FONCTIONS ALPHABET */
void init_alphabet(chars_v* chrs);
void append_char(chars_v* chrs, char value);
char get_char(chars_v* chrs, int index);
void set_char(chars_v* chrs, int index, char value);
void double_capacity_alphabet(chars_v* chrs);
void free_alphabet(chars_v* chrs);

#endif
