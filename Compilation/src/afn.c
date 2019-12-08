#include "afn.h"

/** FONCTIONS **/

// 1. Renvoie un automate standard reconnaissant le langage vide
afn_t afn_empty_language() {
	afn_t a;
	
	a.idInitial = 0; // État initial = 0
	
	init_alphabet(&a.alphabet); // Langage vide
	
	init_states(&a.states);
	state_t s = {0,false}; // 1 seul état non-accepteur
	append_state(&a.states,s);
	
	init_transitions(&a.transitions); // Pas de transitions

	return a;
}

// 2. Renvoie un automate reconnaissant le langage composé du seul mot vide
afn_t afn_empty_word() {
	afn_t a;
	
	a.idInitial = 0; // État initial = 0
	
	init_alphabet(&a.alphabet);
	append_char(&a.alphabet,'\0'); // On peut représenter le mot vide par le caractère '\0'
	
	init_states(&a.states);
	state_t s = {0,true}; // 1 seul état accepteur
	append_state(&a.states,s);

	init_transitions(&a.transitions); // Pas de transitions

	return a;
}

// 3. Renvoie un automate reconnaissant le langage composé d'un mot d'un caractère passé en paramètre
afn_t afn_single_char(char c) {
	afn_t a;

	a.idInitial = 0; // État initial = 0
	
	init_alphabet(&a.alphabet); // L'alphabet contient le car. passé en paramètre
	append_char(&a.alphabet,c);
	
	init_states(&a.states); // 2 états
	state_t s1 = {0,false}; // Le premier (initial) non-accepteur
	state_t s2 = {1,true}; // Le 2nd est accepteur 
	append_state(&a.states,s1);
	append_state(&a.states,s2);

	init_transitions(&a.transitions); // On a une transition, de 0 par c vers 1
	transition_t tr = {&a.states.data[0],c,&a.states.data[1]};
	append_transition(&a.transitions,tr);

	return a;
}


// 4. Renvoie un automate standard reconnaissant l'union de deux langages 
afn_t afn_union(afn_t a1, afn_t a2) {
	afn_t a = a1;

	// ALPHABET
	bool contains = false;
	for(int i = 0; i < a2.alphabet.size; i++) {
		for(int j = 0; j < a.alphabet.size; j++) {
			if(a1.alphabet.data[j] == a2.alphabet.data[i]) {
				contains = true;
				break;
			}
		}
		if(!contains)
			append_char(&a.alphabet,get_char(&a2.alphabet,i));
	}

	// ETATS
	// Si l'un des deux états initiaux est accepteur, le nouvel état initial l'est aussi
	if(a2.states.data[a2.idInitial].isAcceptor)
		a.states.data[a.idInitial].isAcceptor = true;

	// On ajoute tous les états de a2
	for(int i = 0; i < a2.states.size; i++) {
		if(i != a2.idInitial) { // Sauf son état initial
			a2.states.data[i].id = a.states.size; 
			append_state(&a.states,a2.states.data[i]);
		}
	}
	
	// TRANSITIONS
	for(int i = 0; i < a2.transitions.size; i++) {
		if(a2.transitions.data[i].starting->id == a2.idInitial)
			a2.transitions.data[i].starting = &a.states.data[a.idInitial];
		append_transition(&a.transitions,a2.transitions.data[i]);
	}

	return a;
}


// 5. Renvoie un automate standard reconnaissant la concaténation de deux langages
afn_t afn_concat(afn_t a1, afn_t a2) {
	afn_t a = a1;

	// ALPHABET
	bool contains = false;
	for(int i = 0; i < a2.alphabet.size; i++) {
		for(int j = 0; j < a.alphabet.size; j++) {
			if(a1.alphabet.data[j] == a2.alphabet.data[i]) {
				contains = true;
				break;
			}
		}
		if(!contains)
			append_char(&a.alphabet,get_char(&a2.alphabet,i));
	}

	// TRANSITIONS
	// On parcours les transitions de a2
	for(int i = 0; i < a2.transitions.size; i++) {
		// Si elles ont pour état de départ l'ancien état initial s2
		if(a2.transitions.data[i].starting->id == a2.idInitial) { 
			for(int j = 0; j < a1.states.size; j++) { // Alors on ajoute autant de transitions
				if(a1.states.data[j].isAcceptor) { // qu'il n'y a d'états accepteurs dans a1
					transition_t tr = {&a1.states.data[j],a2.transitions.data[i].c,a2.transitions.data[i].arrival};
					append_transition(&a.transitions,tr);
				}
			}
		}
		else // Sinon, on recopie simplement la transition
			append_transition(&a.transitions,a2.transitions.data[i]);
	}

	// ETATS
	// Si l'état initial de a2 n'est pas accepteur
	if(!a2.states.data[a2.idInitial].isAcceptor) {
		for(int i = 0; i < a.states.size; i++) { // Les états finaux de a1 ne sont plus accepteurs
			if(a.states.data[i].isAcceptor)
				a.states.data[i].isAcceptor = false;
		}
	}
	// On ajoute tous les etats de a2
	for(int i = 0; i < a2.states.size; i++) { 
		if(i != a2.idInitial) { // Sauf son état initial
			a2.states.data[i].id = a.states.size;
			append_state(&a.states,a2.states.data[i]);
		}
	}

	return a;
}

