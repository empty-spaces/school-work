#include "afd.h"

// 1. Simule un AFD sur un mot passé en paramètre
bool simulate(afd_t a, char* word, int sizeOfWord) {
	int idState = a.idInitial;
	int idChar;
	// Parcours de chaque caractère du mot  
	for(int i = 0; i < sizeOfWord; i++) {
		idChar = get_alphabet_id(a,word[i]);
		if(idChar == CHAR_NOT_FOUND) // Si un caractère n'est pas présent dans l'alphabet de l'automate
			return false; // Alors le mot n'est pas reconnu par l'automate
		if(a.transitionTable[idState][idChar] == NO_TRANSITION)
			return false;
		idState = a.transitionTable[idState][idChar];
	}

	if(a.states.data[idState].isAcceptor) // Si l'état d'arrivée est accepteur
		return true; // Le mot appartient au langage
	return false; 
}


// 2. Renvoie l'AFD minimal reconnaissant le même langage que l'AFD passé en paramètre 
afd_t minimize(afd_t a) {
	int minimTable[a.alphabet.size + 2][a.states.size];
	int destState;
	bool loop;

	// 1er bilan (groupe accepteur - non accepteur)
	for(int i = 0; i < a.states.size; i++) {
		if(a.states.data[i].isAcceptor)
			minimTable[0][i] = 1;
		else
			minimTable[0][i] = 0;
	}

	do { // Tant Que les deux bilans sont différents
		loop = false;
		// Remplissage de la table
		for(int i = 1; i < a.alphabet.size + 1; i++) { 
			for(int j = 0; j < a.states.size; j++) {
				destState = a.transitionTable[j][i-1];
				minimTable[i][j] = minimTable[0][destState];
			}
		}

		// Post Bilan 
		make_bilan(a.alphabet.size+1,a.states.size,minimTable);

		// Comparaison des bilans
		for(int i = 0; i < a.states.size; i++) {
			if(minimTable[0][i] != minimTable[a.alphabet.size+1][i]) {
				loop = true;
				break;
			}
		}
		if(loop) // Nouveau bilan devient le pré-bilan
			for(int i = 0; i < a.states.size; i++) 
				minimTable[0][i] = minimTable[a.alphabet.size+1][i];

	} while(loop);

	return construct_afd(a,minimTable);
}


// 3. Renvoie l'index de l'alphabet d'un automate correspondant à un caractère passé en paramètre. 
// Renvoie CHAR_NOT_FOUND si le caractère n'est pas présent dans l'alphabet de l'automate
int get_alphabet_id(afd_t a, char c) {
	for(int i = 0; i < a.alphabet.size; i++) {
		if(a.alphabet.data[i] == c)
			return i;
	}
	return CHAR_NOT_FOUND;
}


// 4. Effectue le second bilan (groupe les colonnes identiques)
void make_bilan(int nbRows, int nbCols, int minimTable[nbRows][nbCols]) {
	bool identic;

	for(int i = 0; i < nbCols; i++)
		minimTable[nbRows][i] = 0;

	printf("Bilan = {");
	for(int i = 0; i < nbCols; i++) {
		for(int j = i; j < nbCols; j++) {
			identic = true;
			for(int k = 0; k < nbRows; k++) {
				if(minimTable[k][i] != minimTable[k][j]) {
					identic = false;
					break;
				}
			}
			if(identic)
				minimTable[nbRows][j] = minimTable[nbRows][i];
			else
				minimTable[nbRows][j] = minimTable[nbRows][i] + 1;
		}
		printf("%d,",minimTable[nbRows][i]);
	}  
	printf("}\n");
}