// 6. Renvoie un automate standard reconnaissant la fermeture itérative de Kleene de son langage
afn_t afn_kleene_star(afn_t a) {
	// TRANSITIONS
	for(int i = 0; i < a.transitions.size; i++) {
		if(a.transitions.data[i].starting->id == a.idInitial) {
			for(int j = 0; j < a.states.size; j++) {
				if(a.states.data[j].isAcceptor && j != a.idInitial) {
					transition_t tr = {&a.states.data[j],a.transitions.data[i].c,a.transitions.data[i].arrival};
					append_transition(&a.transitions,tr);
				}
			}
		}
	}

	// L'état initial devient accepteur
	a.states.data[a.idInitial].isAcceptor = true;

	return a;
}

// 7. Fonction d'affichage d'un automate
void afn_print(afn_t a) {
	int nbAcceptors = 0;
	int cptAcceptors = 0;

	printf("Etats = {");
	for(int i = 0; i < a.states.size; i++) {
		state_t s = get_state(&a.states,i);
		if(i == a.states.size - 1)
			printf("%d",s.id);
		else
			printf("%d,",s.id);
		if(s.isAcceptor)
			nbAcceptors ++;
	}
    printf("}\n");

	printf("Etat initial = %d\n",a.idInitial);

	printf("Etats accepteurs = {");
	for(int i = 0; i < a.states.size; i++) {
		state_t s = get_state(&a.states,i);
		if(s.isAcceptor) {
			cptAcceptors ++;
			if(cptAcceptors == nbAcceptors)
				printf("%d",s.id);
			else
				printf("%d,",s.id);
		}
	}
    printf("}\n");


    printf("Transitions = {");
	for(int i = 0; i < a.transitions.size; i++) {
		transition_t tr = get_transition(&a.transitions,i);
		printf("(%d, %c, %d)", tr.starting->id, tr.c, tr.arrival->id);
		if(i < a.transitions.size - 1) 
			printf(",");
	}
    printf("}\n");

    printf("Alphabet = {");
	for(int i = 0; i < a.alphabet.size; i++) {
		char c = get_char(&a.alphabet,i);
		if(c == '\0')
			printf("eps.");
		else
			printf("%c",c);
		if(i < a.alphabet.size - 1)
			printf(",");
	}
    printf("}\n");
}

// 8. Libération de mémoire
void afn_free(afn_t a) {
	free_states(&a.states);
	free_transitions(&a.transitions);
	free_alphabet(&a.alphabet);
}


/** Fonctions de manipulations des tableaux dynamiques **/
/** ETATS **/
void init_states(states_v* states) {
  states->size = 0;
  states->capacity = VECTOR_INITIAL_CAPACITY;
  states->data = malloc(sizeof(state_t) * states->capacity);
}

void append_state(states_v* states, state_t value) {
  double_capacity_states(states);
  states->data[states->size++] = value;
}

state_t get_state(states_v* states, int index) {
  if (index >= states->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, states->size);
    exit(EXIT_FAILURE);
  }
  return states->data[index];
}

void set_state(states_v* states, int index, state_t value) {
  if (index >= states->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, states->size);
    exit(EXIT_FAILURE);
  }
  states->data[index] = value;
}

void double_capacity_states(states_v* states) {
  if (states->size >= states->capacity) {
    states->capacity *= 2;
    states->data = realloc(states->data, sizeof(state_t) * states->capacity);
  }
}

void free_states(states_v* states) {
  free(states->data);
}


/** TRANSITIONS **/ 
void init_transitions(transitions_v* trs) {
  trs->size = 0;
  trs->capacity = VECTOR_INITIAL_CAPACITY;
  trs->data = malloc(sizeof(transition_t) * trs->capacity);
}

void append_transition(transitions_v* trs, transition_t value) {
  double_capacity_transitions(trs);
  trs->data[trs->size++] = value;
}

transition_t get_transition(transitions_v* trs, int index) {
  if (index >= trs->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, trs->size);
    exit(EXIT_FAILURE);
  }
  return trs->data[index];
}

void set_transition(transitions_v* trs, int index, transition_t value) {
  if (index >= trs->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, trs->size);
    exit(EXIT_FAILURE);
  }
  trs->data[index] = value;
}

void double_capacity_transitions(transitions_v* trs) {
  if (trs->size >= trs->capacity) {
    trs->capacity *= 2;
    trs->data = realloc(trs->data, sizeof(transition_t) * trs->capacity);
  }
}

void free_transitions(transitions_v* trs){
  free(trs->data);
}


/** ALPHABET **/
void init_alphabet(chars_v* chrs) {
  chrs->size = 0;
  chrs->capacity = VECTOR_INITIAL_CAPACITY;
  chrs->data = malloc(sizeof(char) * chrs->capacity);
}

void append_char(chars_v* chrs, char value) {
  double_capacity_alphabet(chrs);
  chrs->data[chrs->size++] = value;
}

char get_char(chars_v* chrs, int index) {
  if (index >= chrs->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, chrs->size);
    exit(EXIT_FAILURE);
  }
  return chrs->data[index];
}

void set_char(chars_v* chrs, int index, char value) {
  if (index >= chrs->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, chrs->size);
    exit(EXIT_FAILURE);
  }
  chrs->data[index] = value;
}

void double_capacity_alphabet(chars_v* chrs) {
  if (chrs->size >= chrs->capacity) {
    chrs->capacity *= 2;
    chrs->data = realloc(chrs->data, sizeof(char) * chrs->capacity);
  }
}

void free_alphabet(chars_v* chrs) {
  free(chrs->data);
}