// 5. Construit l'AFD obtenu à partir de la table de minimisation et du précédent AFD
afd_t construct_afd(afd_t a, int minimTable[a.alphabet.size + 1][a.states.size]) {
	afd_t amin;

	amin.idInitial = 0;
	amin.alphabet = a.alphabet;

	init_states(&amin.states);

	for(int i = 0; i < a.states.size; i++) {
		bool in = false;
		for(int j = 0; j < amin.states.size; j++) {
			if(amin.states.data[j].id == minimTable[a.alphabet.size+1][i]) {
				in = true;
				break;
			}
		}
		if(!in) {
			state_t s = {minimTable[a.alphabet.size+1][i],false};
			if(a.states.data[i].isAcceptor)
				s.isAcceptor = true;
			append_state(&amin.states,s);
		}
	}

	amin.transitionTable = (int**) malloc(amin.states.size * sizeof(int*));
	for(int i = 0; i < amin.states.size; i++)
		amin.transitionTable[i] = (int*) malloc(amin.alphabet.size * sizeof(int));

	for(int i = 0; i < a.states.size; i++)
		for(int j = 1; j < amin.alphabet.size + 1; j++)
			amin.transitionTable[minimTable[a.alphabet.size+1][i]][j-1] = minimTable[j][i];

	return amin;
}


// 6. Affiche les informations relatives à un AFD
void  afd_print(afd_t a) {
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


    printf("Table des transitions: \n");
    printf("  || a | b \n----------\n");
	for(int i = 0; i < a.states.size; i++) {
		printf("%d || ",a.states.data[i].id);
		for(int j = 0; j < a.alphabet.size; j++) {
			if(a.transitionTable[i][j] == NO_TRANSITION)
				printf("-");
			else
				printf("%d",a.transitionTable[i][j]);
			if(j == a.alphabet.size - 1)
				printf("\n");
			else
				printf(" | ");
		}
	}
    printf("\n");

    printf("Alphabet = {");
	for(int i = 0; i < a.alphabet.size; i++) {
		char c = get_char(&a.alphabet,i);
		if(i == a.alphabet.size - 1)
			printf("%c",c);
		else
			printf("%c,",c);
	}
    printf("}\n");
}



// 7. Libère les ressources
void afd_free(afd_t a) {
	for(int i = 0; i < a.states.size; i++)
		free(a.transitionTable[i]);
	free(a.transitionTable);
	free_states(&a.states);
	free_alphabet(&a.alphabet);
}


// 8. Créée un automate fini déterministe avec interaction utilisateur
afd_t afd_create_from_input() {
	afd_t a;
	
	int nbCarac = 0;
	int nbStates = 0;
	char c;
	int idArrival = -1;

	init_alphabet(&a.alphabet);
	init_states(&a.states);
	
	/* Initialisation de l'alphabet */
	printf("Size of alphabet : ");
	scanf(" %d", &nbCarac);

	for(int i = 0; i < nbCarac; i++) {
		printf("Char %d of alphabet : ", i);
		scanf(" %c",&c);
		append_char(&a.alphabet,c);
	}

	/* Initialisation des états */
	printf("\nNumber of states : ");
	scanf(" %d", &nbStates);

	state_t s;
	for(int i = 0; i < nbStates; i++) {
		printf("State %d acceptor ? (y/n) : ",i);
		scanf(" %c",&c);
		if(c == 'y' || c == 'Y') {
			s.id = i;
			s.isAcceptor = true;
		}
		else {
			s.id = i;
			s.isAcceptor = false;
		}
		
		append_state(&a.states,s);
	}

	a.idInitial = 0;

	/* Initialisation de la table de transitions */
	a.transitionTable = (int**) malloc(nbStates * sizeof(int*));
	printf("\nTransition table :\n(Type '-1' if no transition)\n");
	for(int i = 0; i < nbStates; i++) {
		a.transitionTable[i] = (int*) malloc(nbCarac * sizeof(int));
		for(int j = 0; j < nbCarac; j++) {
			do {
				printf("%d->'%c'->",i, get_char(&a.alphabet,j));
				scanf(" %d",&idArrival);
			} while(idArrival < NO_TRANSITION || idArrival >= nbStates);

			a.transitionTable[i][j] = idArrival;
		}
	}

	return a;
}
















